/*
 *  ClusterNodeTest.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERNODETEST_H
#define CLUSTERNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ClusterNode.h"

class ClusterNodeTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ClusterNodeTest );
	CPPUNIT_TEST( cloneReturnsACopyOfTheCurrentNode );
	CPPUNIT_TEST( copyConstructorReturnsACopyOfTheCurrentNode );
	CPPUNIT_TEST( constructorInitialises_kParent_Label );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void cloneReturnsACopyOfTheCurrentNode();
		void copyConstructorReturnsACopyOfTheCurrentNode();	
		void constructorInitialises_kParent_Label();
};

#endif