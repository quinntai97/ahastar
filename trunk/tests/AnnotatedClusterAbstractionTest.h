/*
 *  AnnotatedClusterAbstractionTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 21/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ANNOTATEDCLUSTERABSTRACTIONTEST_H
#define ANNOTATEDCLUSTERABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ExperimentManager;
class AnnotatedClusterAbstraction;
class Map;
class node;

class AnnotatedClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( AnnotatedClusterAbstractionTest );
	CPPUNIT_TEST( constructorShouldSplitTheMapAreaIntoCorrectNumberOfClusters );
	CPPUNIT_TEST( getClusterSizeShouldReturnSameValueAsConstructorParameter );
	CPPUNIT_TEST( buildAbstractGraphShouldCreateANewGraphObject );
	CPPUNIT_TEST( addEntranceToGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceToGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes );
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();

		void constructorShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();
		
		void buildAbstractGraphShouldCreateANewGraphObject();

		/* addEntranceToGraph */
		void addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull();
		void addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull();
		void addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject();
		void addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster();
		void addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle();
		void addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle();

		void addEntranceToGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph();
		void addEntranceToGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes();


		/* yet to implement */

		
	private:
		AnnotatedClusterAbstraction *aca;
		ExperimentManager* expmgr;
		Map *testmap;
		node* n1;
		node* n2;
		node* n3hardobst;
};

#endif