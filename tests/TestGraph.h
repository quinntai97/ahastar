/*
 *  TestGraph.h
 *  hog
 *
 *  Created by Daniel Harabor on 26/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "graph.h"
#include "map.h"

using namespace CppUnit;

class TestGraph : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestGraph );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnAnExistingEdgeGivenACapabilityParameterWhichSupersetsTheCapabilityOfAnExistingEdge );
  CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void findAnnotatedEdgeShouldReturnAnExistingEdgeGivenACapabilityParameterWhichSupersetsTheCapabilityOfAnExistingEdge();

private:
	graph *g;

};

#endif