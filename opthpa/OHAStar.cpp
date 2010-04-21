#include "OHAStar.h"

#include "graph.h"
#include "MacroNode.h"
#include "mapAbstraction.h"
#include "map.h"
#include "MacroNode.h"

OHAStar::OHAStar()
{
	cardinal = false;
}

OHAStar::~OHAStar()
{
}

// TODO: if s+g are low level, get their abstract parents and search on those
// fail otherwise?.
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
	if(mfrom->getLabelL(kAbstractionLevel) == 0)
	{
		graph* g = aMap->getAbstractGraph(1);
		mfrom = dynamic_cast<MacroNode*>(g->getNode(mfrom->getLabelL(kParent)));
		mto = dynamic_cast<MacroNode*>(g->getNode(mto->getLabelL(kParent)));
		assert(mfrom && mto);
	}

	mfrom->setMacroParent(mfrom);
	path* p = ClusterAStar::getPath(aMap, from, to, rp);	
	return refinePath(p);
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

// refines an abstract path; each abstract path is made up of a sequence of connected
// fragments where each fragment has a start and goal
path* OHAStar::refinePath(path* _abspath)
{
	mapAbstraction* aMap = dynamic_cast<mapAbstraction*>(getGraphAbstraction());
	path* thepath = 0;
	path* tail = 0;
	path* abspath = _abspath;
	
	while(abspath->next)
	{
		node *fs = aMap->getNodeFromMap(abspath->n->getLabelL(kFirstData), 
				abspath->n->getLabelL(kFirstData+1));
		node* fg = aMap->getNodeFromMap(abspath->next->n->getLabelL(kFirstData), 
				abspath->next->n->getLabelL(kFirstData+1));

		path* segment = new path(fs, 0);
		path* segtail = segment;
		while(segtail->n->getNum() != fg->getNum())
		{
			node* n = closestNeighbour(segtail->n, fg);
			segtail->next = new path(n, 0);
			segtail = segtail->next;
		}

		// append segment to refined path
		if(thepath == 0)
		{
			thepath = segment;										
		}
		else
		{
			// successive segments overlap (one finishes where other begins); fix this 
			assert(tail->n->getNum() == segment->n->getNum());
			tail->next = segment->next;
			segment->next = 0;
			delete segment;
		}
		tail = segtail;
		abspath = abspath->next;			
	}

	delete _abspath;
	return thepath;
}

// use ::h to evaluate all neighbours of 'from' with respect to their distance to 'to'
// return the neighbour with the lowest value (i.e. the one closest to 'to')
node* OHAStar::closestNeighbour(node* from, node* to)
{	
	graph* g = getGraphAbstraction()->getAbstractGraph(0);
	double mindist = DBL_MAX;
	node* closest = 0;

	neighbor_iterator niter = from->getNeighborIter();
	int nid = from->nodeNeighborNext(niter);
	while(nid != -1)
	{
		node* n = g->getNode(nid);
		double hdist = h(n, to);
		if(hdist < mindist)
		{
			closest = n;
			mindist = hdist;
		}
		nid = from->nodeNeighborNext(niter);
	}

	return closest;
}
