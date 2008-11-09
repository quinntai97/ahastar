/*
 *  AnnotatedEdgeFactoryTest.h
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDEDGEFACTORYTEST_H
#define ANNOTATEDEDGEFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class AnnotatedEdgeFactory;

using namespace CppUnit;

class AnnotatedEdgeFactoryTest : public CPPUNIT_NS::TestFixture
{

	public:
		CPPUNIT_TEST_SUITE( AnnotatedEdgeFactoryTest );
		CPPUNIT_TEST( newEdgeShouldConstructANewAnnotatedEdgeObject );
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void newEdgeShouldConstructANewAnnotatedEdgeObject();

	private:
		AnnotatedEdgeFactory* aef;

};

#endif