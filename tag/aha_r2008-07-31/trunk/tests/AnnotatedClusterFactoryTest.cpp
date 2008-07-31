/*
 *  AnnotatedClusterFactoryTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 31/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterFactoryTest.h"
#include "AnnotatedCluster.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterFactoryTest );

void AnnotatedClusterFactoryTest::setUp()
{
	acf = new AnnotatedClusterFactory();
}

void AnnotatedClusterFactoryTest::tearDown()
{
	delete acf;
}

void AnnotatedClusterFactoryTest::createClusterShouldReturnANewInstanceOfAnnotatedCluster()
{	
	int x=0, y=0, cheight=5, cwidth=5;	
	AnnotatedCluster* ac = dynamic_cast<AnnotatedCluster*>(acf->createCluster(x, y, cwidth, cheight));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("factory failed to return an instance of AnnotatedCluster", true, ac!=0);
}
