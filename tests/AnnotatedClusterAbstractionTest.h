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
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();

		void constructorShouldSplitTheMapAreaIntoCorrectNumberOfClusters();
		void getClusterSizeShouldReturnSameValueAsConstructorParameter();
		
		void buildAbstractGraphShouldCreateANewGraphObject();

		
		
		
		
	private:
		AnnotatedClusterAbstraction *aca;
		ExperimentManager* expmgr;
		Map *testmap;
};

#endif