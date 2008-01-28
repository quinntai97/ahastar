/*
 *  AnnotatedAStarTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedAStarTest.h"
#include "AnnotatedAStar.h"
#include "AnnotatedMapAbstractionMock.h"
#include "AnnotatedAStarMock.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedAStarTest );

void AnnotatedAStarTest::setUp()
{
	amamock = new AnnotatedMapAbstractionMock(new Map(maplocation.c_str()), new AnnotatedAStarMock());
	aastar = new AnnotatedAStar();
	aastar->setGraphAbstraction(amamock);
	aastar->setMinClearance(2);
	aastar->setSearchTerrain(4);


	n=s=e=w=ne=nw=se=sw=NULL;
	testedge = NULL;
}

void AnnotatedAStarTest::tearDown()
{
	delete amamock;
	delete aastar;
	
	if(pos != NULL)
		delete pos;
	if(n != NULL)
		delete n;
	if(s != NULL)
		delete s;
	if(e != NULL)
		delete e;
	if(w != NULL)
		delete w;
	if(ne != NULL)
		delete ne;
	if(nw != NULL)
		delete nw;
	if(se != NULL)
		delete se;
	if(sw != NULL)
		delete sw;
	if(testedge != NULL)
		delete testedge;
}

void AnnotatedAStarTest::evaluateMoveNorthLST()
{
	/* testing agent of size=2 moving north (1,1 to 1,0) over terrain=kGround */
	n = getNode(1, 0, kGround);
	pos = getNode(1, 1, kGround);
	testedge = new edge(pos->getNum(), n->getNum(), 1.0);
		
	bool result = aastar->evaluate(pos, n, testedge);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move north", true, result); 
}

void AnnotatedAStarTest::evaluateMoveSouthLST()
{
	/* testing agent of size=2 moving north (1,1 to 1,0) over terrain=kGround */
	n = getNode(1, 2, kGround);
	pos = getNode(1, 1, kGround);
	testedge = new edge(pos->getNum(), n->getNum(), 1.0);
		
	bool result = aastar->evaluate(pos, n, testedge);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move north", true, result); 
}

void AnnotatedAStarTest::evaluateMoveEastLST()
{
	/* testing agent of size=2 moving north (1,1 to 1,0) over terrain=kGround */
	n = getNode(2, 1, kGround);
	pos = getNode(1, 1, kGround);
	testedge = new edge(pos->getNum(), n->getNum(), 1.0);
		
	bool result = aastar->evaluate(pos, n, testedge);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move north", true, result); 
}

void AnnotatedAStarTest::evaluateMoveWestLST()
{
	/* testing agent of size=2 moving north (1,1 to 1,0) over terrain=kGround */
	n = getNode(0, 1, kGround);
	pos = getNode(1, 1, kGround);
	testedge = new edge(pos->getNum(), n->getNum(), 1.0);
		
	bool result = aastar->evaluate(pos, n, testedge);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move north", true, result); 
}

void AnnotatedAStarTest::evaluateMoveNorthWestLST()
{
	/* testing agent of size=2 moving north (1,1 to 1,0) over terrain=kGround */
	n = getNode(0, 0, kGround);
	pos = getNode(1, 1, kGround);
	testedge = new edge(pos->getNum(), n->getNum(), ROOT_TWO);
		
	bool result = aastar->evaluate(pos, n, testedge);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move north", true, result); 
}



void AnnotatedAStarTest::annotateNode(node* n, int t1, int t1c, int t2, int t2c, int t3, int t3c)
{
	n->setTerrainType(t1);
	n->setClearance(t1,t1c);
	n->setClearance(t2,t2c);
	n->setClearance(t3,t3c);

}

node* AnnotatedAStarTest::getNode(int x, int y, int nodeterrain)
{
	node* n = new node("test");
	n->setTerrainType(nodeterrain);
	n->setLabelL(kFirstData, x);
	n->setLabelL(kFirstData+1, y);
	annotateNode(n, kGround, 2, kTrees, 0, (kGround|kTrees), 0);

	return n;
}