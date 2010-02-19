#include "EmptyClusterAbstraction.h"

#include "ClusterNode.h"
#include "EmptyCluster.h"
#include "IEdgeFactory.h"
#include "IHPAClusterFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "map.h"

EmptyClusterAbstraction::EmptyClusterAbstraction(Map* m, IHPAClusterFactory* cf, 
	INodeFactory* nf, IEdgeFactory* ef) 
	throw(std::invalid_argument)
	: HPAClusterAbstraction(m, cf, nf, ef) 
{

}

EmptyClusterAbstraction::~EmptyClusterAbstraction()
{
}

// Decomposes the map into a set of empty (obstacle free) clusters.
// simple flood-fill based decomposition:
//  1. Start with some node @ (x, y). 
//  2. Determine the length of a maximal row by extending horizontally 
//  until an obstacle is encountered or a node assigned to another cluster 
//  is encountered. 
//  3. Once the maximal row size is established, try to extend the cluster vertically
//  by building rows of the same length. 
//  4. Stop when a full-size row cannot be built due to obstacles or nodes already 
//  assigned to another cluster.
void EmptyClusterAbstraction::buildClusters()
{
	Map* m = this->getMap();
	int mapheight = m->getMapHeight();
	int mapwidth = m->getMapWidth();

	int** clearance;
    clearance = new int*[mapwidth];
	for(int i = 0; i<mapwidth; i++)
		clearance[i] = new int[mapheight];

	for(int y=0; y<mapheight; y++)
		for(int x=0; x<mapwidth; x++)
			clearance[x][y] = 0;


	computeClearance(clearance);
	for(int y=0; y<mapheight; y++)
		for(int x=0; x<mapwidth; x++)
		{
			ClusterNode* cur = dynamic_cast<ClusterNode*>(this->getNodeFromMap(x, y));
			if(cur && cur->getParentClusterId() == -1)
			{
				EmptyCluster* cluster = new EmptyCluster(x, y);	
				cluster->setVerbose(getVerbose());
				addCluster(cluster);
				cluster->addNodesToCluster(this, clearance);
				if(this->getVerbose())
					std::cout << "new cluster @ ("<<x<<","<<y<<") with "
						" id: "<< cluster->getId()<< std::endl;
			}	
		}

	//std::cout << "clearance values for map: "<<m->getMapName()<<std::endl;
//	for(int y=0; y<mapheight; y++)
//	{
//		for(int x=0; x<mapwidth; x++)
//			std::cout << clearance[x][y] << " ";
//		std::cout << std::endl;
//	}

	for(int i=0; i<mapwidth; i++)
		delete clearance[i];
	delete[] clearance;
}

void EmptyClusterAbstraction::computeClearance(int** clearance)
{
	Map* m = this->getMap();
	int mapheight = m->getMapHeight();
	int mapwidth = m->getMapWidth();
	for(int x=getMap()->getMapWidth()-1;x>=0; x--)
	{
		for(int y=getMap()->getMapHeight()-1;y>=0; y--)
		{
			node* n = getNodeFromMap(x,y);
			if(n) 
			{	
				int x = n->getLabelL(kFirstData);
				int y = n->getLabelL(kFirstData+1);
				node *adj1, *adj2, *adj3; // adjacent neighbours
				adj1 = getNodeFromMap(x+1, y+1);
				adj2 = getNodeFromMap(x+1,y);
				adj3 = getNodeFromMap(x,y+1);
				
				if(adj1 && adj2 && adj3)
				{	
					int min = clearance[x+1][y+1]; 
					int tmp1 = clearance[x+1][y]; 
					int tmp2 = clearance[x][y+1]; 
					min = tmp1<min?tmp1:min;
					min = tmp2<min?tmp2:min;
					clearance[x][y] = min+1; 
				}
				else // tile is on a border or perimeter boundary. clearance = 1
				{
					clearance[x][y] = 1;
				}
			}
			else
			{
				clearance[x][y] = 0;
			}
		}
	}
}


