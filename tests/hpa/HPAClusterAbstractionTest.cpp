/*
 *  HPAClusterAbstractionTest.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterAbstractionTest.h"
#include "HPAClusterAbstraction.h"
#include "TestConstants.h"
#include "HPAClusterFactory.h"
#include "NodeFactory.h"
#include "EdgeFactory.h"
#include "aStar3.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HPAClusterAbstractionTest );

void HPAClusterAbstractionTest::setUp()
{
}

void HPAClusterAbstractionTest::tearDown()
{
}

void HPAClusterAbstractionTest::buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters()
{		
	HPAClusterAbstraction hpacaMap(new Map(emptymap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	int totalExpectedClusters = 4;

	/* check for the correct # of clusters */
	hpacaMap.buildClusters();
	CPPUNIT_ASSERT_EQUAL(totalExpectedClusters, hpacaMap.getNumClusters());
}

void HPAClusterAbstractionTest::buildClustersShouldCalculateCorrectClusterSize()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	int clusterWidths[4] = {5,5,4,4};
	int clusterHeights[4] = {5,1,5,1};

	hpacaMap.buildClusters();
	cluster_iterator it = hpacaMap.getClusterIter();
	for(int i=0; i<4; i++)
	{
		HPACluster* cluster = hpacaMap.clusterIterNext(it);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster height", clusterHeights[i], cluster->getHeight());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster width", clusterWidths[i], cluster->getWidth());
	}

	
}

void HPAClusterAbstractionTest::getClusterSizeShouldReturnSameValueAsConstructorParameter()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	CPPUNIT_ASSERT_EQUAL(TESTCLUSTERSIZE, hpacaMap.getClusterSize());
}

void HPAClusterAbstractionTest::constructorShouldCreateANewGraphObject()
{	
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	int numGraphsExpected = 2;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("actual graph count does not match expected count", numGraphsExpected, hpacaMap.getNumberOfAbstractionLevels());
}

void HPAClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsLessThanZero()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	int expectedValue = 0;
	int clusterid=-1;
	HPACluster* result = hpacaMap.getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid < 0", expectedValue, (int)result);
}

void HPAClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	int expectedValue = 0;
	int clusterid=hpacaMap.getNumClusters()+1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid >= numclusters", expectedValue, (int)hpacaMap.getCluster(clusterid));
}

void HPAClusterAbstractionTest::getClusterShouldReturnRequestedClusterGivenAValidClusterId()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPACluster *cluster = new HPACluster(0,0,5,5);
	int clusterId = 0;
	cluster->setClusterId(clusterId);
	hpacaMap.clusters[clusterId] = cluster;

	HPACluster* c = hpacaMap.getCluster(0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned null instead of a cluster", true, c!=NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned wrong cluster", clusterId, c->getClusterId());
}

void HPAClusterAbstractionTest::addPathToCacheShouldStoreAPathGivenAnEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	graph* g = new graph();
	node* n1 = new node("");
	node* n2 = new node("");
	g->addNode(n1);
	g->addNode(n2);
	path* p = new path(n1, new path(n2, NULL));
		
	edge* e = new edge(p->n->getNum(), p->next->n->getNum(), 1.0);
	g->addEdge(e);
	
	hpacaMap.addPathToCache(e, p);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cache path count incorrect", 1, (int)hpacaMap.pathCache.size());
	CPPUNIT_ASSERT_MESSAGE("wrong path retrieved", p == hpacaMap.pathCache[e->getUniqueID()]);
	delete g;
}

void HPAClusterAbstractionTest::addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	path* p = new path(NULL, NULL);
	edge* e = new edge(0, 1, 1);
	
	hpacaMap.addPathToCache(NULL, p);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)hpacaMap.pathCache.size());
	
	hpacaMap.addPathToCache(e, NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)hpacaMap.pathCache.size());
	
	delete p;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnAPathGivenAValidEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	hpacaMap.addPathToCache(e,p);
	path* ret = hpacaMap.getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnInvalidEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	
	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	hpacaMap.addPathToCache(e,p);
	path* ret = hpacaMap.getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), new aStarOld(), new HPAClusterFactory(), new NodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	edge* e2 = new edge(1,1,1);
	
	hpacaMap.addPathToCache(e,p);
	
	path* ret = hpacaMap.getPathFromCache(e2);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an NULL value given an invalid edge", NULL == ret);

	delete n;
	delete e;
	delete e2;
}
