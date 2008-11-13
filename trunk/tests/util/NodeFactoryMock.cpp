/*
 *  NodeFactoryMock.cpp
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeFactoryMock.h"
#include "graph.h"

node* NodeFactoryMock::newNode(const char* name) throw(std::invalid_argument)
{
	count++;
	return new node(name);
}

node* NodeFactoryMock::newNode(const node* n) throw(std::invalid_argument)
{
	count++;
	return new node(n);	
}