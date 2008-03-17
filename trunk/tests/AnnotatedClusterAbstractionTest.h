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
	CPPUNIT_TEST( buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters );
	CPPUNIT_TEST( getClusterSizeShouldReturnSameValueAsConstructorParameter );
	CPPUNIT_TEST( constructorShouldCreateANewGraphObject );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsLessThanZero );
	CPPUNIT_TEST( getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters );
	CPPUNIT_TEST( getClusterShouldReturnRequestedClusterGivenAValidClusterId );
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();

		void buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();
		
		void constructorShouldCreateANewGraphObject();
		void getClusterShouldReturnZeroWhenIdParameterIsLessThanZero();
		void getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters();
		void getClusterShouldReturnRequestedClusterGivenAValidClusterId();
		
	private:
		AnnotatedClusterAbstraction *aca;
		ExperimentManager* expmgr;
		Map *testmap;
};

#endif