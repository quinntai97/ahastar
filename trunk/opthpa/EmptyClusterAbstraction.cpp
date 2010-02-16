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

void EmptyClusterAbstraction::buildEntrances()
{
	HPAClusterAbstraction::buildEntrances();
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
	for(int x=0; x<mapwidth; x++)
		for(int y=0; y<mapheight; y++)
		{
			ClusterNode* cur = dynamic_cast<ClusterNode*>(this->getNodeFromMap(x, y));
			if(cur && cur->getParentClusterId() == -1)
			{
				EmptyCluster* cluster = new EmptyCluster(x, y);	
				cluster->setVerbose(getVerbose());
				addCluster(cluster);
				cluster->addNodesToCluster(this);
				if(this->getVerbose())
					std::cout << "new cluster @ ("<<x<<","<<y<<") with "
						" id: "<< cluster->getId()<< std::endl;
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

	connectSG(absStart);
	connectSG(absGoal);
}

void EmptyClusterAbstraction::connectSG(node* absNode)
{
	graph* absg = this->getAbstractGraph(1);
	EmptyCluster* nodeCluster = this->getCluster(dynamic_cast<ClusterNode*>(absNode)->getParentClusterId());

	int x = absNode->getLabelL(kFirstData);
	int y = absNode->getLabelL(kFirstData+1);

	// connect to neighbour above
	int ny = nodeCluster->getVOrigin();
	int nx = x;
	node* absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along top border");

	edge* e = new edge(absNode->getNum(), absNeighbour->getNum(), manhattan(absNode, absNeighbour));
	absg->addEdge(e);

	// connect to neighbour below
	ny = nodeCluster->getVOrigin()+nodeCluster->getHeight()-1;
	nx = x;
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along bottom border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), manhattan(absNode, absNeighbour));
	absg->addEdge(e);

	// connect to neighbour to the left
	ny = y; 
	nx = nodeCluster->getHOrigin();
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along left border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), manhattan(absNode, absNeighbour));
	absg->addEdge(e);

	// connect to neighbour to the right
	ny = y; 
	nx = nodeCluster->getHOrigin()+nodeCluster->getWidth()-1;
	absNeighbour = absg->getNode(this->getNodeFromMap(nx, ny)->getLabelL(kParent));
	if(absNeighbour == 0)
		throw std::invalid_argument("cluster not properly framed along right border");

	e = new edge(absNode->getNum(), absNeighbour->getNum(), manhattan(absNode, absNeighbour));
	absg->addEdge(e);
}

// manhattan heuristic
int EmptyClusterAbstraction::manhattan(node* from, node* to)
{
	int fx = from->getLabelL(kFirstData);
	int fy = from->getLabelL(kFirstData+1);
	int tx = to->getLabelL(kFirstData);
	int ty = to->getLabelL(kFirstData+1);

	unsigned int deltax = fx - tx;
	unsigned int deltay = fy - ty;

	return deltax + deltay;
}

void
EmptyClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph()
	throw(std::runtime_error)
{
	HPAClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph();
}

EmptyCluster* EmptyClusterAbstraction::clusterIterNext(cluster_iterator& it) const
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::clusterIterNext(it));
}

EmptyCluster* EmptyClusterAbstraction::getCluster(int cid)
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::getCluster(cid));
}
