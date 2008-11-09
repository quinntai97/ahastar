/*
 *  AnnotatedEdgeFactory.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedEdgeFactory.h"
#include "AnnotatedEdge.h"

edge* AnnotatedEdgeFactory::newEdge(unsigned int from, unsigned int to, double weight)
{
	return new AnnotatedEdge(from, to, weight);
}
