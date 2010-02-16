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
	CPPUNIT_TEST( addNodesToClusterFramesEmptyClustersWithWidthEqualTo1 );
	CPPUNIT_TEST( addNodesToClusterFramesEmptyClustersWithHeightEqualTo1 );
	CPPUNIT_TEST( addNodesToClusterFramesEmptyClusterWhenOriginIsNotOnTheMapBorder );
	CPPUNIT_TEST( addNodesToClusterAddsMacroEdgesToEachFramedCluster );
	CPPUNIT_TEST( addNodesToClusterAddsNoHorizontalMacroEdgesWhenClusterWidthIs1 );
	CPPUNIT_TEST( addNodesToClusterAddsNoVerticalMacroEdgesWhenClusterHeightIs1 );

	CPPUNIT_TEST( buildHorizontalEntrancesCreatesAllPossibleTransitionPoints );
	CPPUNIT_TEST( buildVerticalEntrancesCreatesAllPossibleTransitionPoints );

	CPPUNIT_TEST_SUITE_END();


	public:
		void setUp();
		void tearDown();

		void addNodesToClusterGrowsAMaximalSizeEmptyCluster();
		void addNodesToClusterSetsTheClusterIdOfEveryNodeInTheClusterArea();
		void addNodesToClusterFramesTheEmptyClusterWithAbstractNodes();
		void addNodesToClusterFramesEmptyClustersWithHeightEqualTo1();
		void addNodesToClusterFramesEmptyClustersWithWidthEqualTo1();
		void addNodesToClusterFramesEmptyClusterWhenOriginIsNotOnTheMapBorder();
		void addNodesToClusterAddsMacroEdgesToEachFramedCluster();
		void addNodesToClusterAddsNoVerticalMacroEdgesWhenClusterHeightIs1();
		void addNodesToClusterAddsNoHorizontalMacroEdgesWhenClusterWidthIs1();

		void buildHorizontalEntrancesCreatesAllPossibleTransitionPoints();
		void buildVerticalEntrancesCreatesAllPossibleTransitionPoints();

};

#endif

