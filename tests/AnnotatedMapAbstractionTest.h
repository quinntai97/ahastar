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

//#include <mapAbstraction.h>
#include <AnnotatedMapAbstraction.h>
#include "TestHelper.h"

class AnnotatedMapAbstractionTest: public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedMapAbstractionTest );
	CPPUNIT_TEST( ValidateAnnotationsTest );
	CPPUNIT_TEST( PathableTest );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		/* tests */
		void ValidateAnnotationsTest();
		void PathableTest();
	
	private:
		void checkSingleNodeAnnotations(node*, int, int);

		AnnotatedMapAbstraction *ama;
		graph *g;
		Map* testmap;
};

#endif