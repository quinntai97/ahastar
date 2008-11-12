/*
 *  HPAClusterTest.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterTest.h"
#include "graph.h"
#include "constants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HPAClusterTest );

void HPAClusterTest::setUp()
{

}

void HPAClusterTest::tearDown()
{

}

void HPAClusterTest::hasaParentReturnsTrueGivenANodeWithTheSameCoordinatesAsTheParameterNode()
{
	HPACluster cluster(0,0,0,0);
	node n("test");
	int x=1;
	int y=1;
	n.setLabelL(kFirstData, x);
	n.setLabelL(kFirstData+1, y);
	cluster.parents[n.getUniqueID()] = &n;
	
	node* n2 = dynamic_cast<node*>(n.clone());
	bool expectedVal = true;
	bool actualVal = cluster.hasaParent(n2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong return value", expectedVal, actualVal); 
	delete n2;
}

void HPAClusterTest::hasaParentReturnsFalseGivenANodeWithDifferentCoordinatesFromTheParameterNode()
{
	HPACluster cluster(0,0,0,0);
	node n("test");
	int x=1;
	int y=1;
	n.setLabelL(kFirstData, x);
	n.setLabelL(kFirstData+1, y);
	cluster.parents[n.getUniqueID()] = &n;	
	node* n2 = new node("test2");
	bool expectedVal = false;
	bool actualVal = cluster.hasaParent(n2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong return value", expectedVal, actualVal); 
	delete n2;
}
