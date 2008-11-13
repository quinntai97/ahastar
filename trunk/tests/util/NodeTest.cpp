/*
 *  NodeTest.cpp
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeTest.h"
#include "constants.h"
#include "graph.h"

CPPUNIT_TEST_SUITE_REGISTRATION( NodeTest );

void NodeTest::setUp()
{

}

void NodeTest::tearDown()
{

}

void NodeTest::cloneShouldDeepCopyNodeAndAllLabels()
{
	this->n = new node("test");
	for(int i=0; i<11; i++)
	{
		n->setLabelL(i, 42);
	}
	n->setKeyLabel(2);
	n->setWidth(42);
	
	node *n2 = dynamic_cast<node*>(n->clone()); 
	for(int i=0; i<11; i++)
	{
		CPPUNIT_ASSERT_EQUAL_MESSAGE("labels of cloned node do not match original", n->getLabelL(i), n2->getLabelL(i));
	}

	CPPUNIT_ASSERT_EQUAL_MESSAGE("keyLabels of cloned node do not match original", n->getKey(), n2->getKey());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("width of cloned node do not match original", n->getWidth(), n2->getWidth());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("name of cloned node do not match original", 0, strcmp(n->getName(), n2->getName()));
	
	delete n;
	delete n2;
}

void NodeTest::cloneShouldNotDeepCopyEdges()
{
	int numExpectedEdges = 0;
	this->n = new node("test");
	node* n2 = new node("test2");
	
	edge* e = new edge(n->getNum(), n2->getNum(), 1.0);
	n->addEdge(e);
	n2->addEdge(e);
	
	node* testme = dynamic_cast<node*>(n->clone());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node contains edges (there should be none)", numExpectedEdges, testme->getNumEdges());
	delete n;
	delete n2;
	delete testme;
}

void NodeTest::copyConstructorShouldDeepCopyNodeAndAllLabels()
{
	this->n = new node("test");
	int label=42;
	for(int i=0; i<11; i++)
	{
		n->setLabelL(i, label);
		label++;
	}
	n->setKeyLabel(2);
	n->setWidth(42);
	
	node *n2 = new node(n); 
	for(int i=0; i<11; i++)
	{
		CPPUNIT_ASSERT_EQUAL_MESSAGE("labels of cloned node do not match original", n->getLabelL(i), n2->getLabelL(i));
	}

	CPPUNIT_ASSERT_EQUAL_MESSAGE("keyLabels of cloned node do not match original", n->getKey(), n2->getKey());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("width of cloned node do not match original", n->getWidth(), n2->getWidth());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("name of cloned node do not match original", 0, strcmp(n->getName(), n2->getName()));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("name of cloned node do not match original", 0, strcmp(n->getName(), n2->getName()));
	
	delete n;
	delete n2;
}

void NodeTest::copyConstructorShouldNotDeepCopyEdges()
{
	int numExpectedEdges = 0;
	this->n = new node("test");
	node* n2 = new node("test2");
	
	edge* e = new edge(n->getNum(), n2->getNum(), 1.0);
	n->addEdge(e);
	n2->addEdge(e);
	
	node* testme = new node(n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cloned node contains edges (there should be none)", numExpectedEdges, testme->getNumEdges());
	delete n;
	delete n2;
	delete testme;
}