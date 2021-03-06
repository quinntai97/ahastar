/*
 *  AnnotatedNodeFactory.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDNODEFACTORY_H
#define ANNOTATEDNODEFACTORY_H

#include "INodeFactory.h"

class node;

class AnnotatedNodeFactory : public INodeFactory
{
	public:
		virtual ~AnnotatedNodeFactory() {}
		virtual node* newNode(const char* name) throw(std::invalid_argument);
		virtual node* newNode(const node* name) throw(std::invalid_argument);

		virtual AnnotatedNodeFactory* 
			clone() { return new AnnotatedNodeFactory(); }
};

#endif
