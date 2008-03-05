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
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();

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
		
		
	private:
		AnnotatedMapAbstractionMock* ama_mock;
		AnnotatedCluster* ac;
		int cwidth; 
		int cheight;

};

#endif