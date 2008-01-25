/*
 *  AnnotatedAStarTest.h
 *  hog
	Tests:
		checkPath: only visit locations that are valid given a set of terrain and size constraints
		- Need to compare performance with A*:
			- Require some well thought out maps; if start/goal are of the same terrain type, they must be pathable. 
			- Run aa* and a* to see if the results are the same for single-terrain problems (no extra edges)
			- Where multi-terrain problems must be solved, need some control over whether or not extra edges are added
			to the map / when  (add extra edges; maybe need a removeedge function?!)
		
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTARTEST_H
#define ANNOTATEDASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class AnnotatedMapAbstraction;
class AnnotatedAStar;

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