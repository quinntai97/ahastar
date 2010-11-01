#include "HPACluster.h"

#include "ClusterAStar.h"
#include "ClusterNode.h"
#include "graph.h"
#include "graphAbstraction.h"
#include "HPAUtil.h"
#include "IEdgeFactory.h"
#include <stdexcept>
#include <sstream>
#include <string>

HPACluster::HPACluster(int x, int y, int _w, int _h, 
		AbstractClusterAStar* _alg, graphAbstraction* map) : Cluster(map)
throw(std::invalid_argument)
{
	init(x, y, _width, _height, _alg);
}

HPACluster::HPACluster(const int x, const int y, 
		const int _width, const int _height) 
throw(std::invalid_argument)
{
	ClusterAStar* _alg = new ClusterAStar();
	try
	{
		init(x, y, _width, _height, _alg);
	}
	catch(std::invalid_argument e)
	{
		delete _alg;
		throw e;
	}
}

void 
HPACluster::init(const int x, const int y, const int _width, const int _height, 
		AbstractClusterAStar* _alg) throw(std::invalid_argument)
{
	if(_width <= 0 || _height <= 0)
		throw std::invalid_argument("HPACluster::HPACluster: cluster height and width cannot be <= 0");
	if(x < 0 || y < 0)
		throw std::invalid_argument("HPACluster::HPACluster: cluster (x,y) coordinates must be >= 0");
	if(_alg == NULL)
		throw std::invalid_argument("HPACluster::HPACluster: search algorithm parameter cannot be null");

	startx = x;
	starty = y;
	width = _width;
	height = _height;
	alg = _alg;
	verbose = false;
	allowDiagonals = false;
}

HPACluster::~HPACluster()
{
	delete alg;
}

// add all traversable nodes in the cluster area to the cluster 
void HPACluster::buildCluster() 
{
	for(int x=this->getHOrigin(); x<getHOrigin()+getWidth(); x++)
		for(int y=this->getVOrigin(); y<getVOrigin()+getHeight(); y++)
		{	
			node* n = map->getNodeFromMap(x,y);
			if(n)
				addNode(map->getNodeFromMap(x,y));
		}
}

// Performs a few extra checks before assigning a node to the cluster.
// First: the node must be within the bounds of the cluster
// Second: the node cannot be assigned to another cluster
void HPACluster::addNode(node* _mynode) throw(std::invalid_argument)
{
	ClusterNode* mynode = dynamic_cast<ClusterNode*>(_mynode);
	if(mynode == NULL)
		throw std::invalid_argument(
				"HPACluster::addNode arg is not of type ClusterNode");

	if(nodes[mynode->getUniqueID()] != NULL) // already added
		return;

	int nx = mynode->getLabelL(kFirstData);
	int ny = mynode->getLabelL(kFirstData+1);
	
	if(nx < this->getHOrigin() || nx >= this->getHOrigin() + this->getWidth())
	{
		std::ostringstream oss;
		oss << "HPACluster::addNode arg outside cluster boundary."<<std::endl;
		mynode->Print(oss);
		oss << std::endl;
		this->print(oss);
		oss << std::endl;
		throw std::invalid_argument(oss.str().c_str());
	}
	
	if(mynode->getParentClusterId() != -1)
	{
		std::ostringstream oss;
		oss << "HPACluster::addNode arg already assigned to another cluster.";
		mynode->Print(oss);
		oss << std::endl;
		print(oss);
		oss << std::endl;
		throw std::invalid_argument(oss.str().c_str());
	}
			
	Cluster::addNode(mynode);
}

// Connects a new parent node with every other other parent node in the cluster by using A*. 
// Each successful search results in a new edge being added to the abstract graph.
void HPACluster::connectParent(node* absStart, HPAClusterAbstraction* map)
{	
	for(nodeTable::iterator nodeIter = parents.begin(); nodeIter != parents.end(); nodeIter++)
	{
		node* absGoal = (*nodeIter).second;
		graph* absg = map->getAbstractGraph(1);
		
		alg->setCorridorNodes(&nodes);

		// get low-level nodes
		node* from = map->getNodeFromMap(absStart->getLabelL(kFirstData),absStart->getLabelL(kFirstData+1)); 
		node* to = map->getNodeFromMap(absGoal->getLabelL(kFirstData),absGoal->getLabelL(kFirstData+1)); 

		path* solution = alg->getPath(map, from, to);
		if(solution != 0)
		{
			double dist = map->distance(solution);
			edge* e = new edge(absStart->getNum(), absGoal->getNum(), dist);
			absg->addEdge(e);
			map->addPathToCache(e, solution);				
		}

		/* record some metrics about the operation */
		map->setNodesExpanded(map->getNodesExpanded() + alg->getNodesExpanded());
		map->setNodesTouched(map->getNodesTouched() + alg->getNodesTouched());
		map->setNodesGenerated(map->getNodesGenerated() + alg->getNodesGenerated());
		map->setSearchTime(map->getSearchTime() + alg->getSearchTime());
	}
}

void HPACluster::buildEntrances(HPAClusterAbstraction* map) throw(std::invalid_argument)
{
	if(map == NULL)
		throw std::invalid_argument("HPACluster::buildEntrances: HPAClusterAbstraction parameter cannot be null");
	
	if(getVerbose())
	{
		std::cout << "buildEntrances; ";
		print(std::cout);
		std::cout << std::endl;
	}

	buildHorizontalEntrances(map);
	buildVerticalEntrances(map);
	if(allowDiagonals)
		buildDiagonalEntrances(map);
}

