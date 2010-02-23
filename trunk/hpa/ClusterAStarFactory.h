/*
 *  ClusterAStarFactory.h
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERASTARFACTORY_H
#define CLUSTERASTARFACTORY_H

#include "IClusterAStarFactory.h"

class ClusterAStar;
class ClusterAStarFactory : public IClusterAStarFactory 
{
	public:
		virtual ~ClusterAStarFactory() { cardinal = false; }
		virtual AbstractClusterAStar* newClusterAStar();
		void setCardinal(bool _cardinal) { this->cardinal = _cardinal; }
		bool getCardinal() { return cardinal; }

	private:
		bool cardinal;
};

#endif
