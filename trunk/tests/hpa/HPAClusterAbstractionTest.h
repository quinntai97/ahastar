/*
 *  HPAClusterAbstractionTest.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERABSTRACTIONTEST_H
#define HPACLUSTERABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HPAClusterAbstraction.h"

using namespace CppUnit;

class ClusterAStarFactory;
class ClusterNodeFactory;
class HPAClusterFactory;
class EdgeFactory;
class HPAClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAClusterAbstractionTest );
	CPPUNIT_TEST( constructorShouldCreateANewGraphObject );
	CPPUNIT_TEST_EXCEPTION( constructorThrowsExceptionGivenAnIncompatibleNodeFactoryParameter, std::invalid_argument );
	
	CPPUNIT_TEST( buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters );
	CPPUNIT_TEST( buildClustersShouldCalculateCorrectClusterSize );
	
	CPPUNIT_TEST( getClusterSizeShouldReturnSameValueAsConstructorParameter );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsLessThanZero );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters );
	CPPUNIT_TEST( getClusterShouldReturnRequestedClusterGivenAValidClusterId );
	//CPPUNIT_TEST( distanceShouldCalculateTheWeightOfTheShortestPathBetweenTwoNodes ); // duplicates graphAbstraction::distance; junk; not required.

	CPPUNIT_TEST( addPathToCacheShouldStoreAPathGivenAnEdge );
	CPPUNIT_TEST( addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull );
	CPPUNIT_TEST( getPathFromCacheShouldReturnAPathGivenAValidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnInvalidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache );

	CPPUNIT_TEST( removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullStartParameter, std::invalid_argument );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullGoalParameter, std::invalid_argument );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractStartParameter, std::invalid_argument );
	CPPUNIT_TEST_EXCEPTION( insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractGoalParameter, std::invalid_argument );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters );
	CPPUNIT_TEST( insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph );
	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort );
	
	CPPUNIT_TEST( buildEntrancesCallsBuildEntranceMethodOnEachCluster );
	
/*	CPPUNIT_TEST( hShouldProduceIdenticalResultsToOverriddenMethodInMapAbstractionGivenTwoValidNodeParameters );
	CPPUNIT_TEST_EXCEPTION( hShouldThrowExceptionGivenANullNodeParameter, NodeIsNullException );
*/


	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		void constructorShouldCreateANewGraphObject();
		void constructorThrowsExceptionGivenAnIncompatibleNodeFactoryParameter();
		
		void buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void buildClustersShouldCalculateCorrectClusterSize();
		
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();		
		void getClusterShouldReturnZeroWhenIdParameterIsLessThanZero();
		void getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters();
		void getClusterShouldReturnRequestedClusterGivenAValidClusterId();
		
		void addPathToCacheShouldStoreAPathGivenAnEdge();
		void addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull();
		void getPathFromCacheShouldReturnAPathGivenAValidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnInvalidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache();
		
		void removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph();
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullStartParameter() throw(std::invalid_argument);
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullGoalParameter() throw(std::invalid_argument);
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractStartParameter() throw(std::invalid_argument);
		void insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractGoalParameter() throw(std::invalid_argument);
		void insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters();
		void insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph();
		void insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort();
		
		void buildEntrancesCallsBuildEntranceMethodOnEachCluster();
		

	private:
		ClusterNodeFactory *nf;
		EdgeFactory *ef;
		HPAClusterFactory *cf;
		ClusterAStarFactory *caf;
};

#endif