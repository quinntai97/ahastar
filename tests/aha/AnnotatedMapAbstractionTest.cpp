/*
 *  AnnotatedMapTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 3/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedMapAbstractionTest.h"
#include "TestConstants.h"
#include "AHAConstants.h"
#include "ExperimentManager.h"

#include "AnnotatedAStarMock.h"
#include "mapAbstraction.h"
#include "map.h"
#include "aStar3.h"


//CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedMapAbstractionTest );

using namespace std;
using namespace ExpMgrUtil;

void AnnotatedMapAbstractionTest::setUp()
{
	expmgr = new ExperimentManager();
	
	// need to setup a map
	testmap = new Map(maplocation.c_str());
	AnnotatedAStarMock* aastar_mock = new AnnotatedAStarMock();
	ama = new AnnotatedMapAbstraction(testmap, aastar_mock);
	g = ama->getAbstractGraph(0);
		
	string targetmap(HOGHOME+"maps/local/demo.map");
	assert(maplocation == targetmap); //make sure we're loading data for the right map

	nclearance=1;	
}

void AnnotatedMapAbstractionTest::tearDown()
{
	delete ama; // also kills testmap
	delete expmgr;
}

/*	runExperiment

	Load some data (pathable known + params known) & compare result with what comes out of pathable 
*/
void AnnotatedMapAbstractionTest::runExperiment(ExperimentKey expkey)
{	
	TestExperiment* exp = expmgr->getExperiment(expkey);
	AnnotatedAStarMock* aastarmock = ((AnnotatedAStarMock*)ama->getSearchAlgorithm());

	aastarmock->setCapability(exp->caps);
	aastarmock->setClearance(exp->size);
	aastarmock->setCurrentTestExperiment(exp);

	node* start = ama->getNodeFromMap(exp->startx, exp->starty);
	node* goal = ama->getNodeFromMap(exp->goalx, exp->goaly);
	CPPUNIT_ASSERT_EQUAL(ama->pathable(start, goal, exp->caps, exp->size), exp->pathable);


}

void AnnotatedMapAbstractionTest::PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent()
{
	runExperiment(kNotPathableGoalIsSoftObstacleLST);
}

void AnnotatedMapAbstractionTest::PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent()
{	
	runExperiment(kPathableToyProblemLST);
}

void AnnotatedMapAbstractionTest::ValidateAnnotationsTest() 
{
	stringstream ss;
	for(int x=0; x<testmap->getMapWidth(); x++) 
	{
		for(int y=0; y<testmap->getMapHeight(); y++)
		{ 
			node *n = ama->getNodeFromMap(x,y);
			if(n)
				checkSingleNodeAnnotations(n, x, y);
		}
	}
}

void AnnotatedMapAbstractionTest::nodeClearanceIsZeroForCapabilityThatDoesNotIncludeTheNodeTerrainType()
{
	int nx=1;
	int ny=2;
	node* target = ama->getNodeFromMap(nx,ny); // node with kGround terrain and kGround neighbours
	setupNeighbourAnnotations(nx, ny);
	ama->annotateNode(target); 

	CPPUNIT_ASSERT_EQUAL(0, target->getClearance(kTrees)); // no annotations for kTrees capability
}

