/*
 *  NodeFactory.cpp
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeFactory.h"
#include "graph.h"

node* NodeFactory::newNode(const char* name)
{
	node* n = new node(name);
	return n;
}