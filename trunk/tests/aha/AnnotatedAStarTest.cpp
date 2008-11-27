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
#include "mapFlatAbstraction.h"
#include "AnnotatedClusterAbstractionMock.h"
#include "TestConstants.h"
#include "statCollection.h"


//CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedAStarTest );

using namespace ExpMgrUtil;

void AnnotatedAStarTest::setUp()
{
	amamock = new AnnotatedMapAbstractionMock(new Map(maplocation.c_str()), new AnnotatedAStarMock());
	aastar = new AnnotatedAStar();
	aastar->setGraphAbstraction(amamock);

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
	int capability = kGround;
	int clearance = 2;
	aastar->setCapability(capability);
	aastar->setClearance(clearance);
	path* p = aastar->getPath(amamock, pos, n); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start node is null", int(p), NULL);

	p = aastar->getPath(amamock, n, pos); 
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
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path *p = aastar->getPath(amamock, pos, pos);
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

void AnnotatedAStarTest::getPathReturnNullWhenNonAnnotatedMapAbstractionParameter()
{
	mapAbstraction* mfa = new mapFlatAbstraction(amamock->getMap());
	pos = getNode(1,2, kGround);
	pos->setClearance(kGround, 2);
	n = getNode(22,1, kGround);
	n->setClearance(kGround, 2);
	
	aastar->setCapability(kGround);
	aastar->setClearance(2);
	path* p = aastar->getPath(mfa, pos, n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with non-annotated map parameter", NULL, (int)p); 
	
	delete pos;
	delete n;
}


void AnnotatedAStarTest::getPathReturnNullWhenMapAbstractionParameterNull()
{

	TestExperiment *te = expmgr->getExperiment(kPathableToyProblemLST);
	pos = te->getStartNode();
	n = te->getGoalNode();
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(NULL, pos, n);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with NULL map abstraction parameter", NULL, (int)p); 
}

void AnnotatedAStarTest::getPathReturnNullWhenHardObstacleBlocksGoal()
{
	//looks like a bug in terrain annotation code; node @ 0,2 has kWater clearance of 9. evaluate passes & everything gets fcked.
	TestExperiment *te = expmgr->getExperiment(kNotPathableHardObstacleBlocksGoal);
	AnnotatedMapAbstraction ama(new Map(maplocation.c_str()), new AnnotatedAStarMock());
	string errmsg("getPath() failed to return null when the only solution is blocked by a hard obstacle");
	node *start = ama.getNodeFromMap(te->startx,te->starty);
	node* goal = ama.getNodeFromMap(te->goalx, te->goaly);

	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(&ama, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), NULL, (int)p);
}

void AnnotatedAStarTest::getPathReturnNullWhenSoftObstacleBlocksGoal()
{
	//looks like a bug in terrain annotation code; node @ 0,2 has kWater clearance of 9. evaluate passes & everything gets fcked.
	TestExperiment *te = expmgr->getExperiment(kNotPathableSoftObstacleBlocksGoal);
	AnnotatedMapAbstraction ama(new Map(maplocation.c_str()),  new AnnotatedAStarMock());
	string errmsg("getPath() failed to return null when the only solution is blocked by a soft obstacle");
	node *start = ama.getNodeFromMap(te->startx,te->starty);
	node* goal = ama.getNodeFromMap(te->goalx, te->goaly);

	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(&ama, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), NULL, (int)p);
}

void AnnotatedAStarTest::getPathWhenSolutionExistsForGroundCapabilityLST()
{
	TestExperiment *te = expmgr->getExperiment(kPathableToyProblemLST);
	AnnotatedMapAbstraction ama(new Map(maplocation.c_str()), new AnnotatedAStar());
	node *start = ama.getNodeFromMap(te->startx,te->starty);
	node* goal = ama.getNodeFromMap(te->goalx, te->goaly);
	
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(&ama, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned a NULL solution for a pathable problem", true, p != NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned solution length does not match length in test experiment", te->distance, ama.distance(p));
	
	delete p;
}

