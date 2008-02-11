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

	pos = n = NULL;
	expmgr = new ExperimentManager();
}

void AnnotatedAStarTest::tearDown()
{
	delete amamock;
	delete aastar;
	delete expmgr;
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

void AnnotatedAStarTest::evaluateMoveToHardObstacle()
{
	TestExperiment* te = expmgr->getExperiment(kNotPathableMoveToHardObstacleLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveToSoftObstacle()
{
	TestExperiment* te = expmgr->getExperiment(kNotPathableMoveToSoftObstacleLST);
	runEvaluateTest(te);
}

void AnnotatedAStarTest::evaluateMoveToNonAdjacentLocation()
{
	TestExperiment* te = expmgr->getExperiment(kNotPathableMoveToNonAdjacentLocationLST);
	runEvaluateTest(te);

}

void AnnotatedAStarTest::getPathStartClearanceTooSmall()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableStartClearanceTooSmallLST);
	string errmsg("getPath() failed to return null when start clearance too small");
	pos = te->getStartNode();
	pos->setClearance(te->sterrain, te->size-1);
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathGoalClearanceTooSmall()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableGoalClearanceTooSmallLST);
	string errmsg("getPath() failed to return null when goal clearance too small");
	n = te->getGoalNode();
	n->setClearance(te->gterrain, te->size-1);
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathReturnNullWhenStartOrGoalNull()
{	
	n = getNode(0,0,4);
	path* p = aastar->getPath(amamock, pos, n, 4, 2); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start node is null", int(p), NULL);

	p = aastar->getPath(amamock, n, pos, 4, 2); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when goal node is null", int(p), NULL);
	
	delete n;
}

void AnnotatedAStarTest::getPathReturnNullWhenStartHardObstacle()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableStartIsHardObstacleLST);
	string errmsg("getPath() failed to return null when start is a hard obstacle");
	runGetPathTest(te, errmsg);
}


void AnnotatedAStarTest::getPathReturnNullWhenGoalHardObstacle()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableGoalIsHardObstacleLST);
	string errmsg("getPath() failed to return null when goal is a hard obstacle");
	runGetPathTest(te, errmsg);
}


void AnnotatedAStarTest::getPathReturnNullWhenStartSoftObstacle()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableStartIsSoftObstacleLST);
	string errmsg("getPath() failed to return null when start is a soft obstacle");
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathReturnNullWhenGoalSoftObstacle()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableGoalIsSoftObstacleLST);
	string errmsg("getPath() failed to return null when goal is a soft obstacle");
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathReturnNullWhenStartAndGoalSameLocation()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableStartAndGoalIdenticalLST);
	string errmsg("getPath() failed to return null when start and goal nodes are identical");
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathReturnNullWhenStartAndGoalNodesIdentical()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableStartAndGoalIdenticalLST);
	string errmsg("getPath() failed to return null when start and goal nodes are identical");
	pos = te->getStartNode();
	path *p = aastar->getPath(amamock, pos, pos, te->caps, te->size);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), int(p), NULL);
}

void AnnotatedAStarTest::getPathReturnNullOnInvalidCapabilityParam()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableInvalidCapability);
	string errmsg("getPath() failed to return null when using invalid capability parameter");
	runGetPathTest(te, errmsg);
}

void AnnotatedAStarTest::getPathReturnNulWhenAgentSizeParamLessThanMin()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableAgentSizeLessThanMin);
	string errmsg("getPath() failed to return null when agent size parameter < minAgentSize");
	runGetPathTest(te, errmsg);

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
	pos = exp->getStartNode();
	n = exp->getGoalNode();
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move", exp->pathable, aastar->evaluate(pos, n)); 
}

void AnnotatedAStarTest::runGetPathTest(TestExperiment* exp, string &errmsg)
{
	
	amamock->setCurrentTestExperiment(exp);
	pos = exp->getStartNode();
	n = exp->getGoalNode();
	
	path *p = aastar->getPath(amamock, pos, n, exp->caps, exp->size);
	//if(!exp->pathable)
		CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), int(p), NULL);
	// else, check if the returned path is valid

}