/*
 *  AnnotatedAStarTest.h
 *  hog
	Tests:
		checkPath: only visit locations that are valid given a set of terrain and size constraints
		- Need to compare performance with A*:
			- Require some well thought out maps; if start/goal are of the same terrain type, they must be pathable. 
			- Run aa* and a* to see if the results are the same for single-terrain problems (no extra edges)
			- Where multi-terrain problems must be solved, need some control over whether or not extra edges are added
			to the map / when  (add extra edges; maybe need a removeedge function?!)
		
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTARTEST_H
#define ANNOTATEDASTARTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ExperimentManager.h"

class ExperimentManager;
class AnnotatedMapAbstractionMock;
class AnnotatedAStar;
class node;
class edge;

class AnnotatedAStarTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( AnnotatedAStarTest );
	CPPUNIT_TEST( evaluateMoveNorthLST );
	CPPUNIT_TEST( evaluateMoveSouthLST );
	CPPUNIT_TEST( evaluateMoveEastLST );
	CPPUNIT_TEST( evaluateMoveWestLST );
	CPPUNIT_TEST( evaluateMoveNorthWestLST );
	CPPUNIT_TEST( evaluateMoveNorthEastLST );
	CPPUNIT_TEST( evaluateMoveSouthEastLST );
	CPPUNIT_TEST( evaluateMoveSouthWestLST );

	//CPPUNIT_TEST( getPathNonAnnotatedMapAbstractionParameter );
	CPPUNIT_TEST_SUITE_END();


	
	public:
		void setUp();
		void tearDown();
		
		void evaluateMoveNorthLST();
		void evaluateMoveSouthLST();
		void evaluateMoveEastLST();
		void evaluateMoveWestLST();
		void evaluateMoveNorthWestLST();
		void evaluateMoveNorthEastLST();
		void evaluateMoveSouthEastLST();
		void evaluateMoveSouthWestLST();

		//void evaluateMoveToCurrentLocationLST(); //ie. pass the same node as the pos & target
		// void evaluateMoveToHardObstacle
		// void evaluateMoveToSoftObstacle

		//void getPathNonAnnotatedMapAbstractionParameter(); 
		
	private:
		void annotateNode(node* n, int t1, int t1c, int t2, int t2c, int t3, int t3c);
		node* getNode(int x, int y, int nodeterrain);
		void runEvaluateTest(ExpMgrUtil::TestExperiment*);

		ExperimentManager* expmgr;
		AnnotatedMapAbstractionMock* amamock;
		AnnotatedAStar* aastar;
		node  *pos, *n, *s, *e, *w, *ne, *nw, *se, *sw;
		edge* testedge;
	
};

#endif