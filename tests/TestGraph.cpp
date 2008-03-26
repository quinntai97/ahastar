/*
 *  TestGraph.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 26/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestGraph.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestGraph );

void TestGraph::setUp()
{
	g = new graph();
}

void TestGraph::tearDown()
{
	delete g;
}

void TestGraph::findAnnotatedEdgeShouldReturnAnExistingEdgeGivenACapabilityParameterWhichSupersetsTheCapabilityOfAnExistingEdge()
{
	graph *g = new graph();
	node* n = new node("");
	node* tn1 = new node("");
	g->addNode(n);
	g->addNode(tn1);
	edge* e = new edge(n->getNum(), tn1->getNum(), 1.0);
	e->setClearance(kGround,2);
	g->addEdge(e);
		
	int targetCapability = (kTrees|kGround);
	edge* expectedEdge = g->findAnnotatedEdge(n, tn1, targetCapability, 2);
	
	edge* e2 = new edge(n->getNum(), tn1->getNum(), 1.0);
	e->setClearance(kGround,3);
	g->addEdge(e);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find existing annotated edge", e, expectedEdge);
	CPPUNIT_ASSERT_EQUAL(2, g->getNumEdges());
}