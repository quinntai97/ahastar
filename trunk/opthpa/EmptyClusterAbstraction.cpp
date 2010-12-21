#include "EmptyClusterAbstraction.h"

#include "ClusterNode.h"
#include "EmptyCluster.h"
#include "IEdgeFactory.h"
#include "IClusterFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "heap.h"
#include "map.h"
#include "MacroEdge.h"
#include "MacroNode.h"

EmptyClusterAbstraction::EmptyClusterAbstraction(Map* m, IClusterFactory* cf, 
	INodeFactory* nf, IEdgeFactory* ef, bool allowDiagonals, bool perimeterReduction_,
	bool bfReduction_) 
	throw(std::invalid_argument)
	: GenericClusterAbstraction(m, cf, nf, ef, allowDiagonals),
	  perimeterReduction(perimeterReduction_), bfReduction(bfReduction_)
{
	AbstractCluster* tmp = cf->createCluster(0, 0, this);
	if(!dynamic_cast<EmptyCluster*>(tmp))
		throw std::invalid_argument("EmptyClusterAbstraction requires"
				"a cluster factory that produces objects derived from"
				" EmptyCluster");
				
	sgEdge = 0;
}

EmptyClusterAbstraction::~EmptyClusterAbstraction()
{
	assert(sgEdge == 0);
}

int EmptyClusterAbstraction::getNumMacro()
{
	int macro = 0;
	cluster_iterator ci = getClusterIter();
	EmptyCluster* cluster = clusterIterNext(ci);
	while(cluster)
	{
		macro += cluster->macro;
		cluster = clusterIterNext(ci);
	}
	return macro;
}

void EmptyClusterAbstraction::buildClusters()
{
	if(getVerbose())
		std::cout << "buildClusters...."<<std::endl;

	Map* m = this->getMap();
	int mapheight = m->getMapHeight();
	int mapwidth = m->getMapWidth();

	// set initial priorities of all potential cluster origins
	// based on # of interior nodes in maximal clearance square of each tile
	heap clusterseeds(30, false);
	for(int y=0; y<mapheight; y++)
		for(int x=0; x<mapwidth; x++)
		{
			node* n = this->getNodeFromMap(x, y);
			if(n)
			{
				EmptyCluster* cluster = dynamic_cast<EmptyCluster*>(
						getClusterFactory()->createCluster(x, y, this)); 
				cluster->setPerimeterReduction(perimeterReduction);
				cluster->setBFReduction(bfReduction);
				cluster->buildCluster();
				cluster->setVerbose(getVerbose());

				double ratio = cluster->getNumNodes() / cluster->getNumParents();
				n->setLabelF(kTemporaryLabel, ratio);
				n->setKeyLabel(kTemporaryLabel);
				clusterseeds.add(n);
				delete cluster;
			}
		}

	// start making clusters; prefer clusters with more interior nodes to others
	// with less
	while(!clusterseeds.empty())
	{
		ClusterNode* cur = dynamic_cast<ClusterNode*>(clusterseeds.peek());

		if(cur->getParentClusterId() == -1)
		{
			EmptyCluster* cluster = dynamic_cast<EmptyCluster*>(
					getClusterFactory()->createCluster(
						cur->getLabelL(kFirstData), 
						cur->getLabelL(kFirstData+1), 
						this)); 
			cluster->setPerimeterReduction(perimeterReduction);
			cluster->setBFReduction(bfReduction);
			cluster->buildCluster();
			cluster->setVerbose(getVerbose());

			double ratio = cluster->getNumNodes() / cluster->getNumParents();
			if(ratio >= cur->getLabelF(kTemporaryLabel))
			{
				addCluster(cluster);
				if(this->getVerbose())
				{
					std::cout << "new cluster w/ priority "<<ratio<<":\n";
				    cluster->print(std::cout);	
				}
				clusterseeds.remove();
				cluster = 0;
			}
			else
			{
				cur->setLabelF(kTemporaryLabel, ratio); 
				clusterseeds.decreaseKey(cur);
				delete cluster;
			}
		}
		else
		{
			clusterseeds.remove();
		}
		
	}
}