void AnnotatedMapAbstractionTest::nodeClearanceZeroWhenTerrainIsHardObstacle()
{
	int nx=1;
	int ny=1;
	node* target = ama->getNodeFromMap(nx,ny); // node with kWater terrain and kGround neighbours
	setupNeighbourAnnotations(nx, ny);
	ama->annotateNode(target); 

	/* node is not traversable by anything */
	CPPUNIT_ASSERT_EQUAL(0, target->getClearance(kGround));
	CPPUNIT_ASSERT_EQUAL(0, target->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL(0, target->getClearance((kGround|kTrees)));
}

void AnnotatedMapAbstractionTest::nodeClearanceMinimumClearanceWhenTerrainIsValidButNoNeighbours()
{
	int nx=23;
	int ny=9;
	node* target = ama->getNodeFromMap(nx,ny); // node with kGround terrain but no neighbours
	ama->annotateNode(target); 

	/* test if the node is traversable by an agent with capability that includes kGround terrain */
	CPPUNIT_ASSERT_EQUAL(nclearance, target->getClearance(kGround));
	CPPUNIT_ASSERT_EQUAL(nclearance, target->getClearance((kGround|kTrees)));
}

void AnnotatedMapAbstractionTest::nodeClearanceMinimumClearanceWhenTerrainIsValidButAllNeighboursHardObstacles()
{
	int nx=23;
	int ny=2;
	node* target = ama->getNodeFromMap(nx,ny); // node with kGround terrain & all kWater neighbours
	setupNeighbourAnnotations(nx, ny);
	ama->annotateNode(target); 

	/* test if the node is traversable by an agent with capability that includes kGround terrain */
	CPPUNIT_ASSERT_EQUAL(nclearance, target->getClearance(kGround));
	CPPUNIT_ASSERT_EQUAL(nclearance, target->getClearance((kGround|kTrees)));
}

void AnnotatedMapAbstractionTest::nodeClearanceEqualsMinimumOfAllNeighboursPlusOne()
{ 
	int nx=1;
	int ny=2;
	node* target = ama->getNodeFromMap(nx,ny); // node with kGround terrain & all kGround neighbours
	setupNeighbourAnnotations(nx, ny);
	ama->annotateNode(target); 

	/* test if the node is traversable by an agent with capability that includes kGround terrain */
	CPPUNIT_ASSERT_EQUAL(nclearance+1, target->getClearance(kGround));
	CPPUNIT_ASSERT_EQUAL(nclearance+1, target->getClearance((kGround|kTrees)));
}

void AnnotatedMapAbstractionTest::setupNeighbourAnnotations(int x, int y)
{
	for(int i=0; i<NUMCAPABILITIES;i++)
	{
		ama->getNodeFromMap(x+1, y)->setTerrainType(ama->getMap()->getTerrainType(x+1,y));
		ama->getNodeFromMap(x+1, y)->setClearance(capabilities[i], nclearance);

		ama->getNodeFromMap(x, y+1)->setTerrainType(ama->getMap()->getTerrainType(x,y+1));
		ama->getNodeFromMap(x, y+1)->setClearance(capabilities[i], nclearance);

		ama->getNodeFromMap(x+1, y+1)->setTerrainType(ama->getMap()->getTerrainType(x+1,y+1));
		ama->getNodeFromMap(x+1, y+1)->setClearance(capabilities[i], nclearance);
	}
}


void AnnotatedMapAbstractionTest::checkSingleNodeAnnotations(node* n, int x, int y)
{
			int nid = n->getNum(); //NB: should change to getUniqueId() but then getNode needs to switch to find or graphs use hashtables to store nodes/edges
		
			/* Test 1: check that missing edges have been re-added */
			node *nadj = ama->getNodeFromMap(x-1,y-1);
			if(nadj) 
				CPPUNIT_ASSERT_MESSAGE("expected edge; none found", g->findEdge(nid,nadj->getNum()) != 0);
			
			nadj = ama->getNodeFromMap(x,y-1);
			if(nadj)
				CPPUNIT_ASSERT_MESSAGE("expected edge; none found",g->findEdge(nid,nadj->getNum()) != 0);
				
			nadj = ama->getNodeFromMap(x-1,y);
			if(nadj)
				CPPUNIT_ASSERT_MESSAGE("expected edge; none found",g->findEdge(nid,nadj->getNum()) != 0);
			

			/* Test 2: validate clearance value annotations for each node. */
			int clVals[4];
			node* nadj2, *nadj3;

			nadj = ama->getNodeFromMap(x+1,y);
			nadj2 = ama->getNodeFromMap(x,y+1);
			nadj3 = ama->getNodeFromMap(x+1,y+1);
			
			/* check if node is traversable (anything not in the conjunction @ capabilities[3] is considered nontraversable & has clearance=0 */
			int nterrain = n->getTerrainType();
			if(nterrain&capabilities[3] != nterrain)
				CPPUNIT_ASSERT(clVals[0] == 0);
			else
				for(int i=0;i<NUMCAPABILITIES;i++) 
				{ 
					if(nadj && nadj2 && nadj3) // clearance value is dependent on clearance values of 3 neighbours
					{		
							clVals[0] = n->getClearance(capabilities[i]);
							clVals[1] = nadj->getClearance(capabilities[i]);
							clVals[2] = nadj2->getClearance(capabilities[i]);
							clVals[3] = nadj3->getClearance(capabilities[i]);

							// clearace >0 for combinations of terrain that include the basic (single) terrain type of the node
							if((capabilities[i]&n->getTerrainType())==n->getTerrainType()) 
							{
								/* get minimum clearance amongst neighbours */
								int min=MAXINT;
								for(int j=1;j<4;j++)
									if(clVals[j] < min)
										min = clVals[j];
								min++; // clearance = min among neighbours + base clearance (1); so we increment min
									
								CPPUNIT_ASSERT(((clVals[0] > 0) && (clVals[0] < MAXINT)));
								CPPUNIT_ASSERT(clVals[0] == min); 
							}
							else
								CPPUNIT_ASSERT(clVals[0] == 0);

							
							
									
					}
					else // some neighbours don't exist; check for min clearance
						CPPUNIT_ASSERT_EQUAL(/*getMessage(ss << "invalid clearance for tile @ "<<x<<","<<y<<". terrain "<<capabilities[i]),*/ n->getClearance(capabilities[i]), 1);
				}

		/* test that we've initialised parent labels */
		CPPUNIT_ASSERT_EQUAL_MESSAGE("parent label default value not default", (int)n->getLabelL(kParent), -1);
}

void AnnotatedMapAbstractionTest::checkNodeAnnotationsAgainstExpectations()
{
		delete ama;
		Map* m = new Map(acmap.c_str());
		ama = new AnnotatedMapAbstraction(m, new AnnotatedAStarMock());
						
		int clearance[6][9] = 
		{	
			{303, 303, 303, 202, 101, 000, 000, 000, 000}, 
			{202, 202, 202, 202, 202, 202, 202, 101, 000}, 
			{102, 101, 101, 101, 101, 101, 202, 101, 000}, 
			{102, 11, 000, 000, 000, 000, 202, 101, 000},
			{102, 101, 000, 102, 202, 202, 202, 101, 000},
			{101, 11, 101, 11, 101, 101, 101, 101, 000}, 
		};
					
		for(int x=0; x<ama->getMap()->getMapWidth(); x++)
			for(int y=0; y<ama->getMap()->getMapHeight(); y++)
			{
				node* current = ama->getNodeFromMap(x,y);	
				int kGroundClearance = clearance[y][x]/100;
				int kTreesAndGroundClearance = clearance[y][x]%10;
				int kTreesClearance = (clearance[y][x]-kGroundClearance*100-kTreesAndGroundClearance)/10;
				
				if(kGroundClearance != current->getClearance(kGround))
					cout << "\nannotation match failed at nodepos: "<<current->getLabelL(kFirstData)<<","<<current->getLabelL(kFirstData+1);
				if(kTreesAndGroundClearance != current->getClearance((kGround|kTrees)))
					cout << "\nkG|kT annotation match failed at nodepos: "<<current->getLabelL(kFirstData)<<","<<current->getLabelL(kFirstData+1);
				if(kTreesClearance != current->getClearance(kTrees))
					cout << "\nannotation match failed at nodepos: "<<current->getLabelL(kFirstData)<<","<<current->getLabelL(kFirstData+1);

				CPPUNIT_ASSERT_EQUAL_MESSAGE("kGround annotations do not match expectations ", kGroundClearance, current->getClearance(kGround));
				CPPUNIT_ASSERT_EQUAL_MESSAGE("kGround|kTrees annotations do not match expectations ", kTreesAndGroundClearance, current->getClearance((kGround|kTrees)));
				CPPUNIT_ASSERT_EQUAL_MESSAGE("kTrees annotations do not match expectations ", kTreesClearance, current->getClearance(kTrees));
			}
}

