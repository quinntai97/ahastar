/*
 *  AnnotatedAStar.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedAStar.h"
#include "AnnotatedMapAbstraction.h"
#include "Timer.h"

using namespace std;

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
	5. if openlist is null return failure
	6. else, goto 1
*/
path* AnnotatedAStar::getPath(graphAbstraction *aMap, node *from, node* to, int capability, int agentsize)
{
	if(aMap == NULL || !dynamic_cast<AbstractAnnotatedMapAbstraction*>(aMap))
		return NULL;

	if(agentsize <= 0)
	{	
		//if(verbose) std::cout << "AnnotatedAStar: attempted to getPath for agentsize <= 0" << std::endl;
		return NULL;
	}

	if(!from || !to)
		return NULL;

	if(from->getUniqueID() == to->getUniqueID())
		return NULL;
		
	if(from->getLabelL(kFirstData) == to->getLabelL(kFirstData) && from->getLabelL(kFirstData+1) == to->getLabelL(kFirstData+1))
		return NULL;

	/* both locations need to be reachable by agent */
	if(from->getClearance(capability) < agentsize || to->getClearance(capability) < agentsize) 
		return NULL;		
	
	//TODO: need a test to check that we've set the fCost value of the start node.
	// label start node cost 0 
	from->setLabelF(kTemporaryLabel, 1*aMap->h(from, to));
	from->markEdge(0);
	
	/* initialise the search params */
	setGraphAbstraction(aMap);
	this->setSearchTerrain(capability);
	this->setMinClearance(agentsize);
	graph *g = aMap->getAbstractGraph(0);
	heap* openList = new heap(30);
	AAStarUtil::NodeMap closedList;
	openList->add(from);
	path *p = NULL;
	
	Timer t;
	nodesExpanded=0;
	nodesTouched=0;
	peakmemory = 0;
	searchtime =0;
	
	if(useCorridor)
		this->setCorridorClusters(from->getParentCluster(),to->getParentCluster());
		
/*	int fromx = from->getLabelL(kFirstData);
	int fromy = from->getLabelL(kFirstData+1);
	int tox = to->getLabelL(kFirstData);
	int toy = to->getLabelL(kFirstData+1);
*/
	
	t.startTimer();
	while(1) 
	{
		/* get the current node on the open list and check if it contains the goal */
		peakmemory = openList->size()>peakmemory?openList->size():peakmemory;
		node* current = ((node*)openList->remove()); 
		int cx = current->getLabelL(kFirstData);
		int cy = current->getLabelL(kFirstData+1);
		nodesExpanded++;
		if(current == to)
		{
			p = extractBestPath(g, current->getNum());
			break;
		}
		
		/* evaluate each neighbour of the newly opened node */
		edge_iterator ei = current->getEdgeIter();
		edge *e = current->edgeIterNext(ei);
		while(e)
		{
			// TODO: fix HOG's graph stuff; nodes identified using position in array instead of uniqueid. graph should just store a hash_map
			int neighbourid = e->getFrom()==current->getNum()?e->getTo():e->getFrom();
			node* neighbour = g->getNode(neighbourid);
			int nx = neighbour->getLabelL(kFirstData);
			int ny = neighbour->getLabelL(kFirstData+1);

			if(!closedList[neighbour->getUniqueID()]) // skip nodes we've already closed
			{
				// if a node on the openlist is reachable via this new edge, relax the edge (see cormen et al)
				if(openList->isIn(neighbour)) 
				{	
					if(evaluate(current, neighbour)) 
					{
						relaxEdge(openList, g, e, current->getNum(), neighbourid, to); 
						nodesTouched++;
					}
				}
				else
				{
					if(evaluate(current, neighbour)) 
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
	check if it is possible to move from the current location to an adjacent target location.
	things we look for:
		- clearance value of the target >= minclearance
		- if the traversal involves a diagonal move, is there an equivalent 2-step move using the cardinal directions?
		
		NB: we assume that an edge exists between the current and target node parameters. Could check this explicitly but HOG's 
		implementation for this stuff is expensive (iteratres over all neighbours). We also only call this from getPath which ensures that
		we only evaluate pairs of connected nodes. 
		
		Probably need to make this function protected rather than public.
								
		Other stuff: 
			* need to move this into abstract implementation; if edge weight > 1.0 (ie. we're looking at an edge part of an abstract graph) then
			* we need to check the width of the corridor to determine if the edge is traversable.
		
			So, does that mean I don't need a separate AHA*?! The actual A* search should be identical to AA* except for this bit!
*/
bool AnnotatedAStar::evaluate(node* current, node* target)
{
	if(!current || !target)
		return false;
				
	AbstractAnnotatedMapAbstraction* ama = (AbstractAnnotatedMapAbstraction*)getGraphAbstraction();
	graph *g = ama->getAbstractGraph(0);

	int tx, ty, tcl, tterr;
	tterr = target->getTerrainType();
	tcl = target->getClearance(this->getSearchTerrain());
	tx = target->getLabelL(kFirstData);
	ty = target->getLabelL(kFirstData+1);
	if(target->getClearance(this->getSearchTerrain()) < this->getMinClearance())
		return false;

	if(useCorridor && !isInCorridor(target))
		return false;

	/* check if we're moving in a cardinal direction */
	tDirection dir = getDirection(current, target);
	if(dir == kStay)
		return false;
		
	if(dir == kN || dir == kS || dir == kE || dir == kW)
		return true;
		
	/* check diagonal move is equivalent to 2-step cardinal move */
	int curx = current->getLabelL(kFirstData);
	int cury = current->getLabelL(kFirstData+1);

	switch(dir) // nb: use of abstractannotatedmapabstraction implies edge exists between each pair of nodes
	{
		case kNW:
			if(evaluate(current, ama->getNodeFromMap(curx-1,cury)) && evaluate(current, ama->getNodeFromMap(curx, cury-1)))
				return true;
			break;
		case kNE: 
			if(evaluate(current, ama->getNodeFromMap(curx+1,cury)) && evaluate(current, ama->getNodeFromMap(curx, cury-1)))
				return true;
			break;
		case kSE:
			if(evaluate(current, ama->getNodeFromMap(curx+1,cury)) && evaluate(current, ama->getNodeFromMap(curx, cury+1)))
				return true;
			break;
		case kSW:
			if(evaluate(current, ama->getNodeFromMap(curx-1,cury)) && evaluate(current, ama->getNodeFromMap(curx, cury+1)))
				return true;
			break;
		default:
			cerr << "\nfatal: edge weight > 1 but move direction is not diagonal!\n ";
			break;
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
			case 0:
				break;
			default: // not moving along x-axis
				return kStay;
		};
		
		switch(deltay)
		{
			case 1: // add northerly component
				dir = dir|kN;
				break;
			case -1: // add southerly component
				dir = dir|kS;
				break;
			case 0:
				break;
			default: // not moving along y-axis
				return kStay;
		}
		
		return (tDirection)dir;
}