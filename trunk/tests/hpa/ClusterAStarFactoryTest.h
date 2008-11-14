/*
 *  ClusterAStarFactoryTest.h
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERASTARFACTORYTEST_H
#define CLUSTERASTARFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ClusterAStarFactory.h"

class ClusterAStarFactoryTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( ClusterAStarFactoryTest );
	CPPUNIT_TEST( newClusterAStarShouldReturnANewInstanceOfClusterAStar );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void newClusterAStarShouldReturnANewInstanceOfClusterAStar();
		
};

#endif