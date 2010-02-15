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

EmptyCluster* EmptyClusterAbstraction::clusterIterNext(cluster_iterator& it) const
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::clusterIterNext(it));
}

EmptyCluster* EmptyClusterAbstraction::getCluster(int cid)
{
	return dynamic_cast<EmptyCluster*>(HPAClusterAbstraction::getCluster(cid));
}
