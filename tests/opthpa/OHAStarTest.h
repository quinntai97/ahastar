// OHAStarTest.h
//
// @author: dharabor
// @created: 08-04-2010

#ifndef OHASTARTEST_H
#define OHASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class OHAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( OHAStarTest );
	CPPUNIT_TEST( relaxEdgeUpdatesPriorityOfNodeWithRespectToMacroParent );
	CPPUNIT_TEST( relaxEdgeUpdatesPriorityOfNodeAndSetsMacroParentToItselfGivenAnInterEdge );
	CPPUNIT_TEST( relaxEdgeUpdatesPriorityOfNodeThatHasNoMacroParent );
	CPPUNIT_TEST( getPathFindsAnOptimalPathInTheAbstractGraph );
	CPPUNIT_TEST( getPathFindsAnOptimalPathInTheLowLevelGraph );
	//CPPUNIT_TEST( extractBestPathWorksAsAdvertisedWhenPredecessorOfGoalIsItsMacroParent );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();

		void extractBestPathWorksAsAdvertisedWhenPredecessorOfGoalIsItsMacroParent();

		void relaxEdgeUpdatesPriorityOfNodeWithRespectToMacroParent();
		void relaxEdgeUpdatesPriorityOfNodeAndSetsMacroParentToItselfGivenAnInterEdge();
		void relaxEdgeUpdatesPriorityOfNodeThatHasNoMacroParent(); 
		void getPathFindsAnOptimalPathInTheAbstractGraph();
		void getPathFindsAnOptimalPathInTheLowLevelGraph();
};

#endif

