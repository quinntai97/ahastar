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
#include "AnnotatedAStar.h"
#include "AnnotatedAStarMock.h"
#include "AnnotatedMapAbstraction.h"
#include "AnnotatedClusterAbstractionMock.h"
#include "TestConstants.h"
#include "statCollection.h"

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
	ahastar->setCapability(kGround);
	ahastar->setClearance(2);
	e->setClearance(kGround,2);
	
	ahastar->e = e;
	bool expectedResult = true;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return true when edge connecting parameter nodes is traversable", expectedResult, actualResult);
}

void AnnotatedHierarchicalAStarTest::evaluateShouldReturnFalseIfTheEdgeIsNotTraversable()
{
	ahastar->setCapability(kGround);
	ahastar->setClearance(2);
	e->setClearance(kTrees,2);
	
	ahastar->e = e;
	bool expectedResult = false;
	bool actualResult = ahastar->evaluate(n, p); // initialised by null in AA* constructor

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate failed to return false when edge connecting parameter nodes is not traversable", expectedResult, actualResult);	
}

void AnnotatedHierarchicalAStarTest::getAbstractPathShouldFindTheShortestPathBetweenTwoAbstractNodesGivenACapabilityAndAClearanceAndAnAnnotatedClusterAbstractionParameter()
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
	
	graph* absmap = aca->getAbstractGraph(1);
	node* absstart = absmap->getNode(start->getLabelL(kParent));
	node* absgoal = absmap->getNode(goal->getLabelL(kParent));

	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
		
	path* p = ahastar->getAbstractPath(aca, absstart,absgoal);
	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);

	double expectedDist = 9.41;

	double actualDist=0;
	path *current = p;
	path* next = p->next;
	while(next)
	{
			edge *e = next->n->getMarkedEdge();
			actualDist+= e->getWeight();
			current = next;
			next = current->next;
	}
	actualDist = (int(actualDist*100+0.5))/100.0;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance wrong", expectedDist, actualDist);

	aca->removeStartAndGoalNodesFromAbstractGraph();
	delete p;
	delete acfactory;
	delete aca;
}

void AnnotatedHierarchicalAStarTest::getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodesGivenACapabilityAndClearanceAndAnnotatedClusterAbstractionParameter()
{
	Map *m = new Map(acmap.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph  *absg = aca->getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = aca->getPathCacheSize();
	
	node *start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(4,5);
	
	int capability = kGround;
	int size = 1;

	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);

	path* p = ahastar->getPath(aca, start,goal);	
	
	int expectedLength = 10;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path length wrong", expectedLength, (int)p->length());	

	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);
	CPPUNIT_ASSERT_MESSAGE("start of path is wrong", start == p->n);
	path* cur = p->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+1 in path is wrong", aca->getNodeFromMap(3,1)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+2 in path is wrong", aca->getNodeFromMap(4,1)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+3 in path is wrong", aca->getNodeFromMap(5,1)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+4 in path is wrong", aca->getNodeFromMap(6,2)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+5 in path is wrong", aca->getNodeFromMap(6,3)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+6 in path is wrong", aca->getNodeFromMap(6,4) == cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+7 in path is wrong", aca->getNodeFromMap(5,4)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+8 in path is wrong", aca->getNodeFromMap(5,5)== cur->n);
	CPPUNIT_ASSERT_MESSAGE("end of path is wrong", goal == p->tail()->n);
	
	double expectedDist =  9.41;
	double dist = aca->distance(p);
	dist = ((int)(dist*100+0.5))/100.0;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance wrong", expectedDist, dist);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in abstract graph is wrong", numNodesExpected, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count in abstract graph is wrong", numEdgesExpected, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numCachedPathsExpected, aca->getPathCacheSize());
	
	delete p;
	delete acfactory;
	delete aca;
}

