/*
 *  MapAbstractionTest.h
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MAPABSTRACTIONTEST_H
#define MAPABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class NodeFactoryMock;
class EdgeFactoryMock;
class Map;

using namespace CppUnit;
using namespace std;

class MapAbstractionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( MapAbstractionTest );
	CPPUNIT_TEST( getMapGraphShouldReturnANewGraphObjectGivenAValidMapAndANodeAndEdgeFactoryParameters );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void getMapGraphShouldReturnANewGraphObjectGivenAValidMapAndANodeAndEdgeFactoryParameters();
		
	private:
		NodeFactoryMock *nf;
		EdgeFactoryMock *ef;
		Map* m;
};

#endif
