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

node* AnnotatedNodeFactory::newNode(const char* name) throw(std::invalid_argument)
{
	return new AnnotatedNode(name);
}

node* AnnotatedNodeFactory::newNode(const node* _n) throw(std::invalid_argument)
{
	const AnnotatedNode* n = dynamic_cast<const AnnotatedNode*>(_n);
	if(!n)
		throw std::invalid_argument("AnnotatedNodeFactory::newNode requires a node of type 'AnnotatedNode'");
		
	return new AnnotatedNode(n);
}
