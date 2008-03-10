/*
 *  AnnotatedClusterTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 27/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERTEST_H
#define ANNOTATEDCLUSTERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class AnnotatedMapAbstractionMock;
class AnnotatedCluster;
class node;
class graph;

class AnnotatedClusterTest : public CPPUNIT_NS::TestFixture
{
	
	CPPUNIT_TEST_SUITE( AnnotatedClusterTest );
	CPPUNIT_TEST( addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions );
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenClusterIsFull );
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster );
	CPPUNIT_TEST( addNodeShouldIncrementByOneTotalNodesInCluster );
	CPPUNIT_TEST( addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId );
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenNodeParameterIsNull );
	CPPUNIT_TEST( addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenXOriginParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenYOriginParameterIsInvalid );
	CPPUNIT_TEST( addEntranceToGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent );
	CPPUNIT_TEST( addEntranceToGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes );
	CPPUNIT_TEST( addEntranceToGraphShouldConnectAbstractNodesWithANewEdge );

	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		void optionalSetUp();

		/* AnnotatedCluster() */
		void constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid();
		void constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid();
		void constructorShouldThrowExceptionWhenXOriginParameterIsInvalid();
		void constructorShouldThrowExceptionWhenYOriginParameterIsInvalid();

		/* addNode() */
		void addNodeShouldIncrementByOneTotalNodesInCluster();
		void addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId();
		void addNodeShouldReturnFalseWhenParameterNodeHasClearanceLargerThanOriginNode();
		void addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster();
		void addNodeShouldThrowExceptionWhenClusterIsFull();
		void addNodeShouldThrowExceptionWhenNodeParameterIsNull();

		/* addNodesToCluster() */
		void addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions();
		void addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull();
		
		/* addEntranceToGraph acceptance tests */
		void addEntranceToGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph();
		void addEntranceToGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes();
		void addEntranceToGraphShouldConnectAbstractNodesWithANewEdge();
		void addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull();
		void addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull();
		void addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject();
		void addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster();
		void addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle();
		void addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle();
		void addEntranceToGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent();

		/* yet to implement */
		void buildHorizontalEntrancesShouldIdentifyAsEntrancesPairsOfNodesWithMaximalClearanceThatConnectAdjacentClusters();
		void buildHorizontalEntrancesShouldSkipAnyCandidateEntrancesWhereOneOfTheNodesIsAHardObstacle();
		void buildHorizontalEntrancesShouldIdentifyOneEntranceForEachSectionOfCapabilityHomogenousNodesAlongTheClusterBorderIfSectionSizeLessThan5();
		void buildHorizontalEntrancesShouldIdentifyTwoEntrancesForEachSectionOfCapabilityHomogenousNodesAlongTheClusterBorderIfSectionSizeAtLeast5();

		
		
	private:
		AnnotatedMapAbstractionMock* ama_mock;
		AnnotatedCluster* ac;
		int cwidth; 
		int cheight;
		node* e1_n1;
		node* e1_n2;
		node* e2_n2hardobst;
		node* e2_n1;
		graph* absg;


};

#endif