/*
 *  HPAClusterFactoryTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 31/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterFactoryTest.h"
CPPUNIT_TEST_SUITE_REGISTRATION( HPAClusterFactoryTest );

void HPAClusterFactoryTest::setUp()
{
	cf = new HPAClusterFactory();
}

void HPAClusterFactoryTest::tearDown()
{
	delete cf;
}

void HPAClusterFactoryTest::createHPAClusterShouldReturnANewInstanceOfHPACluster()
{	
	int x=0, y=0, cheight=5, cwidth=5;	
	HPACluster* ac = dynamic_cast<HPACluster*>(cf->createCluster(x, y, cwidth, cheight));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("factory failed to return an instance of HPACluster", true, ac!=0);
}
