/*
 *  ClusterNodeTest.cpp
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterNodeTest.h"
#include "constants.h"


CPPUNIT_TEST_SUITE_REGISTRATION( ClusterNodeTest );

void ClusterNodeTest::setUp()
{

}

void ClusterNodeTest::tearDown()
{

}

void ClusterNodeTest::cloneReturnsACopyOfTheCurrentNode()
{
	ClusterNode n("test");
	n.setParentClusterId(42);
	ClusterNode* testme = dynamic_cast<ClusterNode*>(n.clone());
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node ids do not match", n.getNum(), testme->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("parent cluster ids do not match", n.getParentClusterId(), testme->getParentClusterId());
	delete testme;
}

void ClusterNodeTest::copyConstructorReturnsACopyOfTheCurrentNode()
{
	ClusterNode n("test");
	n.setParentClusterId(42);
	ClusterNode* testme = new ClusterNode(&n);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node ids do not match", n.getNum(), testme->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("parent cluster ids do not match", n.getParentClusterId(), testme->getParentClusterId());
	delete testme;
}

void ClusterNodeTest::constructorInitialises_kParent_Label()
{
	ClusterNode n("test");
	long expectedValue = -1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("kParent label not initialised.", expectedValue, n.getLabelL(kParent));
	
}
