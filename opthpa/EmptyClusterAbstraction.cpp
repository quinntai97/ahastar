#include "EmptyClusterAbstraction.h"
#include "IEdgeFactory.h"
#include "IHPAClusterFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "map.h"

EmptyClusterAbstraction::EmptyClusterAbstraction(Map* m, IHPAClusterFactory* cf, 
	INodeFactory* nf, IEdgeFactory* ef, unsigned int clustersize) 
	throw(std::invalid_argument)
	: HPAClusterAbstraction(m, cf, nf, ef, clustersize) 
{

}

EmptyClusterAbstraction::~EmptyClusterAbstraction()
{

}

void EmptyClusterAbstraction::buildClusters()
{
	Map* m = this->getMap();
	int mapheight = m->getMapHeight();
	int mapwidth = m->getMapWidth();
	for(int x=0; x<mapwidth; x++)
		for(int y=0; y<mapheight; y++)
		{
			node* cur = this->getNodeFromMap(x, y);
			if(cur->getParentClusterId() == -1)
			{
				EmptyCluster* cluster = new EmptyCluster(x, y);	
				addCluster(cluster);
				cluster->extend();
			}	
		}
}

// TODO: every node in the entrance appears in the abstract graph
// avoid adding intra-edges since we can calculate these on-demand.
void EmptyClusterAbstraction::buildEntrances()
{
}

void
EmptyClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(node* s, node* g)
	throw(std::invalid_argument)
{
}

void
EmptyClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph()
	throw(std::runtime_error)
{
}
