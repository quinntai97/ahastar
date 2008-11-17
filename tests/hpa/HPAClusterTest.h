/*
 *  HPAClusterTest.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERTEST_H
#define HPACLUSTERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HPACluster.h"
#include <stdexcept>

class HPAClusterTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAClusterTest );
	
		CPPUNIT_TEST_EXCEPTION( constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( constructorShouldThrowExceptionWhenXOriginParameterIsInvalid, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( constructorShouldThrowExceptionWhenYOriginParameterIsInvalid, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( constructorShouldThrowExceptionWhenAlgorithmParameterIsNULL, std::invalid_argument );
	
		CPPUNIT_TEST_EXCEPTION( addParentThrowsExceptionGivenANULLNodeParameter, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( addParentThrowsExceptionGivenANULL_HPAClusterAbstraction_Parameter, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( addParentThrowsExceptionGivenANodeParameterThatIsNotOfType_ClusterNode, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( addParentThrowsExceptionGivenANodeParameterThatIsAlreadyAssignedToAnotherCluster, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( addParentThrowsExceptionGivenANodeParameterThatHasNotBeenAddedToTheAbstractGraph, std::invalid_argument );
		CPPUNIT_TEST( addParentUpdatesParentClusterIdOfNodeParameterToIdOfCluster );
		CPPUNIT_TEST( addParentAddsNodeToParentsCollection );
		CPPUNIT_TEST( addParentShouldIgnoreNodesAlreadyInTheParentsCollection );
		CPPUNIT_TEST( addParentShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints );
		
		CPPUNIT_TEST_EXCEPTION( addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster, std::invalid_argument );
		CPPUNIT_TEST_EXCEPTION( addNodeShouldThrowExceptionWhenParameterNodeHasCoordinatesOutsideClusterBoundary, std::invalid_argument);
		CPPUNIT_TEST_EXCEPTION( addNodeShouldThrowExceptionWhenNodeParameterIsNull, std::invalid_argument );
		CPPUNIT_TEST( addNodeShouldIncrementByOneTotalNodesInCluster );
		CPPUNIT_TEST( addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId );

		CPPUNIT_TEST_EXCEPTION( addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull, std::invalid_argument );
		CPPUNIT_TEST( addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions );
		
		CPPUNIT_TEST( buildVerticalEntrancesShouldCreateOneTransitionPointForAnEntranceOfLengthLessThan_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE );
		CPPUNIT_TEST( buildVerticalEntrancesShouldCreateTwoTransitionPointsForAnEntranceOfLengthGreaterThanOrEqualTo_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE );
		CPPUNIT_TEST( buildVerticalEntrancesShouldCreateAnEntranceOnEachSideOfAnObstacleAlongTheEntranceArea );

		CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateOneTransitionPointForAnEntranceOfLengthLessThan_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE );
		CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateTwoTransitionPointsForAnEntranceOfLengthGreaterThanOrEqualTo_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE );
		CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateAnEntranceOnEachSideOfAnObstacleAlongTheEntranceArea );

		CPPUNIT_TEST_EXCEPTION( buildEntrancesShouldThrowExceptionGivenA_NULL_ACAParameter, std::invalid_argument );		
		CPPUNIT_TEST( builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters );



	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void addParentThrowsExceptionGivenANULLNodeParameter();
		void addParentThrowsExceptionGivenANULL_HPAClusterAbstraction_Parameter();
		void addParentThrowsExceptionGivenANodeParameterThatIsNotOfType_ClusterNode();
		void addParentThrowsExceptionGivenANodeParameterThatIsAlreadyAssignedToAnotherCluster();
		void addParentThrowsExceptionGivenANodeParameterThatHasNotBeenAddedToTheAbstractGraph();
		void addParentUpdatesParentClusterIdOfNodeParameterToIdOfCluster();
		void addParentAddsNodeToParentsCollection();
		void addParentShouldIgnoreNodesAlreadyInTheParentsCollection();
		void addParentShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints();
		
		void addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster();
		void addNodeShouldThrowExceptionWhenParameterNodeHasCoordinatesOutsideClusterBoundary();
		void addNodeShouldIncrementByOneTotalNodesInCluster();
		void addNodeShouldThrowExceptionWhenNodeParameterIsNull();
		void addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId();
		
		void addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull();
		void addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions();
		
		void constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid();
		void constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid();
		void constructorShouldThrowExceptionWhenXOriginParameterIsInvalid();
		void constructorShouldThrowExceptionWhenYOriginParameterIsInvalid();
		void constructorShouldThrowExceptionWhenAlgorithmParameterIsNULL();
		
		void buildVerticalEntrancesShouldCreateOneTransitionPointForAnEntranceOfLengthLessThan_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE();
		void buildVerticalEntrancesShouldCreateTwoTransitionPointsForAnEntranceOfLengthGreaterThanOrEqualTo_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE();
		void buildVerticalEntrancesShouldCreateAnEntranceOnEachSideOfAnObstacleAlongTheEntranceArea();

		void buildHorizontalEntrancesShouldCreateOneTransitionPointForAnEntranceOfLengthLessThan_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE();
		void buildHorizontalEntrancesShouldCreateTwoTransitionPointsForAnEntranceOfLengthGreaterThanOrEqualTo_MAX_SINGLE_TRANSITION_ENTRANCE_SIZE();
		void buildHorizontalEntrancesShouldCreateAnEntranceOnEachSideOfAnObstacleAlongTheEntranceArea();
		
		void buildEntrancesShouldThrowExceptionGivenA_NULL_ACAParameter();
		void builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters();

};

#endif