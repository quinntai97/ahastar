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
	CPPUNIT_TEST( ValidateAnnotationsTest );

	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent );
	
/*	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForSmallMultiTerrainAgent );
	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForLargeMultiTerrainAgent );
	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsTrueWhenValidPathExistsForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenNoValidPathExistsForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenStartIsNotTraversableForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenStartIsNotTraversableForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenGoalIsNotTraversableForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenGoalIsNotTraversableForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenStartIsNotValidMapLocationForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenStartIsNotValidMapLocationForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenGoalIsNotValidMapLocationForSmallSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsFalseWhenGoalIsNotValidMapLocationForLargeSingleTerrainAgent );
	CPPUNIT_TEST( PathableReturnsTrueWhenStartAndGoalAreIdentical );
*/
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void ValidateAnnotationsTest();		
/*		void PathableReturnsTrueWhenValidPathExistsForSmallMultiTerrainAgent();
		void PathableReturnsTrueWhenValidPathExistsForLargeMultiTerrainAgent();
		void PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent();
		void PathableReturnsTrueWhenValidPathExistsForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenNoValidPathExistsForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenStartIsNotTraversableForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenStartIsNotTraversableForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenGoalIsNotTraversableForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenGoalIsNotTraversableForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenStartIsNotValidMapLocationForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenStartIsNotValidMapLocationForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenGoalIsNotValidMapLocationForSmallSingleTerrainAgent();
		void PathableReturnsFalseWhenGoalIsNotValidMapLocationForLargeSingleTerrainAgent();
		void PathableReturnsTrueWhenStartAndGoalAreIdentical();
*/ 

		void PathableReturnsTrueWhenValidPathExistsForLargeSingleTerrainAgent();
		void PathableReturnsFalseWhenNoValidPathExistsForLargeSingleTerrainAgent();

	private:
		void runExperiment(ExpMgrUtil::ExperimentKey);
		void checkSingleNodeAnnotations(node*, int, int);

		AnnotatedMapAbstraction *ama;
		ExperimentManager* expmgr;
		graph *g;
		Map* testmap;
};

#endif