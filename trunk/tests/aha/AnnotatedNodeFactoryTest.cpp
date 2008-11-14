/*
 *  AnnotatedAnnotatedNodeFactoryTest.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedNodeFactoryTest.h"
#include "AnnotatedNodeFactory.h"
#include "AnnotatedNode.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedNodeFactoryTest );

void AnnotatedNodeFactoryTest::setUp()
{
	anf = new AnnotatedNodeFactory();
}

void AnnotatedNodeFactoryTest::tearDown()
{
	delete anf;
	anf = 0;
}

void AnnotatedNodeFactoryTest::newNodeShouldConstructANewAnnotatedNodeObject()
{
	char* name = "testAnnotatedNode";
	AnnotatedNode* n = 0;
	n = dynamic_cast<AnnotatedNode*>(anf->newNode(name));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to create new AnnotatedNode", true, n != 0);
	delete n;
}