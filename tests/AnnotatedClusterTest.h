/*
 *  AnnotatedClusterTest.h
 *  hog
 *
	TODO: Optimise buildHorizontalEntrances and buildVerticalEntrances
		- If a large homogenous area exists along a border between two clusters, we should place an entrance every 10 tiles
		- If the size of the homogenous area is < 10, we should have a single entrance in the middle
 
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
	
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenClusterIsFull );
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster );
	CPPUNIT_TEST( addNodeShouldIncrementByOneTotalNodesInCluster );
	CPPUNIT_TEST( addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId );
	CPPUNIT_TEST( addNodeShouldThrowExceptionWhenNodeParameterIsNull );
	
	CPPUNIT_TEST( addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions );
	CPPUNIT_TEST( addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull );
	
	CPPUNIT_TEST( addParentShouldThrowExceptionIfParameterNodeIsAlreadyAssignedToAnotherCluster );
	CPPUNIT_TEST( addParentShouldAddParameterNodeToAbstaractNodesListInCluster );
	CPPUNIT_TEST( addParentShouldNotAddAnyNodesAlreadyMarkedAsBelongingToTargetCluster );
	CPPUNIT_TEST( addParentsShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints );

	
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenXOriginParameterIsInvalid );
	CPPUNIT_TEST( constructorShouldThrowExceptionWhenYOriginParameterIsInvalid );
	
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle );
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfCapabilityClearanceOfFirstParameterNodeIsNotEqualToOrGreaterThanClearanceParameter );
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfCapabilityClearanceOfSecondParameterNodeIsNotEqualToOrGreaterThanClearanceParameter );
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfClearanceParameterLessThanOrEqualToZero );
	CPPUNIT_TEST( addEntranceShouldThrowExceptionIfCapabilityParameterIsInvalid );
	CPPUNIT_TEST( addEntranceShouldSetEdgeWeightToExactlyOne );
	CPPUNIT_TEST( addEntranceShouldTruncateClearanceTo_MAXAGENTSIZE_IfLarger );
	
	CPPUNIT_TEST( validateMapAbstractionThrowsExceptionGivenNullParameter );
	
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph );
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldSetAbstractNodesAsParentsOfNonAbstractNodes );
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldReuseExistingNodeEndpointsIfADifferentEntranceExistsAtSameLocation ); 
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldAddEachEntranceEndpointToItsCluster );
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes ); 
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent );
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster );
	CPPUNIT_TEST( addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodeHaveAnAbstractionLevelNotEqualToZero );
	
	CPPUNIT_TEST( buildVerticalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheVerticalBorderBetweenTwoClusters );
	CPPUNIT_TEST( buildVerticalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter );
	CPPUNIT_TEST( buildVerticalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter );
	CPPUNIT_TEST( buildVerticalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongEasternBorder );
	CPPUNIT_TEST( buildVerticalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea );
	
	CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheHorizontalBorderBetweenTwoClusters );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongSouthernBorder );
	CPPUNIT_TEST( buildHorizontalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea );
	
	CPPUNIT_TEST( builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters );
	CPPUNIT_TEST( buildEntrancesShouldThrowExceptionGivenAnInvalidACAParameter );
		
	CPPUNIT_TEST( validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesHaveIdenticalCoordinates );
	CPPUNIT_TEST( validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesAreNull );

	CPPUNIT_TEST( addTransitionToAbstractGraphShouldThrowExceptionWhenWeightIsNotGreaterThanZero );
	CPPUNIT_TEST( addTransitionToAbstractGraphShouldConnectAbstractNodesWithANewAnnotatedEdge ); 
	CPPUNIT_TEST( addTransitionToAbstractGraphShouldReuseExistingEdgeIfOneExists );
	CPPUNIT_TEST( addTransitionToAbstractGraphShouldAddANewPathToTheAnnotatedClusterAbstractionCacheForEachNewlyCreatedEdge );
	
	CPPUNIT_TEST( connectEntranceEndpointsShouldCalculateTheShortestPathBetweenEachPairOfParentNodesForEachEligibleCapabilityGivenAHighQualityAbstraction );
	CPPUNIT_TEST( connectEntranceEndpointsShouldAddAPathToTheAnnotatedClusterAbstractionCacheEachTimeTwoEndpointsAreConnected );
	CPPUNIT_TEST( connectEntranceEndpointsShouldCalculateFirstTheSetOfShortestPathsWithLargestClearanceGivenALowQualityAbstraction );
	
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		void createEntranceNodes();
		void setupExceptionThrownTestHelper(graph*, AnnotatedCluster*, AnnotatedClusterAbstraction*, std::string&);
		
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
		
		/* addParent */
		void addParentShouldThrowExceptionIfParameterNodeIsAlreadyAssignedToAnotherCluster();
		void addParentShouldAddParameterNodeToAbstaractNodesListInCluster();
		void addParentShouldNotAddAnyNodesAlreadyMarkedAsBelongingToTargetCluster();
		void addParentsShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints();

		/* addNodesToCluster() */
		void addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions();
		void addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull();
		
		void validateMapAbstractionThrowsExceptionGivenNullParameter();
		void validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesHaveIdenticalCoordinates();
		void validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesAreNull();
		
		/* addEndpointsToAbstractGraph */
		void addEndpointsToAbstractGraphShouldSetAbstractNodesAsParentsOfNonAbstractNodes();
		void addEndpointsToAbstractGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph();
		void addEndpointsToAbstractGraphShouldReuseExistingNodeEndpointsIfADifferentEntranceExistsAtSameLocation();
		void addEndpointsToAbstractGraphShouldAddEachEntranceEndpointToItsCluster();
		void addEndpointsToAbstractGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes();
		void addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent();
		void addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster();
		void addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodeHaveAnAbstractionLevelNotEqualToZero();
		
		/* addTransitionToAbstractGraph */
		void addTransitionToAbstractGraphShouldThrowExceptionWhenWeightIsNotGreaterThanZero();
		void addTransitionToAbstractGraphShouldConnectAbstractNodesWithANewAnnotatedEdge();
		void addTransitionToAbstractGraphShouldReuseExistingEdgeIfOneExists();
		void addTransitionToAbstractGraphShouldAddANewPathToTheAnnotatedClusterAbstractionCacheForEachNewlyCreatedEdge();
				
		/* addEntrance() */
		void addEntranceShouldTruncateClearanceTo_MAXAGENTSIZE_IfLarger();
		void addEntranceShouldSetEdgeWeightToExactlyOne();
		void addEntranceShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle(); // should be checking during build phase. not needed?
		void addEntranceShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle();
		void addEntranceShouldThrowExceptionIfCapabilityParameterIsInvalid(); // build phase check?
		void addEntranceShouldThrowExceptionIfCapabilityClearanceOfFirstParameterNodeIsNotEqualToOrGreaterThanClearanceParameter();
		void addEntranceShouldThrowExceptionIfCapabilityClearanceOfSecondParameterNodeIsNotEqualToOrGreaterThanClearanceParameter();
		void addEntranceShouldThrowExceptionIfClearanceParameterLessThanOrEqualToZero();

		/* buildVerticalEntrances() */
		void buildVerticalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheVerticalBorderBetweenTwoClusters();
		void buildVerticalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();
		void buildVerticalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter();
		void buildVerticalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongEasternBorder();
		void buildVerticalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea();

		/* buildHorizontalEntrances() */
		void buildHorizontalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheHorizontalBorderBetweenTwoClusters();
		void buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();
		void buildHorizontalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter();
		void buildHorizontalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongSouthernBorder();
		void buildHorizontalEntrancesShouldCreateTwoTransitionsAtEachEndOfTheEntranceAreaIfAllNodesAlongEachClusterBorderHaveIdenticalClearance();
		void buildHorizontalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea();
		
		/* buildEntrances() */
		void buildEntrancesShouldThrowExceptionGivenAnInvalidACAParameter();
		void builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters();
		
		/* connectEntranceEndpoints */
		void connectEntranceEndpointsShouldCalculateTheShortestPathBetweenEachPairOfParentNodesForEachEligibleCapabilityGivenAHighQualityAbstraction();
		void connectEntranceEndpointsShouldCalculateFirstTheSetOfShortestPathsWithLargestClearanceGivenALowQualityAbstraction();
		void connectEntranceEndpointsShouldAddAPathToTheAnnotatedClusterAbstractionCacheEachTimeTwoEndpointsAreConnected();
		
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
		void checkaddTransitionToAbstractGraphThrowsCorrectException(node* n1, node* n2, int capability, int clearance, double weight)
		{
			bool exceptionThrown = false;
			int numnodes = absg->getNumNodes();
				
			try 
			{
				ac->addTransitionToAbstractGraph(n1, n2, capability, clearance, weight, aca);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, absg->getNumNodes());
		};

		
		template<class ExceptionType>
		void checkaddEndpointsToAbstractGraphThrowsCorrectException(node* n1, node* n2)
		{
			bool exceptionThrown = false;
			int numnodes = absg->getNumNodes();
				
			try 
			{
				ac->addEndpointsToAbstractGraph(n1, n2, aca);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, absg->getNumNodes());
		};

		template<class ExceptionType>
		void checkaddEntranceThrowsCorrectException(node* n1, node* n2, int capability, int clearance)
		{
			bool exceptionThrown = false;
			int numnodes = absg->getNumNodes();
				
			try 
			{
				ac->addEntrance(n1, n2, capability, clearance, aca);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, absg->getNumNodes());
		};

		
		template<class ExceptionType>
		void checkBuildVerticalEntrancesThrowsCorrectException(int capability)
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
		void checkBuildHorizontalEntrancesThrowsCorrectException(int capability)
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
		
		template<class ExceptionType>
		void checkBuildEntrancesThrowsCorrectException()
		{
			bool exceptionThrown = false;
			try
			{
				ac->buildEntrances(aca);
			}
			catch(ExceptionType& e)
			{
					exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);	
		}
		
		template<class ExceptionType>
		void checkValidateMapAbstractionThrowsCorrectException()
		{
			bool exceptionThrown = false;
				
			try 
			{
				ac->validateMapAbstraction(aca);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
		};

		template<class ExceptionType>
		void checkValidateTransitionEndpointsThrowsCorrectException(node* from, node* to)
		{
			bool exceptionThrown = false;
				
			try 
			{
				ac->validateTransitionEndpoints(from, to);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
		};

		template<class ExceptionType>
		void checkAddParentThrowsCorrectException(node* target)
		{
			bool exceptionThrown = false;
				
			try 
			{
				ac->addParent(target, aca);
			}
			catch(ExceptionType& e)
			{	
				exceptionThrown = true;
			}
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE(failmessage.c_str(), true, exceptionThrown);
		};
		
		void setFailMessage(std::string& msg) { failmessage = msg; } 
		void setAbstractGraph(graph *g) { this->absg = g; }
		void setAnnotatedCluster(AnnotatedCluster* ac) { this->ac = ac; }
		void setAnnotatedClusterAbstraction(AnnotatedClusterAbstraction* aca) { this->aca = aca; }
	
	private:
		std::string failmessage;
		AnnotatedCluster* ac; 
		AnnotatedClusterAbstraction* aca;
		graph* absg;
};

#endif