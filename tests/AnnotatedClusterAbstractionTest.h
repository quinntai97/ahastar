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

class AnnotatedClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( AnnotatedClusterAbstractionTest );
	CPPUNIT_TEST( buildClusterShouldReturnNullGivenAnInvalidMapCoordinate );
	CPPUNIT_TEST( buildClusterShouldReturnAClusterAndIncrementNumberOfClustersGivenAValidMapCoordinate );
	CPPUNIT_TEST( buildClusterShouldReturnNullGivenAValidMapCoordinateAndTerrainIsAHardObstacle );
	//CPPUNIT_TEST( addNodesToClusterAssignsAllNodesInAreaMarkedByDimensions );
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		void buildClusterShouldReturnNullGivenAnInvalidMapCoordinate();
		void buildClusterShouldReturnAClusterAndIncrementNumberOfClustersGivenAValidMapCoordinate();
		void buildClusterShouldReturnNullGivenAValidMapCoordinateAndTerrainIsAHardObstacle();
		void buildClusterShouldReturnClusterWithNodesAssignedGivenAValidMapCoordinate();
		void addNodesToClusterAssignsAllNodesInAreaMarkedByDimensions();
		
	private:
		//void experimentInit(const std::string &maploc);
		
		AnnotatedClusterAbstraction *aca;
		ExperimentManager* expmgr;
		Map *testmap;
};

#endif