/*
 *  AnnotatedEdge.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDEDGE_H
#define ANNOTATEDEDGE_H

#include "graph.h"

class AnnotatedEdge : public edge
{
	public:
		AnnotatedEdge(unsigned int from, unsigned int to, double weight) : edge(from, to, weight) {	capability=0; clearance=0; }
		AnnotatedEdge(const AnnotatedEdge* e);
		virtual graph_object* clone() const;
		
		void setClearance(int, int);
		int getClearance(int);
		int getCapability() { return capability; }

	private:
		int capability;
		int clearance;
};

#endif