/*
 *  AnnotatedClusterFactoryInterface.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 29/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterFactory.h"
#include "AnnotatedCluster.h"
#include "AnnotatedClusterAbstraction.h"

AnnotatedCluster* AnnotatedClusterFactory::createCluster(int xpos, int ypos, int width, int height)
{
		return new AnnotatedCluster(xpos, ypos, width, height);
}
