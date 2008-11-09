/*
 *  AnnotatedEdge.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedEdge.h"

AnnotatedEdge::AnnotatedEdge(const AnnotatedEdge* e) : edge(e)
{
	this->clearance = e->clearance;
	this->capability = e->capability;
}

graph_object* AnnotatedEdge::clone() const
{
	return new AnnotatedEdge(this);
}

void AnnotatedEdge::setClearance(int capability, int clearance)
{
	if(clearance <= 0)
		return;
	this->capability = capability;
	this->clearance = clearance;
}
int AnnotatedEdge::getClearance(int agentcapability)
{
	if((this->capability & agentcapability) == capability)
		return clearance;

	return 0;
}