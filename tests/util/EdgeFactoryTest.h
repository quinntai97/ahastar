/*
 *  EdgeFactoryTest.h
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EDGEFACTORYTEST_H
#define EDGEFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class EdgeFactory;

using namespace CppUnit;

class EdgeFactoryTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( EdgeFactoryTest );
	CPPUNIT_TEST( newEdgeShouldConstructANewEdgeObject );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void newEdgeShouldConstructANewEdgeObject();
		
	private:
		EdgeFactory* nf;
};

#endif