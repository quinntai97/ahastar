/*
 *  EdgeFactoryMock.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "EdgeFactoryMock.h"
#include "graph.h"

edge* EdgeFactoryMock::newEdge(unsigned int from, unsigned int to, double weight)
{
	count++;
	return new edge(from, to, weight);
}