// each start/goal node is connected to the nearest abstract node along each border
// of the cluster. i.e. it has 4 neighbours (one above, below, to the left and right).
void
EmptyClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(node* s, node* g)
	throw(std::invalid_argument)
{
	HPAClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(s, g);

	graph* absg = this->getAbstractGraph(1);
	ClusterNode* absStart = dynamic_cast<ClusterNode*>(absg->getNode(s->getLabelL(kParent)));
	ClusterNode* absGoal = dynamic_cast<ClusterNode*>(absg->getNode(g->getLabelL(kParent)));

	if(absStart == 0 || absGoal == 0)
	{
		throw std::invalid_argument("either start or goal node not inserted into abstract graph");
	}

	// only connect nodes that do not already exist in the abstract graph 
	if(this->getStartId() != -1)
		connectSG(absStart);
	if(this->getGoalId() != -1)
		connectSG(absGoal);
}

void EmptyClusterAbstraction::connectSG(node* absNode)
{
	graph* absg = this->getAbstractGraph(1);
	EmptyCluster* nodeCluster = this->getCluster(dynamic_cast<ClusterNode*>(absNode)->getParentClusterId());

	int x = absNode->getLabelL(kFirstData);
	int y = absNode->getLabelL(kFirstData+1);
	if(getVerbose())
		std::cout << "inserting node ("<<x<<", "<<y<<") into abstract graph"<<std::endl;

	// connect to neighbour above
	int ny = nodeCluster->getVOrigin();
	int nx = x;
	node* absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along top border");

	edge* e = new edge(absNode->getNum(), absNeighbour->getNum(), h(absNode, absNeighbour));
	absg->addEdge(e);
	if(getVerbose())
		std::cout << "above: ("<<nx<<", "<<ny<<") weight: "<<e->getWeight();


	// connect to neighbour below
	ny = nodeCluster->getVOrigin()+nodeCluster->getHeight()-1;
	nx = x;
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along bottom border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), h(absNode, absNeighbour));
	absg->addEdge(e);
	if(getVerbose())
		std::cout << "below: ("<<nx<<", "<<ny<<") weight: "<<e->getWeight();


	// connect to neighbour to the left
	ny = y; 
	nx = nodeCluster->getHOrigin();
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along left border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), h(absNode, absNeighbour));
	absg->addEdge(e);
	if(getVerbose())
		std::cout << "left: ("<<nx<<", "<<ny<<") weight: "<<e->getWeight();


	// connect to neighbour to the right
	ny = y; 
	nx = nodeCluster->getHOrigin()+nodeCluster->getWidth()-1;
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along right border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), h(absNode, absNeighbour));
	absg->addEdge(e);
	if(getVerbose())
		std::cout << "right ("<<nx<<", "<<ny<<") weight: "<<e->getWeight() <<std::endl;

}

// h heuristic
double EmptyClusterAbstraction::h(node* from, node* to)
{
	int fx = from->getLabelL(kFirstData);
	int fy = from->getLabelL(kFirstData+1);
	int tx = to->getLabelL(kFirstData);
	int ty = to->getLabelL(kFirstData+1);
//	std::cout << "from: "<<fx<<","<<fy<<") to: ("<<tx<<","<<ty<<") ";

	int deltax = fx - tx;
	if(deltax < 0) deltax *=-1;

	int deltay = fy - ty;
	if(deltay < 0) deltay *=-1;

//	std::cout << "deltax: "<<deltax<<" deltay: "<<deltay<<std::endl;
	return deltax + deltay;
}

EmptyCluster* EmptyClusterAbstraction::clusterIterNext(cluster_iterator& it) const
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::clusterIterNext(it));
}

EmptyCluster* EmptyClusterAbstraction::getCluster(int cid)
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::getCluster(cid));
}
