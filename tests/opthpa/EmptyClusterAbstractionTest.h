#ifndef EMPTYCLUSTERABSTRACTIONTEST_H
#define EMPTYCLUSTERABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class EmptyClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( EmptyClusterAbstractionTest );

	CPPUNIT_TEST( buildClustersDecomposesTheMapIntoEmptyClusters );
	CPPUNIT_TEST( buildEntrancesConnectsAllClusters );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphWorksAsAdvertised );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphDoesNotAddAnythingIfStartOrGoalExistInGraphAlready );
	CPPUNIT_TEST( hComputesTileDistanceBetweenTwoNodes );

	CPPUNIT_TEST_SUITE_END();

	public:
		
		void setUp();
		void tearDown();

		void buildClustersDecomposesTheMapIntoEmptyClusters();
		void buildEntrancesConnectsAllClusters();

		void insertStartAndGoalNodesIntoAbstractGraphWorksAsAdvertised();
		void insertStartAndGoalNodesIntoAbstractGraphDoesNotAddAnythingIfStartOrGoalExistInGraphAlready();
		void hComputesTileDistanceBetweenTwoNodes();

};

#endif

