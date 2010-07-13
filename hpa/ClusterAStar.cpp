/*
 *  ClusterAStar.cpp
 *  hog
 *
	TODO: Move getPath implementation into Abstract class. Then, all anyone needs to do to change the behaviour of this algorithm
	is override evaluate. Could contribute this back to HOG; abstractAStar??
	
	Current setup isn't too bad though; just inherit from here and override as needed.
	
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterAStar.h"
#include "ClusterNode.h"
#include "mapAbstraction.h"
#include "MacroNode.h"
#include "timer.h"
#include "altheap.h"
#include "graph.h"

#include <sstream>

using namespace std;


bool AbstractClusterAStar::isInCorridor(node* _n)
{
/*	ClusterNode* n = dynamic_cast<ClusterNode*>(_n);
	if(n->getParentClusterId() != corridorClusterId) 
		return false; 
	return true; */
	
	if(corridorNodes == NULL) // corridor not set. every node should be considered.
		return true;
	
	if(corridorNodes->find(_n->getUniqueID()) != corridorNodes->end()) // already added
		return true;

	return false;
}

// debugging function
void AbstractClusterAStar::printPath(path* p)
{
	while(p)
	{
		node* n = p->n;
		std::cout << "id: "<<n->getUniqueID()<<" node @ "<<n->getName()<<std::endl;
		p = p->next;
	}		
}

/*
	1. get the current node on the open list
	2. check if node is the goal; goto 6 if yes.
	3. evaluate each neighbour of the newly opened node
		b. if neighbour is on the closed list, skip it
		a. if neighbour is already on the open list, update weights
		c. else check if node is reachable by the agent
			i. if reachable, push node on the open list
			ii. else, skip node
	4. push node on closed list
	5. if openlist is null return failure, else, goto 1
	6. return path
*/
path* ClusterAStar::getPath(graphAbstraction *aMap, node* from, node* to, reservationProvider *rp)
{
	if(verbose) 
	{
		std::cout << "getPath() mapLevel: ";
		std::cout <<from->getLabelL(kAbstractionLevel)<<std::endl;
	}
	if(!checkParameters(aMap, from, to))
	{
		nodesExpanded=0;
		nodesTouched=0;
		peakmemory = 0;
		searchTime =0;
		return NULL;
	}
	this->setGraphAbstraction(aMap);

	graph *g = aMap->getAbstractGraph(from->getLabelL(kAbstractionLevel));
	return search(g, from, to);
}

path* AbstractClusterAStar::search(graph* g, node* from, node* to)
{
	nodesExpanded=0;
	nodesTouched=0;
	peakmemory = 0;
	searchTime =0;

	// label start node cost 0 
	from->setLabelF(kTemporaryLabel, h(from, to));
	//from->setLabelF(kTemporaryLabel, 0);
	from->markEdge(0);
	
	heap* openList = new heap(30);
	std::map<int, node*> closedList;
	
	openList->add(from);
	path *p = NULL;
	
	Timer t;
	t.startTimer();
	while(1) 
	{
		/* get the current node on the open list and check if it contains the goal */
		peakmemory = openList->size()>peakmemory?openList->size():peakmemory;
		node* current = ((node*)openList->remove()); 

		if(current == to)
		{
			p = extractBestPath(g, current->getNum());
			if(verbose)
			{	
				printNode(string("goal found! "), current);
			}
			break;
		}
		
		expand(current, to, openList, closedList, g);
				
		/* check if there is anything left to search; fail if not */
		if(openList->empty())
		{
			if(verbose) std::cout << "search failed. ";
			break;
		}
	}
	searchTime = t.endTimer();
	delete openList; 
	closedList.clear();

	if(verbose)
	{
		std::cout << "\n";
		printPath(p);
	}

	return p;	
}