// each start/goal node is connected to the nearest abstract node along each border
// of the cluster. i.e. it has 4 neighbours (one above, below, to the left and right).
void
EmptyClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(node* s, node* g)
	throw(std::invalid_argument)
{
	if(s->getLabelL(kParent) == -1)
	{
		ClusterNode* start = dynamic_cast<ClusterNode*>(s);
		AbstractCluster* cluster = this->getCluster(
			start->getParentClusterId());

		if(getVerbose())
		{
			const int x = start->getLabelL(kFirstData);
			const int y = start->getLabelL(kFirstData+1);
			std::cout << "inserting start node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;
		}

		cluster->addParent(start);
		startid = start->getLabelL(kParent);
		assert(startid != -1);
	}

	if(g->getLabelL(kParent) == -1)
	{
		ClusterNode* goal = dynamic_cast<ClusterNode*>(g);
		AbstractCluster* cluster = this->getCluster(
			goal->getParentClusterId());

		if(getVerbose())
		{
			const int x = goal->getLabelL(kFirstData);
			const int y = goal->getLabelL(kFirstData+1);
			std::cout << "inserting goal node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;
		}

		cluster->addParent(goal);
		goalid = goal->getLabelL(kParent);
		assert(goalid != -1);
	}
}

void 
EmptyClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph() 
	throw(std::runtime_error)
{
	graph* absg = this->getAbstractGraph(1);
	if(startid != -1)
	{
		ClusterNode* start = dynamic_cast<ClusterNode*>(
				absg->getNode(startid));
		AbstractCluster* cluster = this->getCluster(
				start->getParentClusterId());

		if(getVerbose())
			std::cout << "removing inserted start node"<<std::endl;

		cluster->removeParent(startid);
		startid = -1;
		delete start;
	}

	if(goalid != -1)
	{
		ClusterNode* goal = dynamic_cast<ClusterNode*>(
				absg->getNode(goalid));
		AbstractCluster* cluster = this->getCluster(
				goal->getParentClusterId());

		if(getVerbose())
			std::cout << "removing inserted start node"<<std::endl;

		cluster->removeParent(goalid);
		goalid = -1;
		delete goal;
	}
}

EmptyCluster* EmptyClusterAbstraction::clusterIterNext(cluster_iterator& it) const
{
       return static_cast<EmptyCluster*>(
		GenericClusterAbstraction::clusterIterNext(it));
}

EmptyCluster* EmptyClusterAbstraction::getCluster(int cid)
{
       return static_cast<EmptyCluster*>(
		GenericClusterAbstraction::getCluster(cid));
}

double EmptyClusterAbstraction::getAverageClusterSize()
{
	double total = 0;
	cluster_iterator iter = this->getClusterIter();
	EmptyCluster* cluster = 0;
	while((cluster = this->clusterIterNext(iter)))
	{
		total += cluster->getNumNodes();
	}

	return total/this->getNumClusters();
}

double EmptyClusterAbstraction::getAverageNodesPruned()
{
	double total = 0;
	cluster_iterator iter = this->getClusterIter();
	EmptyCluster* cluster = 0;
	while((cluster = this->clusterIterNext(iter)))
	{
		total += cluster->getNumNodes() - cluster->getNumParents();
	}

	return total/this->getNumClusters();
	
}

int EmptyClusterAbstraction::getNumAbsEdges()
{
	graph* g = this->getAbstractGraph(1);
	int numEdges = g->getNumEdges();

	cluster_iterator iter = this->getClusterIter();
	EmptyCluster* cluster = 0;
	while((cluster = this->clusterIterNext(iter)))
	{
		numEdges += cluster->getNumSecondaryEdges();
	}
	
	return numEdges;
}
