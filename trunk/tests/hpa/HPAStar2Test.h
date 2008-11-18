/*
 *  HPAStar2Test.h
 *  hog
 *
 *  Created by dharabor on 18/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPASTAR2TEST_H
#define HPASTAR2TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HPAStar2.h"
#include <stdexcept>

class path;
class ClusterNode;
class HPAClusterAbstraction;

class HPAStar2Test : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAStar2Test );
		CPPUNIT_TEST( getPathReturnNullWhenStartOrGoalNull );
		CPPUNIT_TEST( getPathReturnNullWhenStartAndGoalSameLocation );
		CPPUNIT_TEST( getPathReturnNullWhenStartAndGoalNodesIdentical );
		CPPUNIT_TEST( getPathReturnNullWhenMapAbstractionParameterNull );
		CPPUNIT_TEST_EXCEPTION( getPathReturnNullWhenStartOrGoalHave_kAbstractionLevel_GreaterThanZero, std::invalid_argument );
		CPPUNIT_TEST( getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodes );
		CPPUNIT_TEST( getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution );
		CPPUNIT_TEST( getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster );
		CPPUNIT_TEST( getPathShouldAddInsertionEffortToPerformanceMetrics );
		CPPUNIT_TEST( logStatsShouldRecordAllMetricsToStatsCollection );
		CPPUNIT_TEST(getPathShouldReturnANonRefinedPathIfRefinementFlagIsNotSet );
		CPPUNIT_TEST(getPathShouldReturnAShortestPathBetweenTwoLowLevelNodesIfFastRefinementFlagIsSet );
		// TODO: intra-edge refinement limited to corridor (requires working mocks)
		// TODO: abstract path search can be limited to a corridor (requires a working ClusterAStarMock + some changes to HPAStar2 to pass in mock)
		// TODO: refine from cache or via search

	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void getPathReturnNullWhenStartOrGoalNull();
		void getPathReturnNullWhenStartAndGoalSameLocation();
		void getPathReturnNullWhenStartAndGoalNodesIdentical();
		void getPathReturnNullWhenMapAbstractionParameterNull();
		void getPathReturnNullWhenStartOrGoalHave_kAbstractionLevel_GreaterThanZero();
		
		void getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodes();
		void getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution();
		void getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster();
		void getPathShouldAddInsertionEffortToPerformanceMetrics();
		void logStatsShouldRecordAllMetricsToStatsCollection();
		void getPathShouldReturnANonRefinedPathIfRefinementFlagIsNotSet();
		void getPathShouldReturnAShortestPathBetweenTwoLowLevelNodesIfFastRefinementFlagIsSet(); // pretty crappy tes. see TODO above for fix. needs mocks.

	private:
		ClusterNode* getNode(int x, int y, HPAClusterAbstraction& hpamap);
		path* p;

};

#endif