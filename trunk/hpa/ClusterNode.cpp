/*
 *  ClusterNode.cpp
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterNode.h"
#include "constants.h"

ClusterNode::ClusterNode(const ClusterNode* n) : node(n)
{
	this->parentClusterId = n->parentClusterId;
}

ClusterNode::ClusterNode(const char* name, int _parentClusterId) : node(name)
{
	this->parentClusterId = _parentClusterId;
	init();
}

ClusterNode::ClusterNode(const char* name) : node(name) 
{ 
	parentClusterId = -1; 
	init();
}

void ClusterNode::init()
{
	this->setLabelL(kParent, -1);
}