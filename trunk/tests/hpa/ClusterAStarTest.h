/*
 *  ClusterAStarTest.h
 *  hog		
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERASTARTEST_H
#define CLUSTERASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ClusterAStar;
class HPAClusterAbstraction;
class ClusterNode;
class edge;
class path;

class ClusterAStarTest : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ClusterAStarTest );	
	CPPUNIT_TEST( getPathReturnNullWhenStartOrGoalNull );
	CPPUNIT_TEST( getPathReturnNullWhenStartAndGoalSameLocation );
	CPPUNIT_TEST( getPathReturnNullWhenStartAndGoalNodesIdentical );
	CPPUNIT_TEST( getPathReturnNullWhenMapAbstractionParameterNull );
	CPPUNIT_TEST( getPathInitialisesFCostValueOfStartToHeuristicGoalDistance );
	CPPUNIT_TEST( getPathEachNodeInReturnedPathHasAnEdgeToItsPredecessor );
	CPPUNIT_TEST( getPathFailsToReturnASoltuionWhenNoneExistsWithinTheCorridorBounds );
	CPPUNIT_TEST( getPathReturnsTheShortestPath );
	CPPUNIT_TEST( logStatsShouldRecordAllMetricsToStatsCollection );

	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		// check for valid start/goal params 
		void getPathReturnNullWhenStartOrGoalNull();
		void getPathReturnNullWhenStartAndGoalSameLocation();
		void getPathReturnNullWhenStartAndGoalNodesIdentical();
		void getPathReturnNullWhenNonHPAClusterAbstractionParameter();
		void getPathReturnNullWhenMapAbstractionParameterNull();
		void getPathInitialisesFCostValueOfStartToHeuristicGoalDistance();
		void getPathEachNodeInReturnedPathHasAnEdgeToItsPredecessor();
		void getPathFailsToReturnASoltuionWhenNoneExistsWithinTheCorridorBounds();
		void getPathReturnsTheShortestPath();
		void logStatsShouldRecordAllMetricsToStatsCollection();
		
	private:
		ClusterNode* getNode(int x, int y, HPAClusterAbstraction&);
		path* p;
};

#endif