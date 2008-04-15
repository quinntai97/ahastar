/*
 *  AnnotatedAStarMock.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 7/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedAStarMock.h"
#include "TestConstants.h"
#include "ExperimentManager.h"

using namespace std;
using namespace ExpMgrUtil;

/* we rig the result by checking for the key associated with the most recently requested ExpMgrUtil::TestExperiment. If the 
	input matches that experiment, we return the rigged result. Otherwise, always false. */
path* AnnotatedAStarMock::getPath(graphAbstraction* aMap, node* start, node* goal, reservationProvider *rp)
{
	if(curexp)
	{
		if(!curexp || curexp->pathable == false)
			return NULL;

		if((start->getTerrainType()&getCapability()) == start->getTerrainType() && (goal->getTerrainType()&getCapability()) == goal->getTerrainType())
		{
			path* p;
			p = new path(goal);
			p = new path(start, p);
			return p;
		}
	}

	return NULL;
	
/*	if(curexp == NULL)
		return 0;
	
	path* p=0;
	if(terrain == curexp->terrain && agentsize == curexp->size)
	{
		int sx, sy, gx, gy;
		sx = start->getLabelL(kFirstData);
		sy = start->getLabelL(kFirstData+1);
		gx = goal->getLabelL(kFirstData);
		gy = goal->getLabelL(kFirstData+1);

		if(sx == curexp->startx && sy == curexp->starty && gx == curexp->goalx && gy == curexp->goaly && curexp->pathable) 
			p = new path(start);
	}
	
	curexp = NULL;	
	return p;*/
}

bool AnnotatedAStarMock::evaluate(node* n, node* target)
{
	return false;
}