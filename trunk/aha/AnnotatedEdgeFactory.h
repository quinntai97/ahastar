/*
 *  AnnotatedEdgeFactory.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDEDGEFACTORY_H
#define ANNOTATEDEDGEFACTORY_H

#include "IEdgeFactory.h"

class AnnotatedEdgeFactory : public IEdgeFactory
{
	public:
		virtual edge* newEdge(unsigned int from, unsigned int to, double weight);

};

#endif
