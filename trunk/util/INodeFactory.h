/*
 *  INodeFactory.h
 *  hog
 *
	Defines a common interface for all NodeFactory classes.
 
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INODEFACTORY_H
#define INODEFACTORY_H

class node;

class INodeFactory
{
	public:
		virtual node* newNode(const char* name) = 0;
};


#endif
