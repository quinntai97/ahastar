/*
 *  CardinalAStarFactory.cpp
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "CardinalAStarFactory.h"
#include "CardinalAStar.h"

AbstractClusterAStar* CardinalAStarFactory::newClusterAStar()
{
	return new CardinalAStar();
}
