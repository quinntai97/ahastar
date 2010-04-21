/*
 *  ClusterAStarTest.cpp
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClusterAStarTest.h"
#include "ClusterAStar.h"
#include "HPAClusterAbstraction.h"
#include "ClusterAStarFactory.h"
#include "HPAClusterFactory.h"
#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "ClusterNode.h"

#include "TestConstants.h"
#include "heap.h"
#include "path.h"
#include "mapFlatAbstraction.h"
#include "TestConstants.h"
#include "statCollection.h"

#include <map>

CPPUNIT_TEST_SUITE_REGISTRATION( ClusterAStarTest );

void ClusterAStarTest::setUp()
{
	p = NULL;
}

void ClusterAStarTest::tearDown()
{
	if(p!=NULL)
		delete p;
}

ClusterNode* ClusterAStarTest::getNode(int x, int y, HPAClusterAbstraction& hpamap)
{
	return dynamic_cast<ClusterNode*>(hpamap.getNodeFromMap(x,y));

}

void ClusterAStarTest::getPathReturnNullWhenStartOrGoalNull()
{	
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);

	p = castar.getPath(&hpamap, NULL, n); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start node is null", true, p == 0);

	p = castar.getPath(&hpamap, n, NULL); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when goal node is null", true, p == 0);
}

void ClusterAStarTest::getPathReturnNullWhenStartAndGoalSameLocation()
{
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	ClusterNode* n2 = getNode(0,1,hpamap);
	n2->setLabelL(kFirstData+1, 0); // both nodes have (0,0) as their coordinates.

	p = castar.getPath(&hpamap, n, n2); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start & goal have same coordinates.", true, p == 0);
}

void ClusterAStarTest::getPathReturnNullWhenStartAndGoalNodesIdentical()
{
	string errmsg("getPath() failed to return null when start and goal nodes are identical");
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	p = castar.getPath(&hpamap, n, n); 

	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), true, p == 0);
}

void ClusterAStarTest::getPathReturnNullWhenMapAbstractionParameterNull()
{
	mapFlatAbstraction mfa(new Map(maplocation.c_str()));
	ClusterAStar castar;
	node* pos = mfa.getNodeFromMap(1,2);
	node* n = mfa.getNodeFromMap(22,1);
	
	p = castar.getPath(NULL, pos, n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with NULL map abstraction parameter", true, p == 0); 
}


/* initialise start to h(start, goal) because AA* does not keep separate g and h distances for each node, only fcost. so, when 
relaxing edges the gcost is re-derived by substracting h cost from fcost. Because of this messy implementation in the parent class
(aStarOld) we need to annotate the fcost of the start with h(start, goal) else a negative value arises when computing fcost of a start neighbour
see aStarOld::relaxEdge() for a better idea of wtf is going on here */
void ClusterAStarTest::getPathInitialisesFCostValueOfStartToHeuristicGoalDistance()
{
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
	ClusterNode* start = getNode(4,8, hpamap);
	ClusterNode* goal = getNode(1,1, hpamap);

	p = castar.getPath(&hpamap, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("fCost of start node != h(start, goal)", start->getLabelF(kTemporaryLabel), hpamap.h(start, goal));		
	delete p;
	p=NULL;
}

// check that the path contains only connected nodes 
void ClusterAStarTest::getPathEachNodeInReturnedPathHasAnEdgeToItsPredecessor()
{
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
	ClusterNode* start = getNode(4,8, hpamap);
//	ClusterNode* goal = getNode(1,1, hpamap);
	ClusterNode* goal = getNode(2,2, hpamap);
	
	p = castar.getPath(&hpamap, start, goal);

	path* p2 = p;
	do {
		node *curnode, *nextnode;
		curnode = p2->n;
		nextnode = p2->next->n;
		edge* medge = nextnode->getMarkedEdge();		
		CPPUNIT_ASSERT_EQUAL(true, medge->getFrom() == curnode->getNum() || medge->getTo() == curnode->getNum());
		CPPUNIT_ASSERT_EQUAL(true, medge->getFrom() == nextnode->getNum() || medge->getTo() == nextnode->getNum());
		
		p2 = p2->next;
	} while(p2->next);	
}

void ClusterAStarTest::getPathFailsToReturnASoltuionWhenNoneExistsWithinTheCorridorBounds()
{
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	hpamap.buildClusters();
	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
	ClusterNode* start = getNode(4,4, hpamap);	
	ClusterNode* goal = getNode(5,1, hpamap);
	HPACluster* startCluster = hpamap.getCluster(start->getParentClusterId());
	
	HPAUtil::nodeTable* clusterNodes = startCluster->getNodes();
	castar.setCorridorNodes(clusterNodes);

	p = castar.getPath(&hpamap, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("found a path that shouldn't exist inside corridor", true, p == NULL);	
}

void ClusterAStarTest::getPathReturnsTheShortestPath()
{
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	hpamap.buildClusters();
	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);
	ClusterNode* start = getNode(4,4, hpamap);
	ClusterNode* goal = getNode(1,4, hpamap);
	
	p = castar.getPath(&hpamap, start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find a path inside corridor", true, p != NULL);	
	
	double pdist = (int)(hpamap.distance(p)*10+0.5)/10; // stupid c++ rounding
	double realdist = (int)(3*10+0.5)/10;
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("corridor path has wrong length", realdist, pdist );
}


void ClusterAStarTest::logStatsShouldRecordAllMetricsToStatsCollection()
{	
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	hpamap.buildClusters();
	ClusterAStar castar;

	statCollection sc;
	p = castar.getPath(&hpamap, hpamap.getNodeFromMap(2,1), hpamap.getNodeFromMap(4,5));
	castar.logFinalStats(&sc);
	
	assert(p != 0);
	string catNE = "nodesExpanded";
	string catNT = "nodesTouched";
	string catPM = "peakMemory";
	string catST = "searchTime";
	
	statValue result;
	bool lookupResult = sc.lookupStat(catNE.c_str(), castar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)castar.getNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat(catNT.c_str(), castar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)castar.getNodesTouched(), result.lval);

	lookupResult = sc.lookupStat(catPM.c_str(), castar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)castar.getPeakMemory(), result.lval);

	lookupResult = sc.lookupStat(catST.c_str(), castar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult == true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)castar.getSearchTime(), result.fval);

}


