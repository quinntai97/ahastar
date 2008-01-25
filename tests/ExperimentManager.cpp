/*
 *  ExperimentManager.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 18/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ExperimentManager.h"
#include "TestConstants.h"

using namespace ExpMgrUtil;

ExpMgrUtil::ExperimentCollection* ExperimentManager::tests = NULL;
int ExperimentManager::refcount=0;

ExpMgrUtil::TestExperiment::TestExperiment(int sx, int sy, int gx, int gy, int terrain, int size, double dist, string mapname, bool pathable)
{
	this->startx = sx;
	this->starty = sy; 
	this->goalx = gx;
	this->goaly = gy; 
	this->terrain = terrain;
	this->size = size;
	this->distance = dist;
	this->mapname = mapname;
	this->pathable = pathable;
}


ExperimentManager::ExperimentManager()
{
	if(tests == NULL) 
	{
		tests = new ExperimentCollection();
		generateExperiments();
	}

	refcount++;
}

ExperimentManager::~ExperimentManager()
{
	refcount--;
	if(refcount == 0) 
	{
		for (__gnu_cxx::hash_map<int, TestExperiment*>::iterator i = tests->begin(); i != tests->end(); ++i)
			delete i->second;	
		
		tests->clear();
		delete tests;
		tests = NULL; // reset the initialization
	}
}

TestExperiment* ExperimentManager::getExperiment(ExperimentKey expkey)
{
	last = expkey;
	return (*tests)[expkey]; // TODO: check behaviour when expkey is nonexistent in the hashtable
}

void ExperimentManager::generateExperiments()
{	
	if(tests->size() == 0)
	{
		(*tests)[((int)kPathableToyProblemLST)] =  new TestExperiment(1,2,5,1,4,2,3.0+ROOT_TWO, maplocation, true);
		(*tests)[((int)kNotPathableGoalIsSoftObstacleLST)] = new TestExperiment(1,2,22,1,4,2,0, maplocation, false);
		(*tests)[((int)kNotPathableStartIsHardObstacleLST)] = new TestExperiment(0,0,22,6,4,2,0, maplocation, false);

	}
}

/*TestExperiment getExperiment(ExperimentKey expkey)
{
/*	switch(expkey)
	{	
	
		
/*		case kNotPathableHardObstacleLST:
		{
			TestExperiment exp = {1,2,22,1,4,2,0, maplocation.c_str(), false};
			return exp;			
		}
		case kGoalNotTraversableLST:
		{
			TestExperiment exp = {0,0,10,5,4,2,0, maplocation.c_str(), false};
			return exp;
		} */
		
		/*
	}	
}*/
/*
TestExperiment getExperimentPathable(ExperimentKey expkey)
{
//	extern float ptrLastExperimentFunction;
//	ptrLastExperimentFunction = &getExperimentPathable;

	switch(expkey)
	{
		case kPathableToyProblemLST:
		{
			TestExperiment exp = {1,2,5,1,4,2,3.0+ROOT_TWO, maplocation.c_str(), true};
			return exp;
		}
		default: 
		{
			TestExperiment exp = {-1,-1,-1,-1,-1,-1,-1,"",false};
	//		ptrLastExperimentFunction = 0;
			return exp;
		}

	}
}


TestExperiment getExperimentOutOfBounds(ExperimentKey expkey)
{
//	extern float ptrLastExperimentFunction = &getExperimentOutOfBounds;

	switch(expkey)
	{
		case kNotPathableStartPositionOutOfBoundsLST:
		{
			TestExperiment exp = {0,0,22,6,4,2,0, maplocation.c_str(), false};	
			return exp;
		}
		case kNotPathableGoalPositionOutOfBoundsLST:
		{
			TestExperiment exp = {0,0,10,5,4,2,0, maplocation.c_str(), false};
			return exp;
		}
		default: 
		{
			TestExperiment exp = {-1,-1,-1,-1,-1,-1,-1,"",false};
			//prtLastExperimentFunction = 0;
			return exp;
		}
	}
}*/