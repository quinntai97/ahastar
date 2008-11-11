/*
 *  ClusterFactory.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterFactory.h"
#include "HPACluster.h"

HPACluster* HPAClusterFactory::createCluster(int xpos, int ypos, int width, int height)
{
		return new HPACluster(xpos, ypos, width, height);
}

