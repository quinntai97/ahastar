/*
 *  AnnotatedClusterAbstractionTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 21/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstractionTest.h"
#include "ExperimentManager.h"
#include "Map.h"
#include "AnnotatedClusterAbstraction.h"
#include "clusterAbstraction.h"
#include "AnnotatedCluster.h"
#include "AnnotatedAStarMock.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterAbstractionTest );

using namespace std;

void AnnotatedClusterAbstractionTest::setUp()
{
	testmap = new Map(maplocation.c_str()); // TODO: need a separate, larger map to test this junk
	aca = new AnnotatedClusterAbstraction(testmap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);
	expmgr = new ExperimentManager();
	
	n1 = aca->getNodeFromMap(2,1);
	n1->setParentCluster(0);
	n2 = aca->getNodeFromMap(5,1);
	n2->setParentCluster(1);
	n3hardobst = aca->getNodeFromMap(0,0);	
}

void AnnotatedClusterAbstractionTest::tearDown()
{
	delete aca;
	delete expmgr;
}

void AnnotatedClusterAbstractionTest::constructorShouldSplitTheMapAreaIntoCorrectNumberOfClusters()
{
	/* figure out how many clusters the test map should have */
	int mapwidth = aca->getMap()->getMapWidth();
	int mapheight = aca->getMap()->getMapHeight();
	
	int numHorizontalClusters = mapwidth / aca->getClusterSize();
	if(mapwidth % aca->getClusterSize() > 0)
		numHorizontalClusters++;
		
	int numVerticalClusters = mapheight / aca->getClusterSize();
	if(mapheight % aca->getClusterSize() > 0)
		numVerticalClusters++;
		
	int totalExpectedClusters = numHorizontalClusters*numVerticalClusters;

	/* check for the correct # of clusters */
	CPPUNIT_ASSERT_EQUAL(totalExpectedClusters, aca->getNumClusters());
}

void AnnotatedClusterAbstractionTest::getClusterSizeShouldReturnSameValueAsConstructorParameter()
{
	CPPUNIT_ASSERT_EQUAL(TESTCLUSTERSIZE, aca->getClusterSize());
}

void AnnotatedClusterAbstractionTest::buildAbstractGraphShouldCreateANewGraphObject()
{
	int numGraphsBefore = aca->getNumberOfAbstractionLevels();
	aca->buildAbstractGraph();
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("actual graph count does not match expected count", numGraphsBefore+1, aca->getNumberOfAbstractionLevels());
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldAddTwoAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph()
{
	int expectedNodesInAbstractGraph=2;

	// nb: deletion responsibility is taken over by the graph object to which we add these nodes
	node* tn1 = new node("");
	node* tn2 = new node("");
	tn1->setTerrainType(kGround);
	tn1->setClearance(kGround, 1);
	tn1->setLabelL(kAbstractionLevel, 0);
	tn1->setParentCluster(0);

	tn2->setTerrainType(kGround);
	tn2->setClearance(kGround, 1);
	tn2->setLabelL(kAbstractionLevel, 0);
	tn2->setParentCluster(1);


	
	aca->buildAbstractGraph();
	aca->addEntranceToGraph(tn1, tn2);
		
	CPPUNIT_ASSERT_EQUAL(expectedNodesInAbstractGraph, aca->getAbstractGraph(1)->getNumNodes());
	
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull()
{
	aca->buildAbstractGraph();
	bool exceptionThrown = false;
	
	try
	{
		aca->addEntranceToGraph(NULL, n1);
	}
	catch(EntranceNodeIsNullException)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull()
{
	aca->buildAbstractGraph();
	bool exceptionThrown = false;
	
	try
	{
		aca->addEntranceToGraph(n1, NULL);
	}
	catch(EntranceNodeIsNullException)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject()
{
	aca->buildAbstractGraph();
	bool exceptionThrown = false;
	try
	{
		aca->addEntranceToGraph(n1, n1);
	}
	catch(EntranceNodesAreIdenticalException)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	bool exceptionThrown = false;
	
	n1->setLabelL(kAbstractionLevel, 1);
	n2->setLabelL(kAbstractionLevel, 2);

	aca->buildAbstractGraph();
	
	try 
	{
		aca->addEntranceToGraph(n1, n2);
	}
	catch(CannotBuildEntranceFromAbstractNodeException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	bool exceptionThrown = false;
	
	n1->setLabelL(kAbstractionLevel, 0);
	n2->setLabelL(kAbstractionLevel, 1);
	aca->buildAbstractGraph();
	
	try 
	{
		aca->addEntranceToGraph(n1, n2);
	}
	catch(CannotBuildEntranceFromAbstractNodeException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster()
{
	bool exceptionThrown = false;
	
	n1->setParentCluster(0);
	n2->setParentCluster(0);
	aca->buildAbstractGraph();
	
	try 
	{
		aca->addEntranceToGraph(n1, n2);
	}
	catch(CannotBuildEntranceToSelfException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
	
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle()
{
	bool exceptionThrown = false;
		
	aca->buildAbstractGraph();
	
	try 
	{
		aca->addEntranceToGraph(n3hardobst, n1);
	}
	catch(EntranceNodeIsHardObstacleException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
	
}

void AnnotatedClusterAbstractionTest::addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle()
{
	bool exceptionThrown = false;
		
	aca->buildAbstractGraph();
	
	try 
	{
		aca->addEntranceToGraph(n1, n3hardobst);
	}
	catch(EntranceNodeIsHardObstacleException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
	
}
