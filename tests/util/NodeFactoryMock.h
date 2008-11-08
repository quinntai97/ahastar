/*
 *  NodeFactoryMock.h
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NODEFACTORYMOCK_H
#define NODEFACTORYMOCK_H

#include "INodeFactory.h"

class NodeFactoryMock : public INodeFactory
{
	public:
		NodeFactoryMock() : count(0) {}
		virtual node* newNode(const char* name);
		int count;
};

#endif