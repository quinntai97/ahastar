/*
 *  TestNode.h
 *  hog
 *
 *	Tests for HOG's "Node" class. Focus:
		- Test Annotations; each node can be annotated with clearance values for different types of terrain. 
		- Test TerrainType; each node is annotated with the terrain type of the corresponding map tile it represents.
		
 *  Created by Daniel Harabor on 28/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TESTNODE_H
#define TESTNODE_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "graph.h"
#include "map.h"

using namespace CppUnit;

class TestNode : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TestNode );
  CPPUNIT_TEST( TerrainAnnotationsTest );
  CPPUNIT_TEST( setClearanceFailsWhenNodeTerrainNotValid );
  CPPUNIT_TEST( setParentClusterFailsWhenClusterIdLessThanZero );
  CPPUNIT_TEST( setParentClusterStoresClusterIdWhenClusterIdMoreThanZero );
  CPPUNIT_TEST( setParentClusterStoresClusterIdWhenClusterIdEqualsZero );
  CPPUNIT_TEST( cloneShouldDeepCopyNodeAndAllAnnotationsExceptParentClusterId );
  CPPUNIT_TEST( cloneShouldNotDeepCopyEdges );
  CPPUNIT_TEST_SUITE_END();

public:
	/* fixture stuff */
	void setUp();
	void tearDown();

	/* test cases */
	void TerrainAnnotationsTest();
	void setClearanceFailsWhenNodeTerrainNotValid();
	void setParentClusterFailsWhenClusterIdLessThanZero();
	void setParentClusterStoresClusterIdWhenClusterIdMoreThanZero();
	void setParentClusterStoresClusterIdWhenClusterIdEqualsZero();
	void cloneShouldDeepCopyNodeAndAllAnnotationsExceptParentClusterId();
	void cloneShouldNotDeepCopyEdges();

private:
	/* test data */
	node *n;
	int terrains[3];
	int clval[4];

};

#endif