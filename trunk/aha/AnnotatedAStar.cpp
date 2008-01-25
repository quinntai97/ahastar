/*
 *  AnnotatedAStar.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedAStar.h"

using namespace std;

/*
	1. get the next node on the open list
	2. check if node is the goal; goto 6 if yes.
	3. evaluate each neighbour of the newly opened node
		b. if neighbour is on the closed list, skip it
		a. if neighbour is already on the open list, update weights
		c. else check if node is reachable by the agent
			i. if reachable, push node on the open list
			ii. else, skip node
	4. push node on closed list
	5. if openlist is null return failure
	6. else, goto 1
*/
path* AnnotatedAStar::getPath(graphAbstraction *aMap, node *from, node* to, int terrain, int agentsize)
{

	/* both locations need to be valid */
	if(!from || !to)
		return 0;
		
	/* both locations need to have terrain clearance >= agentsize -- ie. reachable by agent */
	if(from->getClearance(terrain) < agentsize || to->getClearance(terrain) < agentsize) 
		return 0;
		
	/* both start & goal need to be traversable by the agent NB: we end up making this check twice over the goal if solution exists */
	if(	(from->getTerrainType()&terrain) != from->getTerrainType() || (to->getTerrainType()&terrain) != to->getTerrainType())
		return 0;
	
	
	/* finally, make sure there actually exists a search problem to solve*/
	if(from == to)
		return 0;
	
	/* initialise the search params */
	setGraphAbstraction(aMap);
	searchterrain = terrain;
	minclearance = agentsize;
	graph *g = aMap->getAbstractGraph(0);
	openList = new heap(30);
	openList->add(from);
	path *p = 0;
	
	while(1) 
	{
		/* get the next node on the open list and check if it contains the goal */
		node* next = ((node*)openList->remove()); 
		if(next == to)
		{
			p = aStarOld::extractBestPath(g, next->getNum());
			break;
		}
		
		/* evaluate each neighbour of the newly opened node */
		edge_iterator ei = next->getEdgeIter();
		edge *e = next->edgeIterNext(ei);
		while(e)
		{
			// TODO: fix HOG's graph stuff; nodes identified using position in array instead of uniqueid. graph should just store a hash_map
			int neighbourid = e->getFrom()==next->getNum()?e->getTo():e->getFrom();
			node* neighbour = g->getNode(neighbourid);
			if(!closedList[neighbour->getUniqueID()]) // skip nodes we've already closed
			{
				// if a node on the openlist is reachable via this new edge, relax the edge (see cormen et al)
				if(openList->isIn(neighbour)) 
				{	
					/* (heap *nodeHeap, graph *g, edge *e, int source, int nextNode, node *d) */
					if(evaluate(next, neighbour, e))
						aStarOld::relaxEdge(openList, g, e, next->getNum(), neighbourid, to); 
				}
				else
				{
					if(evaluate(next, neighbour, e))
						openList->add(neighbour); // add all neighbouring nodes reachable by the agent to the open list
				}
				
			}
			e = next->edgeIterNext(ei);
		}
		
		/* check if there is anything left to search; fail if not */
		if(openList->empty())
			break;
	}
	
	delete openList; 
	closedList.clear();
	return p;	
}

/* check if it is possible to move from the current location to an adjacent target location.
	things we look for:
		- clearance value of the target >= minclearance
		- if the traversal involves a diagonal move, is there an equivalent 2-step move using the cardinal directions?
		
		3hrs - 23/12
*/
bool AnnotatedAStar::evaluate(node* current, node* target, edge* e)
{
	if(!current || !target)
		return false;
		
	graph *g = getGraphAbstraction()->getAbstractGraph(0);
	if(target->getClearance(searchterrain) < minclearance)
		return false;
		
	// need to find the edge between the two nodes. also need tomove all this checking code into a private checkLocations method

	if(e->getWeight() > 2) 
	{
		cerr << "fatal: edge weight > sqroot(2); target node not adjacent. something wrong with the graph \n";
		exit(-1);
	}
	
	/* check diagonal move is equivalent to 2-step cardinal move */
	if(e->getWeight() > 1)
	{
		tDirection dir = getDirection(current, target);
		switch(dir)
		{
			case kNE:
				//stuff;
				break;
			case kSE:
				// stuff;
				break;
			case kSW:
				// stuff
				break;
			case kNW: 
				// stuff
				break;
			default:
				cerr << "\nfatal: edge weight > 1 but move direction is not diagonal!\n ";
				break;
		}
	}
	
	return false;
}

/* given two adjacent locations, the current position and a target position, figure out which of the eight compass directions the move
is equivalent to (n,ne,e,se,s,sw,w,nw) 
TODO: there's alot of duplication; this method has an equivalent addPathToCache in almost every Unit class. Should merge into a static.
*/
tDirection AnnotatedAStar::getDirection(node* current, node* target)
{
		int deltax = current->getLabelL(kFirstData) - target->getLabelL(kFirstData);
		int deltay = current->getLabelL(kFirstData+1) - target->getLabelL(kFirstData+1);
		
		int dir = kStay;
		switch(deltax)
		{
			case 1: // add westerly component
				dir = kW;
				break;
			case -1: // add easterly component
				dir = kE;
				break;
			default: // not moving along x-axis
				break;
		};
		
		switch(deltay)
		{
			case 1: // add northerly component
				dir = dir|kN;
				break;
			case -1: // add southerly component
				dir = dir|kS;
				break;
			default: // not moving along y-axis
				break;
		}
		
		return (tDirection)dir;
}