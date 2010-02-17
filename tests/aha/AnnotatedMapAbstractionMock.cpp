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
bool AnnotatedMapAbstractionMock::pathable(node* start, node* goal, int caps, int agentsize)
{
	if((caps = curexp->caps) && (agentsize == curexp->size))
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


/* we do some selective annotation of nodes depending on the test experiment being executed. 
For example, when running kPathableMoveNorthWestLST to test AnnotatedAStar::evaluate we need to check if the neighbouring nodes of the
start position are traversable. This mock class does not annotate the entire map like AnnotatedMapAbstraction, yet we still need some annotations.
So, we selectively annotate for each experiment as required in order to rig the right result. 

Part of the reason for doing this is that we don't have objects that mock the behaviour of functions in mapAbstraction. Our mock
is at a higher level because we want to avoid changing the HOG codebase. So, we assume HOG's stuff works OK but we add extra stuff to 
somehow mimick the behavior of the classes the object being tested is interacting with. Hence, this.
*/

void AnnotatedMapAbstractionMock::setCurrentTestExperiment(TestExperiment* exp)
{
	this->curexp = exp;
	switch(exp->key)
	{
		case kPathableMoveNorthWestLST: 
			this->getNodeFromMap(exp->startx,exp->starty-1)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx,exp->starty-1)->setClearance(exp->sterrain, exp->size); // north neighbour
			this->getNodeFromMap(exp->startx-1,exp->starty)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx-1,exp->starty)->setClearance(exp->sterrain, exp->size); // west neighbour
			
		case kPathableMoveNorthEastLST: 
			this->getNodeFromMap(exp->startx,exp->starty-1)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx,exp->starty-1)->setClearance(exp->sterrain, exp->size); // north neighbour
			this->getNodeFromMap(exp->startx+1,exp->starty)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx+1,exp->starty)->setClearance(exp->sterrain, exp->size); // east neighbour
			
		case kPathableMoveSouthEastLST: 
			this->getNodeFromMap(exp->startx,exp->starty+1)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx,exp->starty+1)->setClearance(exp->sterrain, exp->size); // south neighbour
			this->getNodeFromMap(exp->startx+1,exp->starty)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx+1,exp->starty)->setClearance(exp->sterrain, exp->size); // east neighbour

		case kPathableMoveSouthWestLST: 
			this->getNodeFromMap(exp->startx,exp->starty+1)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx,exp->starty+1)->setClearance(exp->sterrain, exp->size); // south neighbour
			this->getNodeFromMap(exp->startx-1,exp->starty)->setTerrainType(exp->sterrain);
			this->getNodeFromMap(exp->startx-1,exp->starty)->setClearance(exp->sterrain, exp->size); // west neighbour

	}

}

void AnnotatedMapAbstractionMock::annotateMap()
{
	AnnotatedMapAbstraction* theMap = ((AnnotatedMapAbstraction*)this);
	loadClearanceInfo(string(this->getMap()->getMapName()), theMap);
}

void AnnotatedMapAbstractionMock::loadClearanceInfo(const string& filename, AnnotatedMapAbstraction* aMap)
{
	if(filename.compare("annotatedcluster.map"))
	{
		int clearance[6][9] = 
			{	
				{303, 303, 303, 202, 101, 000, 000, 000, 000}, 
				{202, 202, 202, 202, 202, 202, 202, 101, 000}, 
				{102, 101, 101, 101, 101, 101, 202, 101, 000}, 
				{102, 011, 000, 000, 000, 000, 202, 101, 000},
				{102, 101, 000, 102, 202, 202, 202, 101, 000},
				{101, 011, 101, 011, 101, 101, 101, 101, 000}, 
			};
					
		for(int x=0; x<aMap->getMap()->getMapWidth(); x++)
			for(int y=0; y<aMap->getMap()->getMapHeight(); y++)
			{
				node* current = aMap->getNodeFromMap(x,y);
				current->setTerrainType(aMap->getMap()->getTerrainType(x,y));
				int kGroundClearance = clearance[y][x]/100;
				int kTreesAndGroundClearance = clearance[y][x]%10;
				int kTreesClearance = (clearance[y][x]-kGroundClearance*100-kTreesAndGroundClearance)/10;
				current->setClearance(kGround, kGroundClearance);
				current->setClearance(kTrees, kTreesClearance);
				current->setClearance((kTrees|kGround), kTreesAndGroundClearance);
			}
	}
}
