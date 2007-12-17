/*
 *  AnnotatedAStarTest.h
 *  hog
	Tests:
		checkPath: only visit locations that are valid given a set of terrain and size constraints
		
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTARTEST_H
#define ANNOTATEDASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"

class AnnotatedAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedAStarTest );
	CPPUNIT_TEST( CheckPath );
	CPPUNIT_TEST_SUITE_END();

	
	public:
		void setUp();
		void tearDown();
		
		void CheckPath();
		
	private:
		AnnotatedMapAbstraction* ama;
		AnnotatedAStar* aastar;
	
};

#endif