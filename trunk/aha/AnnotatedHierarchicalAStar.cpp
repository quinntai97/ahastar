/*
 *  AnnotatedHierarchicalAStar.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 7/04/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedHierarchicalAStar.h"

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
