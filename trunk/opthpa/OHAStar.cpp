#include "OHAStar.h"
#include "MacroNode.h"
#include "map.h"

OHAStar::OHAStar()
{
}

OHAStar::~OHAStar()
{
}

// TODO: start node is its own macro parent

bool OHAStar::evaluate(node* current, node* target, edge* e)
{
	return false;
}

// relaxEdge keeps track of the shortest path to each node.
// the method is called every time a node is generated.
// See Intro to Algorithms (Cormen et al) for the canonical version.
//
// This version is specifically tailored for pathfinding within empty rooms.
// We make sure each room traversal is optimal by keeping track of the distance
// to a 'macro parent' -- which is the ancestor node that was first expanded in
// the current cluster. See [Harabor & Botea, 2010] for details
//
// @param: openList - a binary heap priority queue
// @param: g - abstract graph
// @param: e - the edge being considered
// @param: fromId - the Id of the node being expanded
// @param: toId - the Id of the node being generated
void OHAStar::relaxEdge(heap *openList, graph *g, edge *e, int fromId, 
		int toId, node *goal)
{
	MacroNode* from = dynamic_cast<MacroNode*>(g->getNode(fromId));
	MacroNode* to = dynamic_cast<MacroNode*>(g->getNode(toId));
	MacroNode* mp = 0; // macro parent 
	double f_to = DBL_MAX; 

	assert(from && to);
	if(from->getParentClusterId() == to->getParentClusterId())
	{
		mp = from->getMacroParent();
		double g_mp = mp->getLabelF(kTemporaryLabel) - h(mp, goal); 
		f_to = g_mp + h(mp, to)  + h(to, goal); // NB: h(mp, to) is exact
	}
	else
	{
		mp = to;
		double g_from = from->getLabelF(kTemporaryLabel) - h(from, goal);
		f_to = g_from + e->getWeight() + h(to, goal);
	}

	// update priority and macro parent if necessary 
	if(f_to < to->getLabelL(kTemporaryLabel))
	{
		to->setLabelF(kTemporaryLabel, f_to);
		openList->decreaseKey(to);
		to->markEdge(e);
		to->setMacroParent(mp);
	}
}

// Extracts the optimal path once the goal is found.
// Unlike the canonical implementation, which simply follows marked edges
// from the goal to the start node, this version alternates between following
// marked edges and macro parents.
// Each macro parent/sibling pair are the endpoints of two nodes on the
// optimal path that enter and exit an empty room.
// Meanwhile, each marked edge is an transition from one empty room to another 
// along the optimal path
//
// @param: g - the search graph
// @param: current - the id of the goal node
path* OHAStar::extractBestPath(graph *g, unsigned int current)
{
	path* p = 0;
	edge* e = 0;

	while(1)
	{
		MacroNode* cn = dynamic_cast<MacroNode*>(g->getNode(current));
		assert(cn);
		MacroNode* mp = cn->getMacroParent();

		if(cn->getNum() != mp->getNum())
		{
			p = new path(cn, p);
			current = mp->getNum();
			if(verbose)
				std::cout << current <<" <- (mp)";

		}
		else
		{
			e = cn->getMarkedEdge();
			if(e == 0)
				break;
			else
			{
				if (e->getFrom() == current)
					current = e->getTo();
				else
					current = e->getFrom();
			}
			p = new path(g->getNode(current), p);
			if(verbose)
				std::cout << current <<" <- ";
		}
	}

	return p;
}

// Equivalent of manhattan heuristic for octile grids
double OHAStar::h(node* a, node*b) throw(std::invalid_argument)
{

	if(a == NULL || b == NULL) 
		throw std::invalid_argument("null node");

	int x1 = a->getLabelL(kFirstData);
	int x2 = b->getLabelL(kFirstData);
	int y1 = a->getLabelL(kFirstData+1);
	int y2 = b->getLabelL(kFirstData+1);
	
	double answer = 0.0;
	const double root2m1 = ROOT_TWO-1;//sqrt(2.0)-1;
		if (fabs(x1-x2) < fabs(y1-y2))
			answer = root2m1*fabs(x1-x2)+fabs(y1-y2);
	else
		answer = root2m1*fabs(y1-y2)+fabs(x1-x2);
	return answer;
}

