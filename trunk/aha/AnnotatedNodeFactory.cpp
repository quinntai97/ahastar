/*
 *  AnnotatedNodeFactory.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedNodeFactory.h"
#include "AnnotatedNode.h"
#include "graph.h"

node* AnnotatedNodeFactory::newNode(const char* name)
{
	return new AnnotatedNode(name);
}
