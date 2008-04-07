/*
 *  AnnotatedHierarchicalAStarTest.h
 *  hog
		
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDHIERARCHICALASTARTEST_H
#define ANNOTATEDHIERARCHICALASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ExperimentManager.h"

class ExperimentManager;
class AnnotatedClusterAbstractionMock;
class AnnotatedHierarchicalAStar;
class node;
class edge;

class AnnotatedHierarchicalAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedHierarchicalAStarTest );

	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		
		
		
	private:
		AnnotatedClusterAbstractionMock* acamock;
		AnnotatedHierarchicalAStar* aastar;
		node  *pos, *n;	
};

#endif