/* initialise start to h(start, goal) because AA* does not keep separate g and h distances for each node, only fcost. so, when 
relaxing edges the gcost is re-derived by substracting h cost from fcost. Because of this messy implementation in the parent class
(aStarOld) we need to annotate the fcost of the start with h(start, goal) else a negative value arises when computing fcost of a start neighbour
see aStarOld::relaxEdge() for a better idea of wtf is going on here */
void AnnotatedAStarTest::getPathInitialisesFCostValueOfStartToHeuristicGoalDistance()
{
	TestExperiment *te = expmgr->getExperiment(kPathableToyProblemLST);
	AnnotatedMapAbstraction ama(new Map(maplocation.c_str()), new AnnotatedAStar());
	node *start = ama.getNodeFromMap(te->startx,te->starty);
	node* goal = ama.getNodeFromMap(te->goalx, te->goaly);

	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(&ama, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("fCost of start node != h(start, goal)", start->getLabelF(kTemporaryLabel), ama.h(start, goal));	
	
	delete p;
}

/* check that the path contains only connected nodes */
void AnnotatedAStarTest::getPathEachNodeInReturnedPathHasAnEdgeToItsPredecessor()
{
	TestExperiment *te = expmgr->getExperiment(kPathableToyProblemLST);
	AnnotatedMapAbstraction ama(new Map(maplocation.c_str()), new AnnotatedAStar());
	node *start = ama.getNodeFromMap(te->startx,te->starty);
	node* goal = ama.getNodeFromMap(te->goalx, te->goaly);
	
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(&ama, start, goal);

	path* p2 = p;
	do {
		node *curnode, *nextnode;
		curnode = p->n;
		nextnode = p->next->n;
		edge* medge = nextnode->getMarkedEdge();		
		CPPUNIT_ASSERT_EQUAL(true, medge->getFrom() == curnode->getNum() || medge->getTo() == curnode->getNum());
		CPPUNIT_ASSERT_EQUAL(true, medge->getFrom() == nextnode->getNum() || medge->getTo() == nextnode->getNum());
		
		p = p->next;
	} while(p->next);
	
	delete p2;
	
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
	
	aastar->setCapability(exp->caps);
	aastar->setClearance(exp->size);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate() failed to return true for legal move", exp->pathable, aastar->evaluate(pos, n)); 
}


void AnnotatedAStarTest::runGetPathTest(TestExperiment* exp, string &errmsg)
{
	
	amamock->setCurrentTestExperiment(exp);
	pos = exp->getStartNode();
	n = exp->getGoalNode();
	
	aastar->setCapability(exp->caps);
	aastar->setClearance(exp->size);
	path *p = aastar->getPath(amamock, pos, n);
	//if(!exp->pathable)
		CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), NULL, (int)p);
	// else, check if the returned path is valid

}

void AnnotatedAStarTest::getPathFailsToReturnASoltuionWhenNoneExistsWithinTheCorridorBounds()
{
	TestExperiment *te = expmgr->getExperiment(kNotPathableWhenCorridorIsRestrictedToParentCluster);
	
	delete amamock;
	amamock = new AnnotatedMapAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock());
	aastar->limitSearchToClusterCorridor(true);
	AnnotatedClusterAbstractionMock::loadClusterAnnotations(acmap, amamock);

	node *start = amamock->getNodeFromMap(te->startx,te->starty);
	node* goal = amamock->getNodeFromMap(te->goalx, te->goaly);
	
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(amamock, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("found a path that shouldn't exist inside corridor", true, p == NULL);	
	
	delete p;
}

void AnnotatedAStarTest::getPathReturnsTheShortestPathWithinCorridorBounds()
{
	TestExperiment *te = expmgr->getExperiment(kPathableWithinCorridor);
	
	delete amamock;
	amamock = new AnnotatedMapAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock());
	aastar->limitSearchToClusterCorridor(true);
	AnnotatedClusterAbstractionMock::loadClusterAnnotations(acmap, amamock);

	node *start = amamock->getNodeFromMap(te->startx,te->starty);
	node* goal = amamock->getNodeFromMap(te->goalx, te->goaly);
	
	aastar->setCapability(te->caps);
	aastar->setClearance(te->size);
	path* p = aastar->getPath(amamock, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find a path inside corridor", true, p != NULL);	
	
	double pdist = (int)(amamock->distance(p)*10+0.5)/10; // stupid c++ rounding
	double realdist = (int)(te->distance*10+0.5)/10;
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("corridor path has wrong length", realdist, pdist );
	
	delete p;	
}

void AnnotatedAStarTest::logStatsShouldRecordAllMetricsToStatsCollection()
{	
	amamock = new AnnotatedMapAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock());
	amamock->annotateMap();

	statCollection sc;
	aastar->setClearance(1);
	aastar->setCapability(kGround);
	path* p = aastar->getPath(amamock, amamock->getNodeFromMap(2,1), amamock->getNodeFromMap(4,5));
	aastar->logFinalStats(&sc);
	
	assert(p != 0);
	string catNE = "nodesExpanded";
	string catNT = "nodesTouched";
	string catPM = "peakMemory";
	string catST = "searchTime";
	
	statValue result;
	bool lookupResult = sc.lookupStat(catNE.c_str(), aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)aastar->getNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat(catNT.c_str(), aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)aastar->getNodesTouched(), result.lval);

	lookupResult = sc.lookupStat(catPM.c_str(), aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)aastar->getPeakMemory(), result.lval);

	lookupResult = sc.lookupStat(catST.c_str(), aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)aastar->getSearchTime(), result.fval);

	lookupResult = sc.lookupStat("agentSize", aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find agentSize metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("agentSize metric in statsCollection doesn't match expected result", aastar->getClearance(), (int)result.lval);

	lookupResult = sc.lookupStat("agentCapability", aastar->getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find agentCapability metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("agentSize metric in statsCollection doesn't match expected result", aastar->getCapability(), (int)result.lval);
}