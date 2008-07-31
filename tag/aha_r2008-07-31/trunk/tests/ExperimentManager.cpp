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
#include "graph.h"
#include "constants.h"

using namespace ExpMgrUtil;

ExpMgrUtil::ExperimentCollection* ExperimentManager::tests = NULL;
int ExperimentManager::refcount=0;

ExpMgrUtil::TestExperiment::TestExperiment(ExperimentKey key, int sx, int sy, int sterrain, int gx, int gy, int gterrain, int caps, int size, double dist, string mapname, bool pathable)
{
	this->key = key;
	this->startx = sx;
	this->starty = sy; 
	this->goalx = gx;
	this->goaly = gy; 
	this->sterrain = sterrain;
	this->gterrain = gterrain;
	this->caps = caps;
	this->size = size;
	this->distance = dist;
	this->mapname = mapname;
	this->pathable = pathable;

	snode = gnode = NULL;
}

ExpMgrUtil::TestExperiment::~TestExperiment()
{
	if(snode != NULL)
		delete snode;
	if(gnode != NULL)
		delete gnode;
}

node* ExpMgrUtil::TestExperiment::getStartNode()
{

	if(snode != NULL)
		return snode;
		
	// NB: might need an extension later for different clearance values (!= size)
	snode = new node("start");
	snode->setTerrainType(sterrain);
	snode->setClearance(sterrain, size);		
	snode->setLabelL(kFirstData, startx);
	snode->setLabelL(kFirstData+1, starty);
	
	if(sterrain == (kGround|kTrees))
	{
		snode->setClearance(kGround, size);
		snode->setClearance(kTrees, size);
	}

	return snode;

}

node* ExpMgrUtil::TestExperiment::getGoalNode()
{

	// NB: might need an extension later for different clearance values (!= size)
	if(gnode != NULL)
		return snode;

	gnode = new node("goal");
	gnode->setTerrainType(gterrain);
	gnode->setClearance(gterrain, size);
	gnode->setLabelL(kFirstData, goalx);
	gnode->setLabelL(kFirstData+1, goaly);

	if(gterrain == (kGround|kTrees))
	{
		gnode->setClearance(kGround, size);
		gnode->setClearance(kTrees, size);
	}

	return gnode;
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


/* TODO: have the experiment generate the start/goal nodes & all annotations etc */
void ExperimentManager::generateExperiments()
{	
	if(tests->size() == 0)
	{
		tests->insert(pair<int, TestExperiment*>((int)kPathableToyProblemLST,  new TestExperiment(kPathableToyProblemLST, 4,2,kGround,1,7,kGround,kGround,2,8, maplocation, true)));
				
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveNorthLST, new TestExperiment(kPathableMoveNorthLST, 4,8,kGround,4,7,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveSouthLST, new TestExperiment(kPathableMoveSouthLST, 4,8,kGround,4,9,kGround,4,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveEastLST, new TestExperiment(kPathableMoveEastLST, 4,8,kGround,5,8,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveWestLST, new TestExperiment(kPathableMoveWestLST, 4,8,kGround,3,8,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveNorthWestLST, new TestExperiment(kPathableMoveNorthWestLST, 4,8,kGround,3,7,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveNorthEastLST, new TestExperiment(kPathableMoveNorthEastLST, 4,8,kGround,5,7,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveSouthEastLST, new TestExperiment(kPathableMoveSouthEastLST, 4,8,kGround,5,9,kGround,kGround,2,1, maplocation, true)));
		tests->insert(pair<int, TestExperiment*>((int)kPathableMoveSouthWestLST, new TestExperiment(kPathableMoveSouthWestLST, 4,8,kGround,3,9,kGround,kGround,2,1, maplocation, true)));
		
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableGoalIsSoftObstacleLST, new TestExperiment(kNotPathableGoalIsSoftObstacleLST, 1,2,kGround,22,1,kTrees,kGround,2,0, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableStartIsSoftObstacleLST, new TestExperiment(kNotPathableStartIsSoftObstacleLST, 22,1,kTrees,1,2,kGround,kGround,2,0, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableStartIsHardObstacleLST, new TestExperiment(kNotPathableStartIsHardObstacleLST, 0,0,kWater,22,6,kGround,kGround,2,0, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableGoalIsHardObstacleLST, new TestExperiment(kNotPathableGoalIsHardObstacleLST, 22,6,kGround,0,0,kWater,kGround,2,0, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableMoveToHardObstacleLST, new TestExperiment(kNotPathableMoveToHardObstacleLST, 2,2,kGround,1,1,kWater,kGround,2,1, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableMoveToSoftObstacleLST, new TestExperiment(kNotPathableMoveToSoftObstacleLST, 5,9,kGround,6,9,kTrees,kGround,2,1, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableMoveToNonAdjacentLocationLST,  new TestExperiment(kNotPathableMoveToNonAdjacentLocationLST, 4,2,kGround,1,7,kGround,kGround,2,0, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableStartClearanceTooSmallLST,  new TestExperiment(kNotPathableStartClearanceTooSmallLST, 1,8,kGround,5,8,kGround,kGround,2,4, maplocation, false)));
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableGoalClearanceTooSmallLST,  new TestExperiment(kNotPathableGoalClearanceTooSmallLST, 5,8,kGround,1,8,kGround,kGround,2,4, maplocation, false)));		
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableStartAndGoalIdenticalLST,  new TestExperiment(kNotPathableStartAndGoalIdenticalLST, 5,8,kGround,5,8,kGround,kGround,2,4, maplocation, false)));		

		tests->insert(pair<int, TestExperiment*>((int)kNotPathableInvalidCapability,  new TestExperiment(kNotPathableInvalidCapability, 4,2,kGround,1,7,kGround,kSwamp,2,0, maplocation, false)));		
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableAgentSizeLessThanMin,  new TestExperiment(kNotPathableAgentSizeLessThanMin, 4,8,kGround,4,7,kGround,kGround,0,0, maplocation, false)));			

		tests->insert(pair<int, TestExperiment*>((int)kNotPathableHardObstacleBlocksGoal,  new TestExperiment(kNotPathableHardObstacleBlocksGoal, 1,2,kGround,22,1,kGround,kGround,2,0, maplocation, false)));			
		tests->insert(pair<int, TestExperiment*>((int)kNotPathableSoftObstacleBlocksGoal,  new TestExperiment(kNotPathableSoftObstacleBlocksGoal, 16,8,kGround,22,8,kGround,kGround,2,0, maplocation, false)));			

		tests->insert(pair<int, TestExperiment*>((int)kNotPathableWhenCorridorIsRestrictedToParentCluster,  new TestExperiment(kNotPathableWhenCorridorIsRestrictedToParentCluster, 4,4,kGround,4,1,kGround,kGround,2,0, acmap, false)));			
		tests->insert(pair<int, TestExperiment*>((int)kPathableWithinCorridor,  new TestExperiment(kPathableWithinCorridor, 1,4,kGround,4,1,kGround,kGround,1,7.41421353816986, acmap, true)));			
		

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