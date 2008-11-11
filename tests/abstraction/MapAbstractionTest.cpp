/*
 *  MapAbstractionTest.cpp
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "MapAbstractionTest.h"
#include "NodeFactoryMock.h"
#include "EdgeFactoryMock.h"
#include "map.h"
#include "TestConstants.h"
#include "mapAbstraction.h"
#include "graph.h";

CPPUNIT_TEST_SUITE_REGISTRATION( MapAbstractionTest );

void MapAbstractionTest::setUp()
{
	nf = new NodeFactoryMock();
	ef = new EdgeFactoryMock();
	m = new Map(emptymap.c_str());
}

void MapAbstractionTest::tearDown()
{
	delete m;
	delete nf;
	delete ef;
	nf = 0;
	ef = 0;
}

void MapAbstractionTest::getMapGraphShouldReturnANewGraphObjectGivenAValidMapAndANodeAndEdgeFactoryParameters()
{
	graph* g=0;
	int expectedNodes = 100;
	int expectedEdges = 342;
	
	g = getMapGraph(m, nf, ef);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("node factory invoked wrong number of times", expectedNodes, nf->count);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge factory invoked wrong number of times", expectedEdges, ef->count);
}