/*
 *  MockNode.cpp
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeMock.h"

graph_object* nodeMock::clone() const
{
	return this->cloneMocker.forward();
}

