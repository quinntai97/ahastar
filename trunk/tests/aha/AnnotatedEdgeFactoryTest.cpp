/*
 *  AnnotatedAnnotatedEdgeFactoryTest.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedEdgeFactoryTest.h"
#include "AnnotatedEdgeFactory.h"
#include "AnnotatedEdge.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedEdgeFactoryTest );

void AnnotatedEdgeFactoryTest::setUp()
{
	aef = new AnnotatedEdgeFactory();
}

void AnnotatedEdgeFactoryTest::tearDown()
{
	delete aef;
	aef = 0;
}

void AnnotatedEdgeFactoryTest::newEdgeShouldConstructANewAnnotatedEdgeObject()
{
	int from=0;
	int to=1;
	double weight = 3.4;
	AnnotatedEdge* n = 0;
	n = dynamic_cast<AnnotatedEdge*>(aef->newEdge(from, to, weight));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to create new AnnotatedEdge", true, n != 0);
	delete n;
}