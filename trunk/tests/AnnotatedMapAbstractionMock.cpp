/*
 *  AnnotatedMapAbstractionMock.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 8/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedMapAbstractionMock.h"
#include "ExperimentManager.h"
#include "aStar3.h"

using namespace std;
using namespace ExpMgrUtil;


/* fake it for every map except "demo.map"; when we have demo.map we still fake pathable using a* instead of annotateda*/
bool AnnotatedMapAbstractionMock::pathable(node* start, node* goal, int terrain, int agentsize)
{
	if(terrain = curexp->terrain && agentsize == curexp->size)
	{
		int sx, sy, gx, gy;
		sx = start->getLabelL(kFirstData);
		sy = start->getLabelL(kFirstData+1);
		gx = goal->getLabelL(kFirstData);
		gy = goal->getLabelL(kFirstData+1);
		if(sx == curexp->startx && sy == curexp->starty && gx == curexp->goalx && gy == curexp->goaly)
			return curexp->pathable;
	}
	
	return false;
}
bool AnnotatedMapAbstractionMock::pathable(node* start, node* goal)
{
	return false; 
}
