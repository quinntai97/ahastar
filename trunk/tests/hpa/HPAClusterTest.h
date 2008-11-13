/*
 *  HPAClusterTest.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERTEST_H
#define HPACLUSTERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "HPACluster.h"

class HPAClusterTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAClusterTest );
	CPPUNIT_TEST( hasaParentReturnsTrueGivenANodeWithTheSameCoordinatesAsTheParameterNode );
	CPPUNIT_TEST( hasaParentReturnsFalseGivenANodeWithDifferentCoordinatesFromTheParameterNode );
	//	CPPUNIT_TEST( insertStartAndGoalIntoAbstractGraphShouldAddToCacheAPathForEachNewlyCreatedEdge );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void hasaParentReturnsTrueGivenANodeWithTheSameCoordinatesAsTheParameterNode();
		void hasaParentReturnsFalseGivenANodeWithDifferentCoordinatesFromTheParameterNode();
};

#endif