// Each cluster only considers veritcal entrances along the length of its eastern border. 
// A naive method would duplicate the creation of some entrances 
void HPACluster::buildVerticalEntrances(HPAClusterAbstraction* map)
{	
	int mapwidth = map->getMap()->getMapWidth();
	int x = this->getHOrigin()+this->getWidth();
	if(x == mapwidth)
		return; 

	// scan for vertical entrances along the eastern border 
	int y = this->getVOrigin();
	while(y < this->getVOrigin()+this->getHeight())
	{
		int length = findVerticalEntranceLength(x,y, map);
	
		// build transition points; long entrances have 2, short entrances have 1.
		if(length == 0)
			y++;
		else
		{
			processVerticalEntrance(map, x, y, length);
			y += length;
		}
	}
}

void 
HPACluster::processVerticalEntrance(int x, int y, int length)
{
	if(length >= MAX_SINGLE_TRANSITION_ENTRANCE_SIZE) 
	{
		// place one transition point at each end of the entrance area
		// NB: (x,y) is inside eastern neighbour
		node* endpoint1 = map->getNodeFromMap(x, y); 
		node* endpoint2 = map->getNodeFromMap(x-1, y);
		this->addTransition(endpoint1, endpoint2);

		endpoint1 = map->getNodeFromMap(x, y+length-1); 
		endpoint2 = map->getNodeFromMap(x-1, y+length-1);
		this->addTransition(endpoint1, endpoint2);			
	}
	else
	{
		// place a transition point in the middle of the entrance area 
		int ty = y + (length/2);
		int tx = x;
		node* endpoint1 = map->getNodeFromMap(tx, ty); 
		node* endpoint2 = map->getNodeFromMap(tx-1, ty);
		this->addTransition(endpoint1, endpoint2);
	}
}

int 
HPACluster::findVerticalEntranceLength(int x, int y)
{
	int length = 0;
	while(y < this->getVOrigin()+this->getHeight())
	{
		if(map->getNodeFromMap(x, y) == NULL || map->getNodeFromMap(x-1, y) == NULL)
			break;
		y++;
		length++;
	}
	
	return length;
}

void 
HPACluster::buildHorizontalEntrances()
{
	int mapheight = map->getMap()->getMapHeight();
	int y = this->getVOrigin()+this->getHeight();
	if(y == mapheight)
		return; 

	// scan for vertical entrances along the eastern border 
	int x = this->getHOrigin();
	while(x < this->getHOrigin()+this->getWidth())
	{
		int length = findHorizontalEntranceLength(x,y, map);
	
		// build transition points; long entrances have 2, others have 1.
		if(length == 0)
			x++;
		else
		{
			processHorizontalEntrance(map, x, y, length);
			x += length;
		}
	}
}

void 
HPACluster::processHorizontalEntrance(int x, int y, int length)
{
			if(length >= MAX_SINGLE_TRANSITION_ENTRANCE_SIZE) 
			{
				// place one transition point at each end of the entrance area
				// NB: (x,y) is inside eastern neighbour
				node* endpoint1 = map->getNodeFromMap(x, y); 
				node* endpoint2 = map->getNodeFromMap(x, y-1);
				this->addTransition(endpoint1, endpoint2);

				endpoint1 = map->getNodeFromMap(x+length-1, y); 
				endpoint2 = map->getNodeFromMap(x+length-1, y-1);
				this->addTransition(endpoint1, endpoint2);			
			}
			else
			{
				// place a transition point in the middle of the entrance area 
				int tx = x + (length/2);
				int ty = y;
				node* endpoint1 = map->getNodeFromMap(tx, ty); 
				node* endpoint2 = map->getNodeFromMap(tx, ty-1);
				this->addTransition(endpoint1, endpoint2);
			}
}

int 
HPACluster::findHorizontalEntranceLength(int x, int y)
{
	int length = 0;
	while(x < this->getHOrigin()+this->getWidth())
	{
		if(map->getNodeFromMap(x, y) == NULL || map->getNodeFromMap(x, y-1) == NULL)
			break;
		x++;
		length++;
	}
	
	return length;
}

// look for entrances between diagonally adjacent clusters
void 
HPACluster::buildDiagonalEntrances()
{
	if(getVerbose())
		std::cout << "buildDiagonalEntrances"<<std::endl;

	int y = this->getVOrigin();
	int x = this->getHOrigin();

	// look for diagonal entrances in the top-left corner of the cluster
	node* endpoint1 = map->getNodeFromMap(x, y);
	node* endpoint2 = map->getNodeFromMap(x-1, y-1);
	if(endpoint1 && endpoint2)
	{
		addTransition(endpoint1, endpoint2);
	}

	// look for diagonal entrances in the top-right corner of the cluster
	x = this->getHOrigin() + this->getWidth()-1;
	endpoint1 = map->getNodeFromMap(x, y);
	endpoint2 = map->getNodeFromMap(x+1, y-1);
	if(endpoint1 && endpoint2)
	{
		addTransition(endpoint1, endpoint2);
	}

	// look for diagonal entrances in the bottom-right corner of the cluster
	y = this->getVOrigin() + this->getHeight() - 1;
	endpoint1 = map->getNodeFromMap(x,y);
	endpoint2 = map->getNodeFromMap(x+1, y+1);
	if(endpoint1 && endpoint2)
	{
		addTransition(endpoint1, endpoint2);
	}

	// look for diagonal entrances in the bottom-left corner of the cluster
	x = this->getHOrigin();
	endpoint1 = map->getNodeFromMap(x,y);
	endpoint2 = map->getNodeFromMap(x-1, y+1);
	if(endpoint1 && endpoint2)
	{
		addTransition(endpoint1, endpoint2, map);
	}
}

void
HPACluster::print(std::ostream& out)
{
	Cluster::print(out);
	out << std::endl;
	out << "Origin: ("<<getHOrigin()<<", "<<getVOrigin()<<") ";
	out << "height: "<<getHeight()<<" width: "<<getWidth();
}

