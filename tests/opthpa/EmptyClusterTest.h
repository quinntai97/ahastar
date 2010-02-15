#ifndef EMPTYCLUSTERTEST_H
#define EMPTYCLUSTERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HPACluster.h"
#include <stdexcept>

class EmptyClusterTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( EmptyClusterTest );

	CPPUNIT_TEST( addNodesToClusterGrowsAMaximalSizeEmptyCluster );
	CPPUNIT_TEST( addNodesToClusterSetsTheClusterIdOfEveryNodeInTheClusterArea );
	CPPUNIT_TEST( addNodesToClusterFramesTheEmptyClusterWithAbstractNodes );

	CPPUNIT_TEST_SUITE_END();


	public:
		void setUp();
		void tearDown();

		void addNodesToClusterGrowsAMaximalSizeEmptyCluster();
		void addNodesToClusterSetsTheClusterIdOfEveryNodeInTheClusterArea();
		void addNodesToClusterFramesTheEmptyClusterWithAbstractNodes();

};

#endif