void AnnotatedHierarchicalAStarTest::getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution()
{
	Map *m = new Map(acmap.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph* absmap = aca->getAbstractGraph(1);
	node *start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(2,5);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
	
	int numExpectedNodes = absmap->getNumNodes();
	int numExpectedEdges = absmap->getNumEdges();
	int numExpectedPathCacheSize = aca->getPathCacheSize();
	
	path* p = ahastar->getPath(aca, start,goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Node count in abstract graph is incorrect following call to getPath", numExpectedNodes, (int)absmap->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge count in abstract graph is incorrect following call to getPath", numExpectedEdges, (int)absmap->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Path cache size in ACA is incorrect following call to getPath", numExpectedPathCacheSize, (int)aca->getPathCacheSize());
	
	delete p;
	delete acfactory;
	delete aca;
}

void AnnotatedHierarchicalAStarTest::getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFindsASolution()
{
	Map *m = new Map(acmap.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph* absmap = aca->getAbstractGraph(1);
	node *start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(4,5);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
	
	int numExpectedNodes = absmap->getNumNodes();
	int numExpectedEdges = absmap->getNumEdges();
	int numExpectedPathCacheSize = aca->getPathCacheSize();
	
	path* p = ahastar->getPath(aca, start, goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("Node count in abstract graph is incorrect following call to getPath", numExpectedNodes, (int)absmap->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Edge count in abstract graph is incorrect following call to getPath", numExpectedEdges, (int)absmap->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Path cache size in ACA is incorrect following call to getPath", numExpectedPathCacheSize, (int)aca->getPathCacheSize());
	
	delete p;
	delete acfactory;
	delete aca;
}

void AnnotatedHierarchicalAStarTest::getPathShouldFindASolutionEvenWhenCacheReturnsAPathInReverseOrderToRequirements()
{
	Map *m = new Map(maplocation.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph* absmap = aca->getAbstractGraph(1);
	node *start = aca->getNodeFromMap(1,5);
	node* goal = aca->getNodeFromMap(16,8);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
	
	int numExpectedNodes = absmap->getNumNodes();
	int numExpectedEdges = absmap->getNumEdges();
	int numExpectedPathCacheSize = aca->getPathCacheSize();
	
	path* p = ahastar->getPath(aca, start, goal);

	CPPUNIT_ASSERT_MESSAGE("failed to find path which exists (but refinement requires reversing some cache segments)", p != NULL);
	
	
	delete p;
	delete acfactory;
	delete aca;
}

void AnnotatedHierarchicalAStarTest::getPathShouldAddInsertionEffortToPerformanceMetrics()
{
	Map *m = new Map(maplocation.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph* absmap = aca->getAbstractGraph(1);
	node *start = aca->getNodeFromMap(1,5);
	node* goal = aca->getNodeFromMap(16,8);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
		
	path* p2 = ahastar->getPath(aca, start, goal);

	CPPUNIT_ASSERT_MESSAGE("insertion effort (in nodes expanded) not recorded", ahastar->getInsertNodesExpanded() != 0);
	CPPUNIT_ASSERT_MESSAGE("insertion effort (in nodes touched) not recorded", ahastar->getInsertNodesTouched() != 0);
	CPPUNIT_ASSERT_MESSAGE("insertion effort (in peak memory) not recorded", ahastar->getInsertPeakMemory() != 0);
	CPPUNIT_ASSERT_MESSAGE("insertion effort (in search time) not recorded", ahastar->getSearchTime() != 0);
		
	CPPUNIT_ASSERT_MESSAGE("insertion effort != search effort", ahastar->getNodesExpanded() != ahastar->getInsertNodesExpanded());
	CPPUNIT_ASSERT_MESSAGE("insertion effort-search effort > 0", (ahastar->getNodesExpanded() - ahastar->getInsertNodesExpanded()) > 0);
	
	delete p2;
	delete acfactory;
	delete aca;
	
}

void AnnotatedHierarchicalAStarTest::logStatsShouldRecordAllMetricsToStatsCollection()
{
	statCollection sc;
	Map *m = new Map(maplocation.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph* absmap = aca->getAbstractGraph(1);
	node *start = aca->getNodeFromMap(1,5);
	node* goal = aca->getNodeFromMap(16,8);
	
	int capability = kGround;
	int size = 1;
	
	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);
		
	path* p = ahastar->getPath(aca, start, goal);
	assert(p != 0);
	
	ahastar->logFinalStats(&sc);
	
	statValue result;
	int lookupResult = sc.lookupStat("nodesExpanded", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)ahastar->getNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat("nodesTouched", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)ahastar->getNodesTouched(), result.lval);

	lookupResult = sc.lookupStat("peakMemory", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)ahastar->getPeakMemory(), result.lval);

	lookupResult = sc.lookupStat("searchTime", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)ahastar->getSearchTime(), result.fval);

	lookupResult = sc.lookupStat("insNodesExpanded", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)ahastar->getInsertNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat("insNodesTouched", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)ahastar->getInsertNodesTouched(), result.lval);

	lookupResult = sc.lookupStat("insPeakMemory", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)ahastar->getInsertPeakMemory(), result.lval);

	lookupResult = sc.lookupStat("insSearchTime", ahastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)ahastar->getInsertSearchTime(), result.fval);
}

void AnnotatedHierarchicalAStarTest::getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster()
{

	Map *m = new Map(acmap.c_str());
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), TESTCLUSTERSIZE);
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	graph  *absg = aca->getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = aca->getPathCacheSize();
	
	node *start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(3,0);

	int capability = kGround;
	int size = 1;

	ahastar->setGraphAbstraction(aca);
	ahastar->setClearance(size);
	ahastar->setCapability(capability);

	path* p = ahastar->getPath(aca, start,goal);	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no solution found when one exists", true, p!=0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insNodesExpanded metric unexpectedly non-zero", (long)0, ahastar->getInsertNodesExpanded());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insNodesTouched metric unexpectedly non-zero", (long)0, ahastar->getInsertNodesTouched());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insSearchTime metric unexpectedly non-zero", (double)0, ahastar->getInsertSearchTime());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insPeakMemory metric unexpectedly non-zero", (long)0, ahastar->getInsertPeakMemory());
		
	delete p;
	delete acfactory;
	delete aca;	
}
