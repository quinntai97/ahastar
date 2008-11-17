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
#include "timer.h"
#include "heap.h"
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
	nodesExpanded=0;
	nodesTouched=0;
	peakmemory = 0;
	searchtime =0;
	
	if(!checkParameters(aMap, from, to))
		return NULL;
	
	this->setGraphAbstraction(aMap);
	
	// label start node cost 0 
	from->setLabelF(kTemporaryLabel, 1*aMap->h(from, to));
	from->markEdge(0);
	
	/* initialise the search params */
	graph *g = aMap->getAbstractGraph(from->getLabelL(kAbstractionLevel));
	heap* openList = new heap(30);
	std::map<int, bool> closedList;
	
	openList->add(from);
	path *p = NULL;
	
	Timer t;
	t.startTimer();
	while(1) 
	{
		/* get the current node on the open list and check if it contains the goal */
		peakmemory = openList->size()>peakmemory?openList->size():peakmemory;
		node* current = ((node*)openList->remove()); 
		//int cx = current->getLabelL(kFirstData);
		//int cy = current->getLabelL(kFirstData+1);
		nodesExpanded++;
		if(current == to)
		{
			p = extractBestPath(g, current->getNum());
			break;
		}
		
		/* evaluate each neighbour of the newly opened node */
		edge_iterator ei = current->getEdgeIter();
		edge* e = current->edgeIterNext(ei);
		while(e)
		{
			// TODO: fix HOG's graph stuff; nodes identified using position in array instead of uniqueid. graph should just store a hash_map
			int neighbourid = e->getFrom()==current->getNum()?e->getTo():e->getFrom();
			node* neighbour = g->getNode(neighbourid);
			//int nx = neighbour->getLabelL(kFirstData);
			//int ny = neighbour->getLabelL(kFirstData+1);
			//double weight = e->getWeight();

			if(closedList.find(neighbour->getUniqueID()) == closedList.end()) // ignore nodes on the closed list
			{
				// if a node on the openlist is reachable via this new edge, relax the edge (see cormen et al)
				if(openList->isIn(neighbour)) 
				{	
					if(evaluate(current, neighbour, e)) 
					{
						relaxEdge(openList, g, e, current->getNum(), neighbourid, to); 
						nodesTouched++;
					}
				}
				else
				{
					if(evaluate(current, neighbour, e)) 
					{
						neighbour->setLabelF(kTemporaryLabel, MAXINT); // initial fCost = inifinity
						neighbour->setKeyLabel(kTemporaryLabel); // an initial key value for prioritisation in the openlist
						neighbour->markEdge(0);  // reset any marked edges (we use marked edges to backtrack over optimal path when goal is found)
						openList->add(neighbour);
						relaxEdge(openList, g, e, current->getNum(), neighbourid, to); 
						nodesTouched++;
					}
				}
				
			}
			e = current->edgeIterNext(ei);
		}

		closedList[current->getUniqueID()] = true;
		
		/* check if there is anything left to search; fail if not */
		if(openList->empty())
			break;
	}
	searchtime = t.endTimer();
	delete openList; 
	closedList.clear();
	return p;	
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
		return false;
				
	if(!isInCorridor(target))
		return false;

	return true;
}

void ClusterAStar::logFinalStats(statCollection *stats)
{
	stats->addStat("nodesExpanded",getName(),getNodesExpanded());
	stats->addStat("nodesTouched",getName(),getNodesTouched());
	stats->addStat("peakMemory",getName(),getPeakMemory());
	stats->addStat("searchTime",getName(),getSearchTime());
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
