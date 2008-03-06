/*
 *  AnnotatedClusterTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 27/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterTest.h"
#include "TestConstants.h"
#include "AnnotatedCluster.h"
#include "AnnotatedMapAbstractionMock.h"
#include "AnnotatedAStarMock.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterTest );

void AnnotatedClusterTest::setUp()
{
	cwidth = 5;
	cheight = cwidth;
	ama_mock = new AnnotatedMapAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock());
	ama_mock->loadClearanceInfo(acmap);
}

void AnnotatedClusterTest::tearDown()
{
	delete ama_mock;
	if(ac)
		delete ac;
}

void AnnotatedClusterTest::addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions()
{
	int startx=0; int starty=0;
	int expectedTotalNodes = cwidth*cheight;
	ac = new AnnotatedCluster(startx, starty, cwidth, cheight);
	
	ac->addNodesToCluster(ama_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not match expectations", expectedTotalNodes, ac->getNumNodes());	
	
	delete ac;
	ac = 0;
}

void AnnotatedClusterTest::addNodeShouldIncrementByOneTotalNodesInCluster()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = 1;
	ac = new AnnotatedCluster(0, 0, cwidth, cheight);
	node* targetnode = ama_mock->getNodeFromMap(x,y);
	bool result = ac->addNode(ama_mock->getNodeFromMap(x,y));
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to return true when adding a traversable node to the cluster", true, result);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not meet expected total", expectedTotalNodes, ac->getNumNodes());
}

void AnnotatedClusterTest::addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = 1;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	node* targetnode = ama_mock->getNodeFromMap(x,y);
	ac->addNode(ama_mock->getNodeFromMap(x,y));
	
	CPPUNIT_ASSERT_EQUAL(ac->getClusterId(), targetnode->getParentCluster());
}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster()
{

	int x = 0; int y = 0;
	int expectedTotalNodes = 0;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	
	node* toAssign = ama_mock->getNodeFromMap(1,1); // pick a node, any node and assign it to some cluster 
	toAssign->setParentCluster(10);

	
	try
	{
		ac->addNode(toAssign);
	}
	catch(NodeIsAlreadyAssignedToClusterException e)
	{
		exceptionThrown = true;
	}

	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception when adding an already assigned node to cluster", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total nodes count unexpectedly changed while trying to add already assigned node to cluster",expectedTotalNodes, ac->getNumNodes());

}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenClusterIsFull()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	
	/* create some test data and fill the cluster */
	for(int i=0; i<expectedTotalNodes; i++)
	{
		node* newnode = new node("");
		newnode->setTerrainType(kGround);
		newnode->setClearance(kGround,1);
		ac->addNode(newnode);
		delete newnode;
	}
	try
	{
		node* toAssign = ama_mock->getNodeFromMap(1,1); // pick a node, any node
		ac->addNode(toAssign);
	}
	catch(ClusterFullException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception when adding node to a full cluster", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total nodes count unexpectedly changed while trying to add node to a full cluster",expectedTotalNodes, ac->getNumNodes());
	
	

}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenNodeParameterIsNull()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);

	try
	{
		ac->addNode(NULL);
	}
	catch(NodeIsNullException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);	
}

void AnnotatedClusterTest::addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);

	try
	{
		ac->addNodesToCluster(NULL);
	}
	catch(AnnotatedMapAbstractionIsNullException e)
	{
		exceptionThrown = true;
	}

	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid()
{

	bool exceptionThrown = false;
	try
	{
		ac = new AnnotatedCluster(0,0,0,1);
	}
	catch(InvalidClusterDimensionsException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid()
{

	bool exceptionThrown = false;
	try
	{
		ac = new AnnotatedCluster(0,0,1,0);
	}
	catch(InvalidClusterDimensionsException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenXOriginParameterIsInvalid()
{
	bool exceptionThrown = false;
	try
	{
		ac = new AnnotatedCluster(-1,0,1,1);
	}
	catch(InvalidClusterOriginCoordinatesException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}
void AnnotatedClusterTest::constructorShouldThrowExceptionWhenYOriginParameterIsInvalid()
{
	bool exceptionThrown = false;
	try
	{
		ac = new AnnotatedCluster(0,-1,1,1);
	}
	catch(InvalidClusterOriginCoordinatesException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}


