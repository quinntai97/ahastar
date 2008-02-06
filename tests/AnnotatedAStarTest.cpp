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
#include "ExperimentManager.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedAStarTest );

using namespace ExpMgrUtil;

void AnnotatedAStarTest::setUp()
{
	amamock = new AnnotatedMapAbstractionMock(new Map(maplocation.c_str()), new AnnotatedAStarMock());
	aastar = new AnnotatedAStar();
	aastar->setGraphAbstraction(amamock);
	aastar->setMinClearance(2);
	aastar->setSearchTerrain(4);


	n=s=e=w=ne=nw=se=sw=NULL;
	testedge = NULL;
	expmgr = new ExperimentManager();
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
	ExpMgrUtil::TestExperiment* te = expmgr->getExperiment(kPathableMoveNorthLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveSouthLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveSouthLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveEastLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveEastLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveWestLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveWestLST);
	runEvaluateTest(te);

}

void AnnotatedAStarTest::evaluateMoveNorthWestLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveNorthWestLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveNorthEastLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveNorthEastLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveSouthEastLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveSouthEastLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveSouthWestLST()
{
	TestExperiment* te = expmgr->getExperiment(kPathableMoveSouthWestLST);
	runEvaluateTest(te);
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

	return n;
}

void AnnotatedAStarTest::runEvaluateTest(TestExperiment* exp)
{
	amamock->setCurrentTestExperiment(exp);
	pos = getNode(exp->startx, exp->starty, exp->terrain);
	pos->setClearance(exp->terrain, exp->size);
	n = getNode(exp->goalx, exp->goaly, exp->terrain);
	n->setClearance(exp->terrain, exp->size);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move", exp->pathable, aastar->evaluate(pos, n)); 

}