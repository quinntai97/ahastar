/*
 *  AnnotatedHierarchicalAStar.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 7/04/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedHierarchicalAStar.h"
#include "AnnotatedClusterAbstraction.h"

bool AnnotatedHierarchicalAStar::evaluate(node* n, node* target) 
{
	if(!n || !target) 
		return false;

	/* only evaluate nodes connected by the edge currently being traversed */
	edge* e = this->traversing();
	if(!e)
		return false;
	
	unsigned int to = e->getTo();
	unsigned int from = e->getFrom();
	if(n->getNum() != to && n->getNum() != from)
		return false;
	if(target->getNum() != to && target->getNum() != from)
		return false;
		
	int capability = this->getCapability();
	int clearance = this->getClearance();
	
	if(e->getClearance(capability) >= clearance)
		return true;
	
	return false;
}

/* 
Find an abstract path and refine it using the path cache

 NB: sometimes we may require a cached path which was obtained by planning in the reverse direction to current requirements
 ie. we store the path from n1 -> n2, but we may require the path from n2 -> n1. 
 In such cases, we specify the id of the node that should be at the head of the path. if the cached path doesn't meet those
 requirements, we reverse it. 
*/
path* AnnotatedHierarchicalAStar::getPath(graphAbstraction* aMap, node* from, node* to, reservationProvider *rp)
{
	AnnotatedClusterAbstraction* aca = dynamic_cast<AnnotatedClusterAbstraction*>(aMap);
	assert(aca != 0); 
		
	insertNodesExpanded = insertNodesTouched = insertPeakMemory =0;
	insertSearchTime = 0;

	path* thepath=0;

	if(from->getParentCluster() == to->getParentCluster())
	{
		AnnotatedAStar aastar;
		aastar.setGraphAbstraction(aMap);
		aastar.setCapability(this->getCapability());
		aastar.setClearance(this->getClearance());
		aastar.limitSearchToClusterCorridor(true);
		thepath = aastar.getPath(aMap, from, to, rp);
		this->nodesExpanded += aastar.getNodesExpanded();
		this->nodesTouched += aastar.getNodesTouched();
		if(this->peakmemory < aastar.getPeakMemory())
			this->peakmemory = aastar.getPeakMemory();
		this->searchtime += aastar.getSearchTime();
	}
	
	if(thepath==0)
	{
		aca->insertStartAndGoalNodesIntoAbstractGraph(from, to);
		graph *absg = aca->getAbstractGraph(1);
		node* absstart = absg->getNode(from->getLabelL(kParent));
		node* absgoal = absg->getNode(to->getLabelL(kParent));
		
	/*	edge_iterator ei = absg->getEdgeIter();
		edge* e = absg->edgeIterNext(ei);
		while(e)
		{
			node* f = absg->getNode(e->getFrom());
			node* t = absg->getNode(e->getTo());
			std::cout << "\n edge connects "<<f->getLabelL(kFirstData)<<","<<f->getLabelL(kFirstData+1)<< " and "<<t->getLabelL(kFirstData)<<","<<t->getLabelL(kFirstData+1);
			std::cout <<"(weight: "<<e->getWeight()<<" caps: "<<e->getCapability() << " clearance: "<<e->getClearance(e->getCapability())<<")";
			e = absg->edgeIterNext(ei);
		}
	*/

		path* abspath = getAbstractPath(aMap, absstart, absgoal);
		
		if(abspath)
		{
			// debugging
	/*		std::cout << "\n abstract path: ";
			path* tmpptr = abspath;
			while(tmpptr)
			{
				node* n = tmpptr->n;
				std::cout << "\n id: "<<n->getUniqueID()<<" node @ "<<n->getLabelL(kFirstData) << ","<<n->getLabelL(kFirstData+1);
				tmpptr = tmpptr->next;
			}
		
	*/
			AnnotatedAStar aastar;
			int capability = this->getCapability();
			int clearance = this->getClearance();
			path* tail;
			path* tmp = abspath;//->next;
			while(tmp->next)
			{
				edge* e = tmp->n->findAnnotatedEdge(tmp->next->n,capability,clearance,MAXINT);
				if(e == NULL)
				{
					std::cout << "\n AHA::getPath -- something went horribly wrong; I couldn't find any cached paths. Search params: ";
					std::cout << "from: "<<from->getLabelL(kFirstData)<<","<<from->getLabelL(kFirstData+1);
					std::cout << " to: "<<to->getLabelL(kFirstData)<<","<<to->getLabelL(kFirstData+1);
					std::cout << " caps: "<<capability<<" clearance: "<<clearance;
					exit(-1);
				}
				
//				path refinement. enable this and comment out section below to turn off caching (one or the other)
				// [refine]
				node* llstart = aca->getNodeFromMap(tmp->n->getLabelL(kFirstData), tmp->n->getLabelL(kFirstData+1));
				node* llgoal = aca->getNodeFromMap(tmp->next->n->getLabelL(kFirstData), tmp->next->n->getLabelL(kFirstData+1));
				aastar.setCapability(this->getCapability());
				aastar.setClearance(this->getClearance());
				path* cachedpath = aastar.getPath(aMap,llstart, llgoal); 
				this->nodesExpanded += aastar.getNodesExpanded();
				this->nodesTouched += aastar.getNodesTouched();
				if(this->peakmemory < aastar.getPeakMemory())
					this->peakmemory = aastar.getPeakMemory();
				this->searchtime += aastar.getSearchTime();
				// [/refine]

/*				// [cache]
				path* cachedpath = aca->getPathFromCache(e)->clone();
				if(e->getFrom() != tmp->n->getNum()) // fix segments if necessary
					cachedpath = cachedpath->reverse();
				// [/cache]
*/
				if(thepath == 0)
					thepath = cachedpath;				
				tail = thepath->tail();	
				
				/*	// debugging
					node* n1 = absg->getNode(e->getFrom());
					node* n2 = absg->getNode(e->getTo());		
					std::cout << "\n expanding abstract edge between nodes: "<<n1->getUniqueID()<<" and "<<n2->getUniqueID();
					path* meh = cachedpath;
					std::cout << "\n expanding cached path: ";
					while(meh)
					{
						std::cout << "\n id: "<<meh->n->getUniqueID()<<" node @ "<<meh->n->getLabelL(kFirstData) << ","<<meh->n->getLabelL(kFirstData+1);
						meh = meh->next;
					}
				*/
				
				if(tail->n->getNum() == cachedpath->n->getNum()) // avoid overlap where the cached path segments overlap (one ends where another begins)
					tail->next = cachedpath->next;
				
				tmp = tmp->next;
			}
		}	
		insertNodesExpanded = aca->getNodesExpanded();
		insertNodesTouched = aca->getNodesTouched();
		insertPeakMemory = aca->getPeakMemory();
		insertSearchTime = aca->getSearchTime();

		aca->removeStartAndGoalNodesFromAbstractGraph();
		delete abspath;
	}
		
	this->nodesExpanded += insertNodesExpanded;
	this->nodesTouched += insertNodesTouched;
	this->searchtime += insertSearchTime;
	
	if(this->peakmemory < insertPeakMemory)
		this->peakmemory = insertPeakMemory;

	//std::cout << "\n thepath distance: "<<aMap->distance(thepath);
	return thepath;
}

void AnnotatedHierarchicalAStar::logFinalStats(statCollection* stats)
{
	AnnotatedAStar::logFinalStats(stats);
	
	stats->addStat("insNodesExpanded",getName(),getInsertNodesExpanded());
	stats->addStat("insNodesTouched",getName(),getInsertNodesTouched());
	stats->addStat("insPeakMemory",getName(),getInsertPeakMemory());
	stats->addStat("insSearchTime",getName(),getInsertSearchTime());
}
