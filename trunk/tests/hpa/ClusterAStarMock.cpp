/*
 *  ClusterAStarMock.cpp
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterAStarMock.h"

path* ClusterAStarMock::getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp)
{
	this->getPathMocker.forward(aMap, from, to);
}