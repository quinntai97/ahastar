/*
 *  CardinalAStarFactory.h
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CARDINALASTARFACTORY_H
#define CARDINALASTARFACTORY_H

#include "IClusterAStarFactory.h"

class CardinalAStarFactory : public IClusterAStarFactory 
{
	public:
		virtual ~CardinalAStarFactory() { }
		virtual AbstractClusterAStar* newClusterAStar();
};

#endif
