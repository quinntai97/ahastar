/*
 *  AnnotatedHierarchicalAStarTest.h
 *  hog
		
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDHIERARCHICALASTARTEST_H
#define ANNOTATEDHIERARCHICALASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ExperimentManager.h"

class ExperimentManager;
class AnnotatedClusterAbstractionMock;
class AnnotatedHierarchicalAStar;
class node;
class edge;

class AnnotatedHierarchicalAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedHierarchicalAStarTest );
	CPPUNIT_TEST( evaluateShouldReturnFalseIfEitherNodeParameterIsNull );
	CPPUNIT_TEST( evaluateShouldReturnFalseIfEdgeBeingTraversedIsNull );
	CPPUNIT_TEST( evaluateShouldReturnFalseIfEdgeDoesNotConnectParameterNodes );
	CPPUNIT_TEST( evaluteShouldReturnTrueIfTheEdgeConnectingTwoParameterNodesIsTraversable );
	CPPUNIT_TEST( evaluateShouldReturnFalseIfTheEdgeIsNotTraversable );
	CPPUNIT_TEST( getAbstractPathShouldFindTheShortestPathBetweenTwoAbstractNodesGivenACapabilityAndAClearanceAndAnAnnotatedClusterAbstractionParameter );
	CPPUNIT_TEST( getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodesGivenACapabilityAndClearanceAndAnnotatedClusterAbstractionParameter );
	CPPUNIT_TEST( getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution );
	CPPUNIT_TEST( getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFindsASolution );
	CPPUNIT_TEST( getPathShouldFindASolutionEvenWhenCacheReturnsAPathInReverseOrderToRequirements );
	CPPUNIT_TEST( getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster );
	CPPUNIT_TEST( getPathShouldAddInsertionEffortToPerformanceMetrics );
	CPPUNIT_TEST( logStatsShouldRecordAllMetricsToStatsCollection );
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		void evaluateShouldReturnFalseIfEitherNodeParameterIsNull();
		void evaluateShouldReturnFalseIfEdgeBeingTraversedIsNull();
		void evaluateShouldReturnFalseIfEdgeDoesNotConnectParameterNodes();
		void evaluteShouldReturnTrueIfTheEdgeConnectingTwoParameterNodesIsTraversable();
		void evaluateShouldReturnFalseIfTheEdgeIsNotTraversable();
		
		//// INTEGRATION TEST ////
		void getAbstractPathShouldFindTheShortestPathBetweenTwoAbstractNodesGivenACapabilityAndAClearanceAndAnAnnotatedClusterAbstractionParameter();
		void getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodesGivenACapabilityAndClearanceAndAnnotatedClusterAbstractionParameter();
		void getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution();
		void getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFindsASolution();	
		void getPathShouldFindASolutionEvenWhenCacheReturnsAPathInReverseOrderToRequirements();	
		void getPathShouldAddInsertionEffortToPerformanceMetrics();
		void getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster();
		
		void logStatsShouldRecordAllMetricsToStatsCollection();
		
		
	private:
		AnnotatedClusterAbstractionMock* acamock;
		AnnotatedHierarchicalAStar* ahastar;
		node  *p, *n;	
		edge *e;
};

#endif