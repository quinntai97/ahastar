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

class HPAClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAClusterAbstractionTest );
	CPPUNIT_TEST( buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters );
	CPPUNIT_TEST( buildClustersShouldCalculateCorrectClusterSize );
	CPPUNIT_TEST( getClusterSizeShouldReturnSameValueAsConstructorParameter );
	CPPUNIT_TEST( constructorShouldCreateANewGraphObject );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsLessThanZero );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters );
	CPPUNIT_TEST( getClusterShouldReturnRequestedClusterGivenAValidClusterId );
	//CPPUNIT_TEST( distanceShouldCalculateTheWeightOfTheShortestPathBetweenTwoNodes ); // duplicates graphAbstraction::distance; junk; not required.
	CPPUNIT_TEST( addPathToCacheShouldStoreAPathGivenAnEdge );
	CPPUNIT_TEST( addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull );
	CPPUNIT_TEST( getPathFromCacheShouldReturnAPathGivenAValidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnInvalidEdge );
	CPPUNIT_TEST( getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache );
/*	CPPUNIT_TEST( hShouldProduceIdenticalResultsToOverriddenMethodInMapAbstractionGivenTwoValidNodeParameters );
	CPPUNIT_TEST_EXCEPTION( hShouldThrowExceptionGivenANullNodeParameter, NodeIsNullException );
*/


	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		void buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void buildClustersShouldCalculateCorrectClusterSize();
		
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();
		void constructorShouldCreateANewGraphObject();
		void getClusterShouldReturnZeroWhenIdParameterIsLessThanZero();
		void getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters();
		void getClusterShouldReturnRequestedClusterGivenAValidClusterId();
		
		void addPathToCacheShouldStoreAPathGivenAnEdge();
		void addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull();
		void getPathFromCacheShouldReturnAPathGivenAValidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnInvalidEdge();
		void getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache();
		
	private:
//		HPAClusterAbstraction* hpacaMap;
};

#endif