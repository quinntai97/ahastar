#include "JPAExpansionPolicy.h"

#include "graph.h"
#include "Heuristic.h"
#include "mapAbstraction.h"
#include "ProblemInstance.h"

JPAExpansionPolicy::JPAExpansionPolicy()
	: ExpansionPolicy()
{
	neighbourIndex = 0;
}


JPAExpansionPolicy::~JPAExpansionPolicy()
{
}


node* 
JPAExpansionPolicy::first()
{
	neighbourIndex = 0;
	return n();
}

node* 
JPAExpansionPolicy::next()
{
	node* nextnode = 0;

	if(hasNext())
	{
		neighbourIndex++;
		nextnode = n();
	}

	return nextnode;
}

// Returns the current neighbour of the target node being expanded.
// Usually the neighbour is a node incident with the target -- however
// if the edge (target, neighbour) is a transition which involves jumping
// over the row or column of the goal node we return instead the node
// on the row or column of the goal node. 
//
// This procedure is necssary to ensure we do not miss the goal node
// while searching on a map of type JumpPointAbstraction.
node*
JPAExpansionPolicy::n()
{
	edge_iterator iter = target->getOutgoingEdgeIter() + neighbourIndex;
	edge* e = target->edgeIterNextOutgoing(iter);

	node* n = problem->getMap()->getAbstractGraph(0)->getNode(e->getTo());

	int tx = target->getLabelL(kFirstData);
	int ty = target->getLabelL(kFirstData+1);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	int gx = problem->getGoalNode()->getLabelL(kFirstData);
	int gy = problem->getGoalNode()->getLabelL(kFirstData+1);

	if(((tx < gx <= nx) || (tx > gx >= nx)) && 
			((ty < gy <= ny) || (ty > gy >= ny)))
	{
		// the edge (target, n) crosses both row and column of the goal node
		int deltay = abs(gy - ty);
		int deltax = abs(gx - tx);

		if(deltax < deltay)
		{
			nx = gx;
			ny = (ny > ty)?(ty + deltax):(ty - deltax);
		}
		else
		{
			ny = gy;
			nx = (nx > tx)?(tx + deltay):(tx - deltay);
		}
	}
	else if((tx < gx <= nx) || (tx > gx >= nx))
	{
		// the edge (target, n) crosses only the row of the goal node
		ny = gy;
		int deltay = abs(gy - ty);
		nx = (nx > tx)?(tx + deltay):(tx - deltay);
	}
	else if((ty < gy <= ny) || (ty > gy >= ny))
	{
		// the edge (target, n) crosses only the column of the goal node
		int deltax = abs(gx - tx);
		nx = gx;
		ny = (ny > ty)?(ty + deltax):(ty - deltax);
	}

	n = problem->getMap()->getNodeFromMap(nx, ny);
	return n;
}

double
JPAExpansionPolicy::cost_to_n()
{
	node* current = n();
	return problem->getHeuristic()->h(target, current);
}

bool
JPAExpansionPolicy::hasNext()
{
	if(neighbourIndex+1 < (unsigned int)target->getNumOutgoingEdges())
		return true;
	return false;
}

