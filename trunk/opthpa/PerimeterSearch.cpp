#include "PerimeterSearch.h"
#include "MacroNode.h"
#include "MacroEdge.h"
#include <string>

PerimeterSearch::PerimeterSearch()
{
}

PerimeterSearch::~PerimeterSearch()
{

}

void PerimeterSearch::expand(node* current_, node* goal, edge_iterator begin, 
		unsigned int card, heap* openList, std::map<int, node*>& closedList, graph* g)
{
	if(verbose)
		std::cout << "PerimeterSearch expand..."<<std::endl;

	MacroNode* current = dynamic_cast<MacroNode*>(
			current_);
	assert(current);

	AbstractClusterAStar::expand(current, goal, current->getEdgeIter(), 
			current->getNumEdges(), openList, closedList, g);

	
	// if parent of 'current' is in a different cluster, also process
	// neighbours connected via secondary edges
	edge* markedEdge = current->getMarkedEdge();
	if(markedEdge) // only start node has no marked edge (i.e. parent)
	{
		int parentId = markedEdge->getFrom() == current->getNum()?markedEdge->getTo():markedEdge->getFrom();
		MacroNode* parent = dynamic_cast<MacroNode*>(
				g->getNode(parentId));
		assert(parent);

		if(parent->getParentClusterId() != current->getParentClusterId())
		{
			if(verbose)
				std::cout << "processing secondary edges; ";
			AbstractClusterAStar::expand(current, goal, current->secondaryEdgeIter(), 
					current->numSecondaryEdges(), openList, closedList, g);
			nodesExpanded--; // no double counting
		}
	}
	else if(nodesExpanded == 1)
	{
		AbstractClusterAStar::expand(current, goal, current->secondaryEdgeIter(), 
				current->numSecondaryEdges(), openList, closedList, g);
		nodesExpanded--; // no double counting
	}
}

bool PerimeterSearch::evaluate(node* current, node* target, edge* e)
{
	return ClusterAStar::evaluate(current, target, e);
}