void AbstractClusterAStar::expand(node* current, node* to, heap* openList, std::map<int, node*>& closedList, graph* g)
{
	if(verbose) printNode(string("expanding... "), current, to);
	nodesExpanded++;

	/* evaluate each neighbour of the newly opened node */
	edge_iterator ei = current->getEdgeIter();
	edge* e = current->edgeIterNext(ei);
	while(e)
	{
		// TODO: fix HOG's graph stuff; nodes identified using position in array instead of uniqueid. graph should just store a hash_map
		int neighbourid = e->getFrom()==current->getNum()?e->getTo():e->getFrom();
		ClusterNode* neighbour = dynamic_cast<ClusterNode*>(g->getNode(neighbourid));

		if(neighbour->getUniqueID() == current->getUniqueID())
		{
			std::cout << "break here plox"<<std::endl;
		}

		assert(neighbour->getUniqueID() != current->getUniqueID());

		
		if(closedList.find(neighbour->getUniqueID()) == closedList.end()) // ignore nodes on the closed list
		{
			// if a node on the openlist is reachable via this new edge, 
			// relax the edge (see cormen et al)
			if(openList->isIn(neighbour)) 
			{	
				if(evaluate(current, neighbour, e)) 
				{		
					if(verbose) std::cout << "\t\trelaxing; ";
					relaxEdge(openList, g, e, current->getNum(), neighbourid, to); 
					nodesTouched++;
					if(verbose)
						std::cout << " f: "<<neighbour->getLabelF(kTemporaryLabel)<<std::endl;
				}
				else
				{
					if(verbose)
						std::cout << "\t\tin open list but not evaluating?!?!";
				}
			}
			else
			{
				if(evaluate(current, neighbour, e)) 
				{
					if(verbose) std::cout << "\t\tadding to open list";

					neighbour->setLabelF(kTemporaryLabel, MAXINT); // initial fCost = inifinity
					neighbour->setKeyLabel(kTemporaryLabel); // an initial key value for prioritisation in the openlist
					neighbour->reset();  // reset any marked edges from previous searches 
					openList->add(neighbour);
					relaxEdge(openList, g, e, current->getNum(), neighbourid, to); 
					nodesTouched++;

					if(verbose)
						std::cout << "; f: "<<neighbour->getLabelF(kTemporaryLabel);

				}
				else
				{
					if(verbose)
						std::cout << "\t\tnot in open list and not evaluating";
				}

			}
			if(markForVis)
				neighbour->drawColor = 1; // visualise touched
		}
		else
		{
			if(verbose) std::cout << "\t\tclosed!";
			double fclosed = neighbour->getLabelF(kTemporaryLabel);
			double gclosed =  fclosed - h(neighbour, to);

			// alternate fcost
			double alth = h(neighbour, to);
			double altg = current->getLabelF(kTemporaryLabel) - h(current, to);

			if((altg + e->getWeight() + alth) < fclosed)
			{
				std::cout << "node "<<neighbour->getName()<<" expanded out of order! ";
				std::cout << " fClosed = "<<fclosed << " fActual: "<<altg + e->getWeight() + alth;
				std::cout << " gClosed = "<<gclosed<< "; alternative: "<<altg+e->getWeight();
				printNode("\nfaulty node: ", neighbour, to); 
				std::cout << std::endl;
				printNode(" alt parent: ", current, to);
				std::cout << std::endl;
			}

		}
		if(verbose)
		{
			printNode(string("\tneighbour... "), neighbour);
			std::cout << std::endl;
		}
		e = current->edgeIterNext(ei);
	}
		
	if(markForVis)
		current->drawColor = 2; // visualise expanded

	if(verbose)
	{	
		printNode(string("closing... "), current);
		std::cout << " f: "<<current->getLabelF(kTemporaryLabel) <<std::endl;
	}
	closedList[current->getUniqueID()] = current;	
}

ClusterAStar::ClusterAStar() : AbstractClusterAStar()
{
	cardinal = false;
}

ClusterAStar::~ClusterAStar()
{
}

/* evaluate()
	check if it is possible to move from the current location to an adjacent target location via some edge.
	things we look for:
		- both nodes are non null
		- both nodes are inside the corridor (if useCorridor is set)
*/
bool ClusterAStar::evaluate(node* current, node* target, edge* e)
{
	if(!current || !target)
	{
		if(verbose)
		{
//			if(!current)
//				std::cout << " ::evaluate: current not set!";
//			if(!target)
//				std::cout << " ::evaluate: current not set!";
		}
		return false;
	}
				
	if(!isInCorridor(target))
	{
		//if(verbose)
		//{
		//	std::cout << " ::evaluate: target not in corridor!";
		//}
		return false;
	}

	if(cardinal && e->getWeight() != (int)e->getWeight())
	{
		//if(verbose)
		//{
		//	std::cout << " ::evaluate: cardinal set and edge weight not integer!";
		//}
		return false;
	}

	return true;
}

void ClusterAStar::logFinalStats(statCollection *stats)
{
	searchAlgorithm::logFinalStats(stats);
	stats->addStat("peakMemory",getName(),getPeakMemory());
}

bool ClusterAStar::checkParameters(graphAbstraction* aMap, node* from, node* to)
{
	if(aMap == NULL)
		return false;
				
	if(!from || !to)
		return false;

	if(from->getUniqueID() == to->getUniqueID())
		return false;
		
	if(from->getLabelL(kFirstData) == to->getLabelL(kFirstData) && from->getLabelL(kFirstData+1) == to->getLabelL(kFirstData+1))
		return false;

	return true;
}

void AbstractClusterAStar::printNode(string msg, node* n, node* goal)
{	
	std::cout << msg <<"addr: "<<&(*n)<<" num: "<<n->getUniqueID();
	std::cout <<" ("<<n->getLabelL(kFirstData)<<","<<n->getLabelL(kFirstData+1)<<") ";

	if(dynamic_cast<MacroNode*>(n))
	{
		MacroNode* mp = dynamic_cast<MacroNode*>(n)->getMacroParent();
		if(mp)
		{
			std::cout << " mp: "<<dynamic_cast<MacroNode*>(n)->getMacroParent()->getName()<<" ";
		}
			if(n->getMarkedEdge())
			{
				graph* g =  getGraphAbstraction()->getAbstractGraph(n->getLabelL(kAbstractionLevel));
				edge* e = n->getMarkedEdge();
				int parentId = e->getTo() == n->getNum()?e->getFrom():e->getTo();
				node* parent = g->getNode(parentId);
				std::cout << " p: ("<<parent->getLabelL(kFirstData)<<", "<<parent->getLabelL(kFirstData+1)<<") ";
			}
	}

	if(goal)
	{
		double hcost = h(n, goal);
		//double hcost = 0; 
		double gcost = n->getLabelF(kTemporaryLabel) - hcost;
		std::cout << " f: "<<gcost+hcost<<" g: "<<gcost<<" h: "<<hcost<<std::endl;
	}
}
