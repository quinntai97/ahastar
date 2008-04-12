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
	
	int to = e->getTo();
	int from = e->getFrom();
	if(n->getNum() != to && n->getNum() != from)
		return false;
	if(target->getNum() != to && target->getNum() != from)
		return false;
		
	int capability = this->getSearchTerrain();
	int clearance = this->getMinClearance();
	
	if(e->getClearance(capability) >= clearance)
		return true;
	
	return false;
}

path* AnnotatedHierarchicalAStar::getPath(graphAbstraction* aMap, node* from, node* to, int capability, int clearance)
{
	assert(dynamic_cast<AnnotatedClusterAbstraction*>(aMap)); // need a test; should throw exception instead

	AnnotatedClusterAbstraction* aca = dynamic_cast<AnnotatedClusterAbstraction*>(aMap);
	aca->insertStartAndGoalNodesIntoAbstractGraph(from, to);

	graph *absg = aca->getAbstractGraph(1);
	node* absstart = absg->getNode(from->getLabelL(kParent));
	node* absgoal = absg->getNode(to->getLabelL(kParent));
	
	path* abspath = getAbstractPath(aMap, absstart, absgoal, capability, clearance);
	
	if(!abspath)
		return NULL;
	
/*	// debugging
	std::cout << "\n abstract path: ";
	path* tmp = abspath;
	while(tmp)
	{
		node* n = tmp->n;
		std::cout << "\n id: "<<n->getUniqueID()<<" node @ "<<n->getLabelL(kFirstData) << ","<<n->getLabelL(kFirstData+1);
		tmp = tmp->next;
	}
*/
	edge* e = absg->findAnnotatedEdge(abspath->n,abspath->next->n,capability,clearance,MAXINT);
	path* thepath=aca->getPathFromCache(e);
	path* tail;
	path* tmp = abspath->next;
	while(tmp->next)
	{
		tail = thepath->tail();
		edge* e = absg->findAnnotatedEdge(tmp->n,tmp->next->n,capability,clearance,MAXINT);
		if(e == NULL)
		{
			std::cout << "\n AHA::getPath -- something went horribly wrong; I couldn't find any cached paths";
			exit(-1);
		}
		path* cachedpath = aca->getPathFromCache(e);
		
/*		// debugging
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
		if(e->getFrom() != tmp->n->getNum()) // some cached paths may be stored in reverse order to our requirements. need to reverse them.
			cachedpath = cachedpath->reverse();
			
		if(tail->n->getNum() == cachedpath->n->getNum()) // avoid overlap where the cached path segments overlap (one ends where another begins)
			tail->next = cachedpath->next;
		else 
			tail->next = cachedpath;
	
		tmp = tmp->next;
	}
	
	aca->removeStartAndGoalNodesFromAbstractGraph();
	delete abspath;
	
	return thepath;
}