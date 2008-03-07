/*
 *  TestNode.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 28/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestNode.h"
#include "constants.h"
#include <cppunit/Message.h>
#include <string>

using namespace std;
//using namespace CppUnit;

CPPUNIT_TEST_SUITE_REGISTRATION( TestNode );

void TestNode::setUp()
{
	terrains[0] = kGround;
	terrains[1] = kTrees;
	terrains[2] = (kTrees|kGround);
	
	for(int i=0;i<4;i++)
	{
		clval[i] = i;
	}
	
	n = new node("");
}

void TestNode::tearDown()
{
	delete n;
}

/* TerrainAnnotationsTest
	@desc:		 
		This test assumes a simple map with only two basic terrains; each node thus has a max of 3 annotations. 
*/
void TestNode::TerrainAnnotationsTest()
{
	
	// check that the terrain types are only of the defined types
	n->setTerrainType(kGround);
	CPPUNIT_ASSERT_MESSAGE("invalid terrain type",
	n->getTerrainType() == terrains[0] || n->getTerrainType() == terrains[1] || n->getTerrainType() == terrains[2]);

	for(int i=0;i<3;i++)
	{
	
		// check that clearance values are initialised to zero
		CPPUNIT_ASSERT_MESSAGE("clearance initialised to non-zero value", n->getClearance(terrains[i]) == 0);

		// check that we can set and retrieve clearance values
		n->setClearance(terrains[i], clval[i+1]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("retrieved clearance != value set earlier", n->getClearance(terrains[i]), clval[i+1]);
		
		
	}
		
	// check that negative clearance values are not allowed and zeroed instead
	n->setClearance(terrains[0], -1);
	CPPUNIT_ASSERT_MESSAGE("clearance value < 0", n->getClearance(terrains[0])  >= 0);
	
	int i =0;
	return;
}

void TestNode::setClearanceFailsWhenNodeTerrainNotValid()
{
	n->setTerrainType(kWater);
	n->setClearance(kGround, 1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("managed to set clearance > 0 for node with invalid terrain (kWater)", 0, n->getClearance(kGround));
}

void TestNode::setParentClusterFailsWhenClusterIdLessThanZero()
{
	int cid = -2;
	n->setParentCluster(cid);
	CPPUNIT_ASSERT_EQUAL(-1, n->getParentCluster());
}

void TestNode::setParentClusterStoresClusterIdWhenClusterIdMoreThanZero()
{
	int cid = 2;
	n->setParentCluster(cid);
	CPPUNIT_ASSERT_EQUAL(cid, n->getParentCluster());	
}

void TestNode::setParentClusterStoresClusterIdWhenClusterIdEqualsZero()
{
	int cid = 0;
	n->setParentCluster(cid);
	CPPUNIT_ASSERT_EQUAL(cid, n->getParentCluster());	
}

void TestNode::cloneShouldDeepCopyNodeAndAllAnnotations()
{

	/* create and annotate some nodes to use as test data */
	node* tn1 = new node("");
	
	tn1->setLabelL(kAbstractionLevel, 0);
	tn1->setParentCluster(0);
	tn1->setLabelL(kFirstData, 4);
	tn1->setLabelL(kFirstData+1, 1);	
	tn1->setTerrainType(kGround);
	tn1->setClearance(kGround, 1);
	tn1->setClearance(kTrees, 1);
	tn1->setClearance(kGround|kTrees, 2);

	node* tn1clone = dynamic_cast<node*>(tn1->clone());
	CPPUNIT_ASSERT_MESSAGE("clone returned original node! failed to copy!", tn1 != tn1clone);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has different terrain type", tn1->getTerrainType(), tn1clone->getTerrainType());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has different parent cluster", tn1->getParentCluster(), tn1clone->getParentCluster());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has different clearance value for kTrees capability", tn1->getClearance(kTrees), tn1clone->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has different clearance value for kTrees capability", tn1->getClearance(kTrees), tn1clone->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has different clearance value for (kGround|kTrees) capability", tn1->getClearance((kGround|kTrees)), tn1clone->getClearance((kGround|kTrees)));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has incorrect kAbstractionLevel label", tn1->getLabelL(kAbstractionLevel), tn1clone->getLabelL(kAbstractionLevel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has incorrect x-coordinate label", tn1->getLabelL(kFirstData), tn1clone->getLabelL(kFirstData));	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node has incorrect y-coordinate label", tn1->getLabelL(kFirstData+1), tn1clone->getLabelL(kFirstData+1));

}
