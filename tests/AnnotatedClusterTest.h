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

#include "AnnotatedCluster.h"
#include "graph.h"
#include "AnnotatedClusterAbstractionMock.h"

class TestEntrance
{
	public:
		TestEntrance(int fromx, int fromy, int tox, int toy, int capability, int clearance, int fromClusterId, int toClusterId);
		int getFromX() { return fromx; }
		int getToX() { return tox; }
		int getFromY() { return fromy; }
		int getToY() { return toy; }

		int getCapability() { return capability; }
		int getClearance(int capability);
		int getFromClusterId() { return fromClusterId; }
		int getToClusterId() { return toClusterId; }

	
	private:
		int fromx, fromy, tox, toy, capability, clearance, fromClusterId, toClusterId;
};


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
	CPPUNIT_TEST( addInterEdgeShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph );
	CPPUNIT_TEST( addInterEdgeShouldReuseExistingNodeEndpointsIfADifferentEntranceExistsAtSameLocation );
	CPPUNIT_TEST( addInterEdgeShouldReuseExistingEdgeIfCapabilityParameterIsASupersetOfExistingEdgeCapability );
	CPPUNIT_TEST( addInterEdgeShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes ); 
	CPPUNIT_TEST( addInterEdgeShouldConnectAbstractNodesWithANewAnnotatedEdge );
	CPPUNIT_TEST( addInterEdgeShouldAddEachEntranceEndpointToItsCluster );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfFirstNodeParameterNodeIsNull );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfSecondNodeParameterNodeIsNull );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfParameterNodesPointToSameObject );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfParameterNodesShareTheSameCluster );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfParameterNodesAreNotAdjacent );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfCapabilityClearanceOfFirstParameterNodeIsNotEqualToOrGreaterThanClearanceParameter );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfCapabilityClearanceOfSecondParameterNodeIsNotEqualToOrGreaterThanClearanceParameter );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfClearanceParameterLessThanOrEqualToZero );
	CPPUNIT_TEST( addInterEdgeShouldThrowExceptionIfCapabilityParameterIsInvalid );
	CPPUNIT_TEST( buildVerticalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheVerticalBorderBetweenTwoClusters );
	CPPUNIT_TEST( buildVerticalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter );
	CPPUNIT_TEST( buildVerticalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter );
	CPPUNIT_TEST( buildVerticalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongEasternBorder );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheHorizontalBorderBetweenTwoClusters );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongSouthernBorder );
	CPPUNIT_TEST( builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters );


	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		void createEntranceNodes();
		void setupExceptionThrownTestHelper();

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
		
		/* addInterEdge() */
		void addInterEdgeShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph();
		void addInterEdgeShouldAddEachEntranceEndpointToItsCluster();
		void addInterEdgeShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes();
		void addInterEdgeShouldConnectAbstractNodesWithANewAnnotatedEdge();
		void addInterEdgeShouldReuseExistingNodeEndpointsIfADifferentEntranceExistsAtSameLocation();
		void addInterEdgeShouldReuseExistingEdgeIfCapabilityParameterIsASupersetOfExistingEdgeCapability();
		void addInterEdgeShouldThrowExceptionIfFirstNodeParameterNodeIsNull();
		void addInterEdgeShouldThrowExceptionIfSecondNodeParameterNodeIsNull();
		void addInterEdgeShouldThrowExceptionIfParameterNodesPointToSameObject();
		void addInterEdgeShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addInterEdgeShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero();
		void addInterEdgeShouldThrowExceptionIfParameterNodesShareTheSameCluster();
		void addInterEdgeShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle();
		void addInterEdgeShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle();
		void addInterEdgeShouldThrowExceptionIfParameterNodesAreNotAdjacent();
		void addInterEdgeShouldThrowExceptionIfWeightParameterInvalid();
		void addInterEdgeShouldThrowExceptionIfCapabilityParameterIsInvalid();
		void addInterEdgeShouldThrowExceptionIfCapabilityClearanceOfFirstParameterNodeIsNotEqualToOrGreaterThanClearanceParameter();
		void addInterEdgeShouldThrowExceptionIfCapabilityClearanceOfSecondParameterNodeIsNotEqualToOrGreaterThanClearanceParameter();
		void addInterEdgeShouldThrowExceptionIfClearanceParameterLessThanOrEqualToZero();



		/* buildVerticalEntrances() */
		void buildVerticalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheVerticalBorderBetweenTwoClusters();
		void buildVerticalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();
		void buildVerticalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter();
		void buildVerticalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongEasternBorder();

		/* buildHorizontalEntrances() */
		void buildHorizontalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheHorizontalBorderBetweenTwoClusters();
		void buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();
		void buildHorizontalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter();
		void buildHorizontalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongSouthernBorder();
		
		void builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters();
		
		//void buildEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();

		/* yet to implement */

		
		// future optimisation?
		void buildVerticalEntrancesShouldIdentifyOneEntranceForEachSectionOfCapabilityHomogenousNodesAlongTheClusterBorderIfSectionSizeLessThan5();
		void buildVerticalEntrancesShouldIdentifyTwoEntrancesForEachSectionOfCapabilityHomogenousNodesAlongTheClusterBorderIfSectionSizeAtLeast5();

		
	private:
		double interedge_weight;
		int cwidth, cheight;
		int e1_capability, e1_clearance;
		int e2_capability, e2_clearance;
		int e3_capability, e3_clearance;
		int e4_capability, e4_clearance;

		AnnotatedClusterAbstractionMock* aca_mock;
		AnnotatedCluster* ac;
		node *e1_n1, *e1_n2;
		node *e2_n2hardobst, *e2_n1;
		node *e3_n1, *e3_n2;
		node *e4_n1, *e4_n2;
		graph* absg;
		
		exceptionThrownHelper *testHelper;
		


};

/* templated code for running tests that involve throwing exceptions */
class exceptionThrownHelper
{
	public:
		exceptionThrownHelper() {}
		
		template<class ExceptionType>
		void checkaddInterEdgeThrowsCorrectException(node* n1, node* n2, int capability, int clearance)
		{
			bool exceptionThrown = false;
			int numnodes = absg->getNumNodes();
				
			try 
			{
				ac->addInterEdge(n1, n2, capability, clearance, aca_mock);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, absg->getNumNodes());
		};
		
		template<class ExceptionType>
		void checkBuildVerticalEntrancesThrowsCorrectException(int capability, AnnotatedClusterAbstraction* aca)
		{
			bool exceptionThrown = false;
			try
			{
				ac->buildVerticalEntrances(capability, aca);
			}
			catch(ExceptionType& e)
			{
					exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
		}

		template<class ExceptionType>
		void checkBuildHorizontalEntrancesThrowsCorrectException(int capability, AnnotatedClusterAbstraction* aca)
		{
			bool exceptionThrown = false;
			try
			{
				ac->buildHorizontalEntrances(capability, aca);
			}
			catch(ExceptionType& e)
			{
					exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
		}

		
		void setFailMessage(std::string& msg) { failmessage = msg; } 
		void setAbstractGraph(graph *g) { this->absg = g; }
		void setAnnotatedCluster(AnnotatedCluster* ac) { this->ac = ac; }
		void setAnnotatedClusterAbstraction(AnnotatedClusterAbstractionMock* aca_mock) { this->aca_mock = aca_mock; }
	
	private:
		std::string failmessage;
		AnnotatedCluster* ac; 
		AnnotatedClusterAbstractionMock* aca_mock;
		graph* absg;
};

#endif