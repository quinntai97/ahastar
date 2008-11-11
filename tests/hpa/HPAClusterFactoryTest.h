/*
 *  HPAClusterFactoryTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 31/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERFACTORYTEST_H
#define HPACLUSTERFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "HPAClusterFactory.h"

class HPAClusterFactoryTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( HPAClusterFactoryTest );
	CPPUNIT_TEST( createHPAClusterShouldReturnANewInstanceOfHPACluster );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void createHPAClusterShouldReturnANewInstanceOfHPACluster();
		
	private:
		HPAClusterFactory* cf;
};

#endif