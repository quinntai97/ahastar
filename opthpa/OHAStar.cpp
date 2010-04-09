#include "OHAStar.h"
#include "MacroNode.h"
#include "map.h"
#include "MacroNode.h"

OHAStar::OHAStar()
{
	cardinal = false;
}

OHAStar::~OHAStar()
{
}

path* OHAStar::getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp)
{
	MacroNode* mfrom = dynamic_cast<MacroNode*>(from);
	MacroNode* mto = dynamic_cast<MacroNode*>(to);

	if(!mfrom || !mto)
	{
		std::cout << "OHA* failed; start or goal not of type MacroNode";
		std::cout << "start type: "<<typeid(from).name()<<" goal type: ";
		std::cout << typeid(to).name() <<std::endl;
		return 0;
	}

	// abstract or low-level search? this decision affects the behaviour of ::relaxEdge
	if(mfrom->getLabelL(kAbstractionLevel) == 1)
	{
		mfrom->setMacroParent(mfrom);
	}
	else
		mfrom->setMacroParent(0);

	return ClusterAStar::getPath(aMap, from, to, rp);	
}

// if ::cardinal == true, edges with non-integer costs will not be evaluated during search.
// i.e. we pretend the graph has no diagonal edges (characteristic of a 4-connected graph).
bool OHAStar::evaluate(node* current, node* target, edge* e)
{
	bool retVal = ClusterAStar::evaluate(current, target, e);
	if(cardinal)
	{
		if(e->getWeight() != (int)e->getWeight())
			retVal = false;
		if(e->getWeight() > 2 && !retVal)
			std::cerr << "found a macro edge with non-integer costs! wtf?";
	}
	return retVal;
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
	assert(from && to);

	double f_to = DBL_MAX; 
	MacroNode* mp = from->getMacroParent(); // macro parent 

	// if 'from' has a macro parent, we relax the edge with respect to its macro parent
	// otherwise, standard A* relaxation is used
	if(mp == 0) 
	{
		double g_from = from->getLabelF(kTemporaryLabel) - h(from, goal);
		f_to = g_from + e->getWeight() + h(to, goal);
	}
	else
	{
		if(from->getParentClusterId() == to->getParentClusterId())
		{
			double g_mp = mp->getLabelF(kTemporaryLabel) - h(mp, goal); 
			f_to = g_mp + h(mp, to)  + h(to, goal); // NB: h(mp, to) is exact
		}
		else
		{
			mp = to;
			double g_from = from->getLabelF(kTemporaryLabel) - h(from, goal);
			f_to = g_from + e->getWeight() + h(to, goal);
		}
	}

	// update priority and macro parent if necessary 
	if(f_to < to->getLabelL(kTemporaryLabel))
	{
		if(verbose)
		{
			std::cout << " relaxing "<<to->getName()<<" from: "<<from->getName()<< " f(to): "<<f_to;
			if(!mp)
				std::cout << " no mp!"<<std::endl;
			else
			{
				double g_mp = mp->getLabelF(kTemporaryLabel) - h(mp, goal); 
				std::cout << " fmp: "<<mp->getLabelF(kTemporaryLabel)<<" hmp: "<<h(mp, goal);
				std::cout << " gmp: "<<g_mp<< " h(to, goal): "<<h(to, goal);
				std::cout << " mp(to) = "<<mp->getName()<<std::endl;
			}
		}

		to->setLabelF(kTemporaryLabel, f_to);
		openList->decreaseKey(to);
		to->markEdge(e);
		to->setMacroParent(mp);
	}
	else
		if(verbose)
			std::cout << " not relaxing!"<<std::endl;
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
		p = new path(cn, p);
		if(verbose)
			std::cout << current <<"cn: "<<cn->getName();

		MacroNode* mp = cn->getMacroParent();
		if(mp)
		{
			if(cn->getNum() != mp->getNum())
			{
				current = mp->getNum();
				if(verbose)
					std::cout << " following macro parent "<<mp->getName()<<std::endl;
			}
			else
			{
				e = cn->getMarkedEdge();
				if(e != 0)
				{
					current = e->getFrom() == current?e->getTo():e->getFrom();
					if(verbose)
						std::cout << " following marked edge"<<std::endl;
				}
				else
				{
					// cn must be the start node (cannot follow macro parent or marked edge)
					if(verbose)
						std::cout << " is start! finished!"<<std::endl; 
					break;
				}
			}
		}
		else
		{
				// special case for searching on the non-abstracted graph
				// where none of the nodes have macro parents
				e = cn->getMarkedEdge();
				if(e == 0)
					break;
				current = e->getFrom() == current?e->getTo():e->getFrom();

				if(verbose)
					std::cout << current <<" <- ";
		}
	}

	return p;
}


