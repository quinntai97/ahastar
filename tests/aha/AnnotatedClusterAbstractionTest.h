/*
 *  AnnotatedClusterAbstractionTest.h
 *  hog
 *
 *	TODO: Not all buildClusterXXXX tests are properly mocked. Fix this.
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
class AnnotatedClusterMockFactory;
class Map;
class node;
class edge;
class AnnotatedClusterMock;
class NodeIsHardObstacleException;
class NodeIsNullException;

class AnnotatedClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( AnnotatedClusterAbstractionTest );
	CPPUNIT_TEST( buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters );
	CPPUNIT_TEST( buildClustersShouldCalculateCorrectClusterSize );
	CPPUNIT_TEST( getClusterSizeShouldReturnSameValueAsConstructorParameter );
	CPPUNIT_TEST( constructorShouldCreateANewGraphObject );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsLessThanZero );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters );
	CPPUNIT_TEST( getClusterShouldReturnRequestedClusterGivenAValidClusterId );
	CPPUNIT_TEST( buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraph );
	CPPUNIT_TEST( buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraphGivenALowQualityAbstraction );
	CPPUNIT_TEST( buildEntrancesShouldAskEachClusterToCreateItsOwnEntrances );
	CPPUNIT_TEST( buildEntrancesShouldResultInOneCachedPathForEachAbstractEdge );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeHasAbstractionLevelGreaterThanZero );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeIsNull );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphShouldConnectAnyNewNodesToAllOtherAbstractNodesInTheParentCluster );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldSet_kParent_LabelOfOriginalNodeEqualToIdIfANewNodeIsAddedToAbstractGraph );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldSet_kAbstractionLevel_LabelOfNewNodesToPointToTheCorrectAbstractGraph );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldAddToCacheAPathForEachNewlyCreatedEdge );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableStartNodeParameters, NodeIsHardObstacleException );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableGoalNodeParameters, NodeIsHardObstacleException );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldResetStartIDAndGoalIDToDefaultValues );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldNotDeleteAnyNodesOriginallyInTheAbstractGraph );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromParentClusters );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldResetToDefault_kParent_LabelOfOriginalNodesForWhichANewNodeWasInsertedIntoAbstractGraph );
	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllPathsAddedByInsertionMethod );
	CPPUNIT_TEST( distanceShouldCalculateTheWeightOfTheShortestPathBetweenTwoNodes );
	CPPUNIT_TEST( addPathToCacheShouldStoreAPathGivenAnEdge );
	CPPUNIT_TEST( addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull );
	CPPUNIT_TEST( getPathFromCacheShouldReturnAPathGivenAValidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnInvalidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache );
	CPPUNIT_TEST( hShouldProduceIdenticalResultsToOverriddenMethodInMapAbstractionGivenTwoValidNodeParameters );
	CPPUNIT_TEST_EXCEPTION( hShouldThrowExceptionGivenANullNodeParameter, NodeIsNullException );
	
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithTheSameCapabilityButDifferentClearance );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithIntersectingCapabilitySets );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWhereOneDominatesTheOtherBut__TO__and__FROM__AttributesOfEndpointsIsSwapped );

	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotUnmarkAnyEdgesWhichAreAlreadyMarked );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenTwoInteEdgesEitherOfWhichOrBothAreNull );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesDominantEdgeGivenTwoInterEdgesWithIntersectingCapabilityButNoCorridorSizeDominance );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenOneOrTwoInteredgesWhichReferenceInvalidNodeIDsAsTheirEndpoints );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithUniqueEndpointsWhichDoNotConnectTheSameSetOfClusters );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithSomeSharedEndpointsWhichDoNotConnectTheSameSetOfClusters );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfNoCircuitExistsBetweenEndpointsOfTwoTransitionsWhereOneDomiantesTheOther );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitDoNotHaveSufficentClearance );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitAreNotTraversableByTheCapabilityOfTheDominatedTransition );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfBothParameterEdgesAreIntraEdgesThatBelongToSameCluster );
	CPPUNIT_TEST( findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfEitherParameterIsAnIntraEdge );

	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldDeleteAllMarkedEdgesAndAnyEndpointsWhichAreNotRequiredByOtherInterEdges );
	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldDeleteAnyDominatedEdgeButNotAnyEndpointsWhichConnectOtherInterEdges );
	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldNotDeleteAnythingIfNoEdgesAreMarked );
	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldRemove__kParent__LabelOfLowLevelNodesThatWereAbstractedByADeletedNode );
	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldRepair__kParent__LabelsOfLowLevelsNodes );
	CPPUNIT_TEST( removeDominatedEdgesAndEndpointsShouldRepairAbstractNodeCollectionInEachAffectedCluster );


	CPPUNIT_TEST_SUITE_END();
	
	
	public:
		void setUp();
		void setupDominanceRelationshipTestData(int dmCapability, int dmClearance, int dtCapability, int dtClearance);
		void setupMockClusterExpectationsForBuildClusterTests(AnnotatedClusterMock*);
		void setupMockClusterExpectationsForBuildEntranceTests(AnnotatedClusterMock*);		
		void tearDown();
		
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();		
		void constructorShouldCreateANewGraphObject();
		void getClusterShouldReturnZeroWhenIdParameterIsLessThanZero();
		void getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters();
		void getClusterShouldReturnRequestedClusterGivenAValidClusterId();
		void buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void buildClustersShouldCalculateCorrectClusterSize();
		
		void buildEntrancesShouldAskEachClusterToCreateItsOwnEntrances();
		void buildEntrancesShouldResultInOneCachedPathForEachAbstractEdge();

		//////// INTEGRATION TESTS BELOW HERE (uses production code from other classes) ////////////
		void buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraph();
		void buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraphGivenALowQualityAbstraction();

		//void insertNodeIntoAbstractGraphShouldCloneALowLevelNodeAndAddItToTheLocalClusterAndAbstractGraph();
		void insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters();
		void insertStartAndGoalNodesIntoAbstractGraphShouldConnectAnyNewNodesToAllOtherAbstractNodesInTheParentCluster();
		void insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph();
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeHasAbstractionLevelGreaterThanZero();
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeIsNull();
		void insertStartAndGoalIntoAbstractGraphShouldSet_kParent_LabelOfOriginalNodeEqualToIdIfANewNodeIsAddedToAbstractGraph();
		void insertStartAndGoalIntoAbstractGraphShouldSet_kAbstractionLevel_LabelOfNewNodesToPointToTheCorrectAbstractGraph();
		void insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort();
		void insertStartAndGoalIntoAbstractGraphShouldAddToCacheAPathForEachNewlyCreatedEdge();
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableStartNodeParameters();
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableGoalNodeParameters();
				
		void removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph();
		void removeStartAndGoalNodesFromAbstractGraphShouldNotDeleteAnyNodesOriginallyInTheAbstractGraph();
		void removeStartAndGoalNodesFromAbstractGraphShouldResetStartIDAndGoalIDToDefaultValues();
		void removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromParentClusters();
		void removeStartAndGoalNodesFromAbstractGraphShouldResetToDefault_kParent_LabelOfOriginalNodesForWhichANewNodeWasInsertedIntoAbstractGraph();
		void removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllPathsAddedByInsertionMethod();
		
		void distanceShouldCalculateTheWeightOfTheShortestPathBetweenTwoNodes();
		
		void addPathToCacheShouldStoreAPathGivenAnEdge();
		void addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull();
		
		void getPathFromCacheShouldReturnAPathGivenAValidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnInvalidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache();

		void hShouldProduceIdenticalResultsToOverriddenMethodInMapAbstractionGivenTwoValidNodeParameters();
		void hShouldThrowExceptionGivenANullNodeParameter();
		
		/* positive dominance cases */
		void findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithTheSameCapabilityButDifferentClearance();
		void findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithIntersectingCapabilitySets();
		void findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWhereOneDominatesTheOtherBut__TO__and__FROM__AttributesOfEndpointsIsSwapped();
					
		/* negative dominance cases */
		void findAndMarkDominatedTransitionShouldNotUnmarkAnyEdgesWhichAreAlreadyMarked();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenTwoInteEdgesEitherOfWhichOrBothAreNull();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesDominantEdgeGivenTwoInterEdgesWithIntersectingCapabilityButNoCorridorSizeDominance();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenOneOrTwoInteredgesWhichReferenceInvalidNodeIDsAsTheirEndpoints();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithUniqueEndpointsWhichDoNotConnectTheSameSetOfClusters();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithSomeSharedEndpointsWhichDoNotConnectTheSameSetOfClusters();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfNoCircuitExistsBetweenEndpointsOfTwoTransitionsWhereOneDomiantesTheOther();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitDoNotHaveSufficentClearance();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitAreNotTraversableByTheCapabilityOfTheDominatedTransition();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfBothParameterEdgesAreIntraEdgesThatBelongToSameCluster();
		void findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfEitherParameterIsAnIntraEdge();
		
		void removeDominatedEdgesAndEndpointsShouldDeleteAnyDominatedEdgeButNotAnyEndpointsWhichConnectOtherInterEdges();
		void removeDominatedEdgesAndEndpointsShouldDeleteAllMarkedEdgesAndAnyEndpointsWhichAreNotRequiredByOtherInterEdges();
		void removeDominatedEdgesAndEndpointsShouldNotDeleteAnythingIfNoEdgesAreMarked();
		void removeDominatedEdgesAndEndpointsShouldRemove__kParent__LabelOfLowLevelNodesThatWereAbstractedByADeletedNode();
		void removeDominatedEdgesAndEndpointsShouldRepair__kParent__LabelsOfLowLevelsNodes();
		void removeDominatedEdgesAndEndpointsShouldRepairAbstractNodeCollectionInEachAffectedCluster();

		
	private:
		AnnotatedClusterAbstraction *aca;
		AnnotatedClusterMockFactory* acmock_factory;
		ExperimentManager* expmgr;
		Map *testmap;
		edge *e1, *e2, *intra1, *intra2;
		node *n1, *n2, *n3, *n4;
};

#endif