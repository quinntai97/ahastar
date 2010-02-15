#include "EmptyClusterTest.h"

#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "EmptyClusterFactory.h"
#include "map.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EmptyClusterTest );

void EmptyClusterTest::setUp()
{
}

void EmptyClusterTest::tearDown()
{
}

void EmptyClusterTest::addNodesToClusterGrowsAMaximalSizeEmptyCluster()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster cluster(0, 0);
	cluster.addNodesToCluster(&ecmap);

	int expectedHeight = 3;
	int expectedWidth = 5;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong height", expectedHeight,
			cluster.getHeight());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong width", expectedWidth, 
			cluster.getWidth());
}

void EmptyClusterTest::addNodesToClusterSetsTheClusterIdOfEveryNodeInTheClusterArea()
{
	CPPUNIT_ASSERT_EQUAL_MESSAGE("implement me", true, false);
}

void EmptyClusterTest::addNodesToClusterFramesTheEmptyClusterWithAbstractNodes()
{
	CPPUNIT_ASSERT_EQUAL_MESSAGE("implement me", true, false);
}

