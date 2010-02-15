#ifndef EMPTYCLUSTERABSTRACTIONTEST_H
#define EMPTYCLUSTERABSTRACTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class EmptyClusterAbstractionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( EmptyClusterAbstractionTest );

	CPPUNIT_TEST( buildClustersDecomposesTheMapIntoEmptyClusters );

	CPPUNIT_TEST_SUITE_END();

	public:
		
		void setUp();
		void tearDown();

		void buildClustersDecomposesTheMapIntoEmptyClusters();

};

#endif

