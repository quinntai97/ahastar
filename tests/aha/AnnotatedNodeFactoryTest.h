/*
 *  AnnotatedNodeFactoryTest.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDNODEFACTORYTEST_H
#define ANNOTATEDNODEFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class AnnotatedNodeFactory;

using namespace CppUnit;

class AnnotatedNodeFactoryTest : public CPPUNIT_NS::TestFixture
{

	public:
		CPPUNIT_TEST_SUITE( AnnotatedNodeFactoryTest );
		CPPUNIT_TEST( newNodeShouldConstructANewAnnotatedNodeObject );
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void newNodeShouldConstructANewAnnotatedNodeObject();

	private:
		AnnotatedNodeFactory* anf;

};

#endif