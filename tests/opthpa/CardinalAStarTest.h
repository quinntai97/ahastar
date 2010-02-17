#ifndef CARDINALASTARTEST_H
#define CARDINALASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class CardinalAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( CardinalAStarTest );
	CPPUNIT_TEST( evaluateReturnsFalseWhenTwoNodesAreConnectedByANonCardinalEdge );
	CPPUNIT_TEST( evaluateReturnsTrueWhenTwoNodesAreConnectedByACardinalEdge );
	CPPUNIT_TEST( getPathReturnsAnOptimalLengthSolution );
	CPPUNIT_TEST( ItsGoTime );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void evaluateReturnsFalseWhenTwoNodesAreConnectedByANonCardinalEdge();
		void evaluateReturnsTrueWhenTwoNodesAreConnectedByACardinalEdge();
		void getPathReturnsAnOptimalLengthSolution();
		void ItsGoTime();
};

#endif

