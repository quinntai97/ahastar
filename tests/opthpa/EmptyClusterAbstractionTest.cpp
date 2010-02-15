#include "EmptyClusterAbstractionTest.h"

#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "EmptyClusterFactory.h"
#include "HPAUtil.h"
#include "map.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EmptyClusterAbstractionTest );

void EmptyClusterAbstractionTest::setUp()
{
}

void EmptyClusterAbstractionTest::tearDown()
{
}

void EmptyClusterAbstractionTest::buildClustersDecomposesTheMapIntoEmptyClusters()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
//	ecmap.setVerbose(true);
	ecmap.buildClusters();

	int numExpectedClusters = 9;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to build correct # of clusters", 
			numExpectedClusters, ecmap.getNumClusters());

	int csizes[42] = {15, 3, 2, 6, 5, 4, 2, 1, 1};

	int cszIndex = 0;
	cluster_iterator it = ecmap.getClusterIter();
	EmptyCluster* cluster = ecmap.clusterIterNext(it);
	while(cluster)
	{
		CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong # of nodes", csizes[cszIndex],
				cluster->getNumNodes());
		cluster = ecmap.clusterIterNext(it);
		cszIndex++;
		
	}
}
