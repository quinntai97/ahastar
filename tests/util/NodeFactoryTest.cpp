/*
 *  NodeFactoryTest.cpp
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeFactoryTest.h"
#include "NodeFactory.h"
#include "graph.h"

CPPUNIT_TEST_SUITE_REGISTRATION( NodeFactoryTest );

void NodeFactoryTest::setUp()
{
	nf = new NodeFactory();
}

void NodeFactoryTest::tearDown()
{
	delete nf;
	nf = 0;
}

void NodeFactoryTest::newNodeShouldConstructANewNodeObject()
{
	char* name = "testnode";
	node* n = nf->newNode(name);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to create new node", true, n != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("created node has wrong name", 0, strcmp(n->getName(), name));
	delete n;
}

void NodeFactoryTest::newNodeShouldConstructANewNodeObjectGivenAnotherNodeAsAParameter()
{
	node n("testnode");
	node* testme = nf->newNode(&n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to create new node", true, testme != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("created node has wrong name", 0, strcmp(n.getName(), testme->getName()));
	delete testme;

}