#include "PerimeterSearch.h"

#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "MacroNode.h"
#include "MacroEdge.h"
#include <string>

PerimeterSearch::PerimeterSearch()
{
}

PerimeterSearch::~PerimeterSearch()
{

}

path* PerimeterSearch::getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp)
{
	visitedClusters.clear();
	return ClusterAStar::getPath(aMap, from, to, rp);
}

void PerimeterSearch::expand(node* current_, node* goal, edge_iterator begin, 
		unsigned int card, heap* openList, std::map<int, node*>& closedList, graph* g)
{
	if(verbose)
		std::cout << "PerimeterSearch expand..."<<std::endl;

	MacroNode* current = static_cast<MacroNode*>(
			current_);
	assert(current);

	AbstractClusterAStar::expand(current, goal, current->getEdgeIter(), 
			current->getNumEdges(), openList, closedList, g);

	
	// process any neighbours connected via secondary edges
	if(current->numSecondaryEdges() > 0)
	{
		if(expandSecondary(current))
		{
			if(verbose)
				std::cout << "processing secondary edges; ";
			AbstractClusterAStar::expand(current, goal, current->secondaryEdgeIter(), 
					current->numSecondaryEdges(), openList, closedList, g);
			nodesExpanded--; // no double counting
		}
		else
		{
			if(verbose)
				std::cout << "skipping secondary edges"<<std::endl;
		}
	}
	else
	{
		if(verbose)
			std::cout << "no secondary edges; moving on"<<std::endl;
	}
}

bool PerimeterSearch::expandSecondary(MacroNode* current)
{
	bool retVal = true;
	EmptyClusterAbstraction* aMap = dynamic_cast<EmptyClusterAbstraction*>(this->getGraphAbstraction());
	EmptyCluster* parentCluster = aMap->getCluster(current->getParentClusterId());


	if(visitedClusters.find(parentCluster->getId()) != visitedClusters.end())
	{
		EmptyCluster::RoomSide side = parentCluster->whichSide(current);
		node* best = parentCluster->getBestExpandedNode(side);

		if(best)
		{
			double g_cur = current->getLabelF(kTemporaryLabel) - h(current, best);
			double g_best = best->getLabelF(kTemporaryLabel) - h(current, best);

			double g_delta = g_cur - g_best;
			if(g_delta < 0)
				g_delta*=-1;

			double dist = h(current, best)*(ROOT_TWO-1);
			if(g_delta >= dist)
			{
				retVal = false;
			}
			else
			{
				if(g_cur < g_best)
				{
					parentCluster->setBestExpandedNode(current);
				}
			}
		}
		else
		{
			parentCluster->setBestExpandedNode(current);
		}
	}
	else
	{
		visitedClusters[parentCluster->getId()] = true;
		parentCluster->setBestExpandedNode(current);
	}

	return true;
}

