/*
 *  AnnotatedHierarchicalAStarTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 7/04/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedHierarchicalAStarTest.h"
#include "AnnotatedHierarchicalAStar.h"
#include "AnnotatedAStarMock.h"
#include "AnnotatedClusterAbstractionMock.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedHierarchicalAStarTest );

void AnnotatedHierarchicalAStarTest::setUp()
{
	ahastar = new AnnotatedHierarchicalAStar();
	Map *m = new Map(acmap.c_str());
	acamock = new AnnotatedClusterAbstractionMock(m, new AnnotatedAStarMock(), TESTCLUSTERSIZE);
	graph* g = acamock->getAbstractGraph(1);
	n = new node("n");
	p = new node("p");
	g->addNode(n);
	g->addNode(p);
	e = new edge(n->getNum(), p->getNum(), 7);
}

void AnnotatedHierarchicalAStarTest::tearDown()
{
	delete ahastar;
	delete acamock;
}

void AnnotatedHierarchicalAStarTest::evaluateShouldReturnFalseIfEitherNodeParameterIsNull()
{
	bool expectedResult = false;
	bool actualResult = ahastar->evaluate(NULL, n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when first node is null", expectedResult, actualResult);
	
	actualResult = ahastar->evaluate(n, NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when second node is null", expectedResult, actualResult);
}

void AnnotatedHierarchicalAStarTest::evaluateShouldReturnFalseIfEdgeBeingTraversedIsNull()
{
	bool expectedResult = false;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when edge being traversed is null", expectedResult, actualResult);
}

void AnnotatedHierarchicalAStarTest::evaluateShouldReturnFalseIfEdgeDoesNotConnectParameterNodes()
{
	ahastar->e = new edge(10,11,7);
	bool expectedResult = false;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when edge being traversed does not connect parameter nodes", expectedResult, actualResult);	
}

void AnnotatedHierarchicalAStarTest::evaluteShouldReturnTrueIfTheEdgeConnectingTwoParameterNodesIsTraversable()
{
	ahastar->setSearchTerrain(kGround);
	ahastar->setMinClearance(2);
	e->setClearance(kGround,2);
	
	ahastar->e = e;
	bool expectedResult = true;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return true when edge connecting parameter nodes is traversable", expectedResult, actualResult);
}

void AnnotatedHierarchicalAStarTest::evaluateShouldReturnFalseIfTheEdgeIsNotTraversable()
{
	ahastar->setSearchTerrain(kGround);
	ahastar->setMinClearance(2);
	e->setClearance(kTrees,2);
	
	ahastar->e = e;
	bool expectedResult = false;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when edge connecting parameter nodes is not traversable", expectedResult, actualResult);	
}

void AnnotatedHierarchicalAStarTest::getPathShouldFindTheShortestPathBetweenTwoAbstractNodesGivenACapabilityAndAClearanceAndAnAnnotatedClusterAbstractionParameter()
{
	Map *m = new Map(acmap.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node *start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(4,5);
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start,goal);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setMinClearance(size);
	ahastar->setSearchTerrain(capability);
	
	graph* absmap = aca->getAbstractGraph(1);
	node* absstart = absmap->getNode(start->getLabelL(kParent));
	node* absgoal = absmap->getNode(goal->getLabelL(kParent));
	
	path* p = ahastar->getPath(aca, absstart,absgoal,capability,size);
	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);

	double expectedDist = 9.41;
	double dist = aca->distance(p);
	dist = ((int)(dist*100+0.5))/100.0;
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance wrong", expectedDist, dist);

	aca->removeStartAndGoalNodesFromAbstractGraph();
	delete p;
	delete acfactory;
	delete aca;
}


