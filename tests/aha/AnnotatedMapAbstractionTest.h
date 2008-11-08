/*
 *  AnnotatedMapTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 3/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDMAPABSTRACTIONTEST_H
#define ANNOTATEDMAPABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <AnnotatedMapAbstraction.h>
#include "ExperimentManager.h"

class AnnotatedMapAbstractionTest: public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedMapAbstractionTest );
//	CPPUNIT_TEST( ValidateAnnotationsTest );

	CPPUNIT_TEST( nodeClearanceIsZeroForCapabilityThatDoesNotIncludeTheNodeTerrainType );
	CPPUNIT_TEST( nodeClearanceZeroWhenTerrainIsHardObstacle );
	CPPUNIT_TEST( nodeClearanceMinimumClearanceWhenTerrainIsValidButNoNeighbours );
	CPPUNIT_TEST( nodeClearanceMinimumClearanceWhenTerrainIsValidButAllNeighboursHardObstacles );
	CPPUNIT_TEST( nodeClearanceEqualsMinimumOfAllNeighboursPlusOne );
	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent );
	CPPUNIT_TEST( checkNodeAnnotationsAgainstExpectations );
	
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void nodeClearanceZeroWhenTerrainIsHardObstacle();
		void nodeClearanceMinimumClearanceWhenTerrainIsValidButNoNeighbours();
		void nodeClearanceMinimumClearanceWhenTerrainIsValidButAllNeighboursHardObstacles();
		void nodeClearanceEqualsMinimumOfAllNeighboursPlusOne();
		void nodeClearanceIsZeroForCapabilityThatDoesNotIncludeTheNodeTerrainType();
		
		void ValidateAnnotationsTest();	
		void checkNodeAnnotationsAgainstExpectations();

		void PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent();

	private:
		void runExperiment(ExpMgrUtil::ExperimentKey);
		void checkSingleNodeAnnotations(node*, int, int);
		void setupNeighbourAnnotations(int x, int y);

		AnnotatedMapAbstraction *ama;
		ExperimentManager* expmgr;
		graph *g;
		Map* testmap;
		int nclearance;
};

#endif