/*
 *  EdgeFactoryTest.cpp
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "EdgeFactoryTest.h"
#include "EdgeFactory.h"
#include "graph.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EdgeFactoryTest );

void EdgeFactoryTest::setUp()
{
	nf = new EdgeFactory();
}

void EdgeFactoryTest::tearDown()
{
	delete nf;
	nf = 0;
}

void EdgeFactoryTest::newEdgeShouldConstructANewEdgeObject()
{
	unsigned int fromId = 0; 
	unsigned int toId = 1;
	double weight = 2.4;
	edge* e = nf->newEdge(fromId, toId, weight);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to create new Edge", true, e != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("created Edge has wrong fromId", fromId, e->getFrom());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("created Edge has wrong toId", toId, e->getTo());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("created Edge has wrong weight", weight, e->getWeight());
}