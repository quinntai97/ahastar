/*
 *  EdgeFactoryMock.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EDGEFACTORYMOCK_H
#define EDGEFACTORYMOCK_H

#include "IEdgeFactory.h"

class edge;

class EdgeFactoryMock : public IEdgeFactory
{	
	public:
		EdgeFactoryMock() : count(0) {}
		virtual edge* newEdge(unsigned int from, unsigned int to, double weight);
		
		int count;
};

#endif