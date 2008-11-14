/*
 *  MockClusterNode.cpp
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterNodeMock.h"

graph_object* ClusterNodeMock::clone() const
{
	return this->cloneMocker.forward();
}

