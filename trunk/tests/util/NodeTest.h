/*
 *  NodeTest.h
 *  hog
 *
 *	Tests for HOG's "Node" class. Focus:
		- Test Annotations; each node can be annotated with clearance values for different types of terrain. 
		- Test TerrainType; each node is annotated with the terrain type of the corresponding map tile it represents.
		
 *  Created by Daniel Harabor on 28/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NODETEST_H
#define NODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

class node;
class graph;
class edge;

class NodeTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( NodeTest );
  CPPUNIT_TEST( cloneShouldDeepCopyNodeAndAllLabels );
  CPPUNIT_TEST( cloneShouldNotDeepCopyEdges );
  CPPUNIT_TEST( copyConstructorShouldDeepCopyNodeAndAllLabels );
  CPPUNIT_TEST( copyConstructorShouldNotDeepCopyEdges );

  CPPUNIT_TEST_SUITE_END();

public:
	/* fixture stuff */
	void setUp();
	void tearDown();

	void cloneShouldDeepCopyNodeAndAllLabels();
	void cloneShouldNotDeepCopyEdges();
	void copyConstructorShouldDeepCopyNodeAndAllLabels();
	void copyConstructorShouldNotDeepCopyEdges();


private:
	graph* g;
	node *n;
};

#endif