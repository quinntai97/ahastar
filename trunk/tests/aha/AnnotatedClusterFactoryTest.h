/*
 *  AnnotatedClusterFactoryTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 31/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERFACTORYTEST_H
#define ANNOTATEDCLUSTERFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "AnnotatedClusterFactory.h"

class AnnotatedClusterFactoryTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedClusterFactoryTest );
	CPPUNIT_TEST( createClusterShouldReturnANewInstanceOfAnnotatedCluster );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void createClusterShouldReturnANewInstanceOfAnnotatedCluster();
		
	private:
		AnnotatedClusterFactory* acf;
};

#endif