void ClusterAStarTest::expandDoesNotReopenNodesOnTheClosedList()
{
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory());
	hpamap.setClusterSize(TESTCLUSTERSIZE);

	ClusterAStar castar;
	castar.setGraphAbstraction(&hpamap);

	heap openList;
	std::map<int, node*> closedList;
	
	graph g;
	node* target = new node("target");
	target->setLabelL(kFirstData, 0);
	target->setLabelL(kFirstData, 1);
	node* neighbour1 = new node("neighbour1");
	neighbour1->setLabelL(kFirstData, 1);
	neighbour1->setLabelL(kFirstData, 0);
	node* neighbour2 = new node("neighbour2");
	neighbour2->setLabelL(kFirstData, 1);
	neighbour2->setLabelL(kFirstData, 1);
	node* goal = new node("goal");
	goal->setLabelL(kFirstData, 3);
	goal->setLabelL(kFirstData, 4);
	
	
	g.addNode(target);
	g.addNode(neighbour1);
	g.addNode(neighbour2);
	g.addNode(goal);
	
	edge* e = new edge(target->getNum(), neighbour1->getNum(), 1.0);
	g.addEdge(e);
	e = new edge(target->getNum(), neighbour2->getNum(), 1.0);
	g.addEdge(e);
	
	closedList[neighbour1->getUniqueID()] = neighbour1;
	castar.expand(target, goal, &openList, closedList, &g);
	
	int expectedSizeOfOpenList = 1;
	int expectedSizeOfClosedList = 2;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open list size is wrong", expectedSizeOfOpenList, (int)openList.size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("closed list unexpectedly modified!", expectedSizeOfClosedList, (int)closedList.size());

	openList.remove();
	castar.expand(neighbour2, NULL, &openList, closedList, &g);
	expectedSizeOfOpenList = 0;
	expectedSizeOfClosedList = 3;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open list size is wrong", expectedSizeOfOpenList, (int)openList.size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("closed list unexpectedly modified!", expectedSizeOfClosedList, (int)closedList.size());
}
