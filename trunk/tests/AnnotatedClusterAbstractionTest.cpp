/*
 *  AnnotatedClusterAbstractionTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 21/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstractionTest.h"
#include "ExperimentManager.h"
#include "Map.h"
#include "AnnotatedClusterAbstraction.h"
#include "clusterAbstraction.h"
#include "AnnotatedAStarMock.h"

#include "AnnotatedClusterFactory.h"
#include "AnnotatedClusterMockFactory.h"
#include "AnnotatedClusterMock.h"
#include "TestConstants.h"
#include <mockpp/chaining/ChainingMockObjectSupport.h>


CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterAbstractionTest );

using namespace std;
USING_NAMESPACE_MOCKPP

void AnnotatedClusterAbstractionTest::setUp()
{
	testmap = new Map(maplocation.c_str()); // TODO: need a separate, larger map to test this junk
	aca = new AnnotatedClusterAbstraction(testmap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);
	expmgr = new ExperimentManager();
	acmock_factory = new AnnotatedClusterMockFactory(this);
}

void AnnotatedClusterAbstractionTest::tearDown()
{
	delete aca;
	delete expmgr;
}

void AnnotatedClusterAbstractionTest::setupMockClusterExpectationsForBuildClusterTests(AnnotatedClusterMock* acm)
{
	acm->addNodesToClusterMocker.expects(once()).id(MOCKPP_PCHAR("addNodesMocker"));
}

void AnnotatedClusterAbstractionTest::setupMockClusterExpectationsForBuildEntranceTests(AnnotatedClusterMock* acm)
{
	setupMockClusterExpectationsForBuildClusterTests(acm);
	acm->buildEntrancesMocker.expects(once()).after(MOCKPP_PCHAR("addNodesMocker"));	
}

void AnnotatedClusterAbstractionTest::buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters()
{
	acmock_factory->setTest(1);
	/* figure out how many clusters the test map should have */
	int mapwidth = aca->getMap()->getMapWidth();
	int mapheight = aca->getMap()->getMapHeight();
	
	int numHorizontalClusters = mapwidth / aca->getClusterSize();
	if(mapwidth % aca->getClusterSize() > 0)
		numHorizontalClusters++;
		
	int numVerticalClusters = mapheight / aca->getClusterSize();
	if(mapheight % aca->getClusterSize() > 0)
		numVerticalClusters++;
		
	int totalExpectedClusters = numHorizontalClusters*numVerticalClusters;

	/* check for the correct # of clusters */
	aca->buildClusters(acmock_factory);
	CPPUNIT_ASSERT_EQUAL(totalExpectedClusters, aca->getNumClusters());
}

void AnnotatedClusterAbstractionTest::buildClustersShouldCalculateCorrectClusterSize()
{
	acmock_factory->setTest(1);

	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);

	int clusterWidths[4] = {5,4,5,4};
	int clusterHeights[4] = {5,5,1,1};

	for(int i=0; i<aca->getNumClusters(); i++)
	{
		AnnotatedCluster* ac = aca->getCluster(i);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster height", clusterHeights[i], ac->getHeight());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster width", clusterWidths[i], ac->getWidth());
	}
}

void AnnotatedClusterAbstractionTest::getClusterSizeShouldReturnSameValueAsConstructorParameter()
{
	CPPUNIT_ASSERT_EQUAL(TESTCLUSTERSIZE, aca->getClusterSize());
}

void AnnotatedClusterAbstractionTest::constructorShouldCreateANewGraphObject()
{
	int numGraphsExpected = 2;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("actual graph count does not match expected count", numGraphsExpected, aca->getNumberOfAbstractionLevels());
}


void AnnotatedClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsLessThanZero()
{
	int expectedValue = 0;
	int clusterid=-1;
	AnnotatedCluster* result = aca->getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid < 0", expectedValue, (int)result);
}

void AnnotatedClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters()
{
	int expectedValue = 0;
	int clusterid=aca->getNumClusters()+1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid >= numclusters", expectedValue, (int)aca->getCluster(clusterid));
}

void AnnotatedClusterAbstractionTest::getClusterShouldReturnRequestedClusterGivenAValidClusterId()
{
	acmock_factory->setTest(1);

	aca->buildClusters(acmock_factory);
	int clusterid=0;
	AnnotatedCluster* ac = aca->getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned wrong cluster", true, ac->getClusterId() == clusterid );
}

/* integration test */
void AnnotatedClusterAbstractionTest::buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraph()
{
	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);
	
	AnnotatedClusterFactory* ac_factory = new AnnotatedClusterFactory();
	aca->buildClusters(ac_factory);
	delete ac_factory;
	
	int numExpectedClusters = 4;
	int numExpectedAbstractEdges = 7;
	int numExpectedAbstractNodes = 10;
	
	cout << "\nok, go";
	aca->buildEntrances();
	cout << "\nok, done";

	graph* absg = aca->getAbstractGraph(1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of clusters created", numExpectedClusters, aca->getNumClusters());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract nodes", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract edges", numExpectedAbstractEdges, absg->getNumEdges());
}

void AnnotatedClusterAbstractionTest::buildEntrancesShouldAskEachClusterToCreateItsOwnEntrances()
{
	acmock_factory->setTest(2);

	int numExpectedClusters = 4;
	aca->buildClusters(acmock_factory);
	
	/* run test */
	aca->buildEntrances();
	
	for(int i=0;i<4;i++)
	{
		AnnotatedClusterMock* acm = dynamic_cast<AnnotatedClusterMock*>(aca->getCluster(i));
		acm->verify();
	}
}
