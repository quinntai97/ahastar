/*
 *  ClusterAStarFactoryTest.cpp
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterAStarFactoryTest.h"
#include "ClusterAStar.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ClusterAStarFactoryTest );

void ClusterAStarFactoryTest::setUp()
{
}

void ClusterAStarFactoryTest::tearDown()
{
}

void ClusterAStarFactoryTest::newClusterAStarShouldReturnANewInstanceOfClusterAStar()
{	
	ClusterAStarFactory caf;
	ClusterAStar* ac = dynamic_cast<ClusterAStar*>(caf.newClusterAStar());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("factory failed to return an instance of ClusterAStar", true, ac!=0);
	delete ac;
}
