/*
 *  HPAStar2Test.cpp
 *  hog
 *
 *  Created by dharabor on 18/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAStar2Test.h"
#include "path.h"
#include "ClusterNode.h"
#include "HPAClusterAbstraction.h"
#include "map.h"
#include "TestConstants.h"
#include "ClusterAStarFactory.h"
#include "ClusterAStar.h"
#include "HPAClusterFactory.h"
#include "EdgeFactory.h"
#include "ClusterNodeFactory.h"
#include "mapFlatAbstraction.h"
#include "aStar3.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HPAStar2Test );

void HPAStar2Test::setUp()
{
	p = NULL;
}

void HPAStar2Test::tearDown()
{
	if(p!=NULL)
		delete p;
}

ClusterNode* HPAStar2Test::getNode(int x, int y, HPAClusterAbstraction& hpamap)
{
	return dynamic_cast<ClusterNode*>(hpamap.getNodeFromMap(x,y));

}

void HPAStar2Test::getPathReturnNullWhenStartOrGoalNull()
{	
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);

	p = hpastar.getPath(&hpamap, NULL, n); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start node is null", int(p), NULL);

	p = hpastar.getPath(&hpamap, n, NULL); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when goal node is null", int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenStartAndGoalSameLocation()
{
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	ClusterNode* n2 = getNode(0,1,hpamap);
	n2->setLabelL(kFirstData+1, 0); // both nodes have (0,0) as their coordinates.

	p = hpastar.getPath(&hpamap, n, n2); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start & goal have same coordinates.", int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenStartAndGoalNodesIdentical()
{
	string errmsg("getPath() failed to return null when start and goal nodes are identical");
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	p = hpastar.getPath(&hpamap, n, n); 

	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenMapAbstractionParameterNull()
{
	mapFlatAbstraction mfa(new Map(maplocation.c_str()));
	HPAStar2 hpastar;
	node* pos = mfa.getNodeFromMap(1,2);
	node* n = mfa.getNodeFromMap(22,1);
	
	p = hpastar.getPath(NULL, pos, n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with NULL map abstraction parameter", NULL, (int)p); 
}

void HPAStar2Test::getPathReturnNullWhenStartOrGoalHave_kAbstractionLevel_GreaterThanZero()
{
	HPAClusterAbstraction hpamap(new Map(hpaentrancetest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
		
	ClusterNode* n1 = getNode(0,0,hpamap);
	ClusterNode* n2 = getNode(2,2,hpamap);
	
	n1->setLabelL(kAbstractionLevel, 1);
	n2->setLabelL(kAbstractionLevel, 1);
	p = hpastar.getPath(&hpamap, n2, n1); 
}

void HPAStar2Test::getPathShouldReturnTheShortestPathBetweenTwoLowLevelNodes()
{
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	/* record size of graph and pathcache; make sure we don't screw anything up */
	graph  *absg = hpamap.getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = hpamap.getPathCacheSize();
	
	node *start = hpamap.getNodeFromMap(2,1);
	node* goal = hpamap.getNodeFromMap(4,5);
	HPAStar2 hpastar;
	p = hpastar.getPath(&hpamap, start,goal);	
	
	int expectedLength = 8;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path length wrong", expectedLength, (int)p->length());	

	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);
	CPPUNIT_ASSERT_MESSAGE("start of path is wrong", start == p->n);
	path* cur = p->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+1 in path is wrong", hpamap.getNodeFromMap(1,2)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+2 in path is wrong", hpamap.getNodeFromMap(0,3)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+3 in path is wrong", hpamap.getNodeFromMap(1,4)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+4 in path is wrong", hpamap.getNodeFromMap(1,5)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+5 in path is wrong", hpamap.getNodeFromMap(2,5)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+6 in path is wrong", hpamap.getNodeFromMap(3,5) == cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("end of path is wrong", goal == p->tail()->n);
	
	double expectedDist =  8.24	;
	double dist = hpamap.distance(p);
	dist = ((int)(dist*100+0.5))/100.0;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance wrong", expectedDist, dist);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in abstract graph is wrong", numNodesExpected, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count in abstract graph is wrong", numEdgesExpected, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numCachedPathsExpected, hpamap.getPathCacheSize());
}

void HPAStar2Test::getPathShouldRemoveAllInsertedNodesAndEdgesFromTheAbstractPathAndPathCacheIfTheSearchFailsToFindASolution()
{
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	/* record size of graph and pathcache; make sure we don't screw anything up */
	graph  *absg = hpamap.getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = hpamap.getPathCacheSize();

	node *start = hpamap.getNodeFromMap(2,1);
	node* goal = hpamap.getNodeFromMap(8,0);
	
	HPAStar2 hpastar;
	p = hpastar.getPath(&hpamap, start,goal);	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to return a null path for an invalid problem", true, p == NULL);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in abstract graph is wrong", numNodesExpected, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count in abstract graph is wrong", numEdgesExpected, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numCachedPathsExpected, hpamap.getPathCacheSize());
}

void HPAStar2Test::getPathShouldFindASolutionWithoutInsertingIntoTheAbstractGraphIfBothStartAndGoalAreInTheSameCluster()
{
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	node *start = hpamap.getNodeFromMap(5,2);
	node* goal = hpamap.getNodeFromMap(7,2);

	HPAStar2 hpastar;
	p = hpastar.getPath(&hpamap, start,goal);	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("no solution found when one exists", true, p!=0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insNodesExpanded metric unexpectedly non-zero", (long)0, hpastar.getInsertNodesExpanded());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insNodesTouched metric unexpectedly non-zero", (long)0, hpastar.getInsertNodesTouched());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insSearchTime metric unexpectedly non-zero", (double)0, hpastar.getInsertSearchTime());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insPeakMemory metric unexpectedly non-zero", (long)0, hpastar.getInsertPeakMemory());
}

void HPAStar2Test::getPathShouldAddInsertionEffortToPerformanceMetrics()
{
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	node *start = hpamap.getNodeFromMap(2,1);
	
	/* positions of transition points in the start cluster; need to run an A* search to each of these for each insertion */
	node* tp1 = hpamap.getNodeFromMap(4,2);
	node* tp2 = hpamap.getNodeFromMap(1,4);
	node* tp3 = hpamap.getNodeFromMap(4,4);
	
	node* goal = hpamap.getNodeFromMap(5,2); // just outside start cluster
	HPACluster* startCluster = hpamap.getCluster(dynamic_cast<ClusterNode*>(start)->getParentClusterId());
	ClusterAStar castar;
	castar.setCorridorNodes(startCluster->getNodes());
	p = castar.getPath(&hpamap, start, tp1);
	if(p) delete p;
		
	long ne = castar.getNodesExpanded();
	long nt = castar.getNodesTouched();
	long pm = castar.getPeakMemory();
	double st = castar.getSearchTime();

	p = castar.getPath(&hpamap, start, tp2);
	if(p) delete p;
	
	ne += castar.getNodesExpanded();
	nt += castar.getNodesTouched();
	pm = pm<castar.getPeakMemory()?castar.getPeakMemory():pm;
	st += castar.getSearchTime();

	p = castar.getPath(&hpamap, start, tp3);
	if(p) delete p;
	
	ne += castar.getNodesExpanded();
	nt += castar.getNodesTouched();
	pm = pm<castar.getPeakMemory()?castar.getPeakMemory():pm;
	st += castar.getSearchTime();

	HPAStar2 hpastar;
	p = hpastar.getPath(&hpamap, start, goal);	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insertion nodes expanded is wrong", ne, hpastar.getInsertNodesExpanded());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insertion nodes touched is wrong", nt, hpastar.getInsertNodesTouched());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("insertion peak memory is wrong", pm, hpastar.getInsertPeakMemory());
//	CPPUNIT_ASSERT_EQUAL_MESSAGE("insertion search time is wrong", st, hpastar.getInsertSearchTime()); // don't bother. differences between infrequent runs make this unreliable
}

void HPAStar2Test::logStatsShouldRecordAllMetricsToStatsCollection()
{
	statCollection sc;
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	node *start = hpamap.getNodeFromMap(2,1);
	node* goal = hpamap.getNodeFromMap(4,5);
	
	HPAStar2 hpastar;
	p = hpastar.getPath(&hpamap, start, goal);
	assert(p != 0);
	
	hpastar.logFinalStats(&sc);
	
	statValue result;
	int lookupResult = sc.lookupStat("nodesExpanded", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)hpastar.getNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat("nodesTouched", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)hpastar.getNodesTouched(), result.lval);

	lookupResult = sc.lookupStat("peakMemory", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)hpastar.getPeakMemory(), result.lval);

	lookupResult = sc.lookupStat("searchTime", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)hpastar.getSearchTime(), result.fval);

	lookupResult = sc.lookupStat("insNodesExpanded", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesExpanded metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesExpanded metric in statsCollection doesn't match expected result", (long)hpastar.getInsertNodesExpanded(), result.lval);

	lookupResult = sc.lookupStat("insNodesTouched", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find nodesTouched metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("nodesTouched metric in statsCollection doesn't match expected result", (long)hpastar.getInsertNodesTouched(), result.lval);

	lookupResult = sc.lookupStat("insPeakMemory", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find peakMemory metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("peakMemory metric in statsCollection doesn't match expected result", (long)hpastar.getInsertPeakMemory(), result.lval);

	lookupResult = sc.lookupStat("insSearchTime", hpastar.getName() , result);
	CPPUNIT_ASSERT_MESSAGE("couldn't find searchTime metric in statsCollection", lookupResult != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("searchTime metric in statsCollection doesn't match expected result", (double)hpastar.getInsertSearchTime(), result.fval);
}

void HPAStar2Test::getPathShouldReturnANonRefinedPathIfRefinementFlagIsNotSet()
{
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	/* record size of graph and pathcache; make sure we don't screw anything up */
	graph  *absg = hpamap.getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = hpamap.getPathCacheSize();
	
	node *start = hpamap.getNodeFromMap(2,1);
	node* goal = hpamap.getNodeFromMap(4,5);
	HPAStar2 hpastar;
	hpastar.setRefineAbstractPathFlag(false);
	p = hpastar.getPath(&hpamap, start,goal);

	int expectedLength = 4;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path length wrong", expectedLength, (int)p->length());	

	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);
	CPPUNIT_ASSERT_MESSAGE("start of path is wrong", start == p->n);
	path* cur = p->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+1 in path is wrong", hpamap.getNodeFromMap(1,4) == cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+4 in path is wrong", hpamap.getNodeFromMap(1,5) == cur->n);
	CPPUNIT_ASSERT_MESSAGE("end of path is wrong", goal == p->tail()->n);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in abstract graph is wrong", numNodesExpected, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count in abstract graph is wrong", numEdgesExpected, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numCachedPathsExpected, hpamap.getPathCacheSize());
}

void HPAStar2Test::getPathShouldReturnAShortestPathBetweenTwoLowLevelNodesIfFastRefinementFlagIsSet()
{
	return;
	HPAClusterAbstraction hpamap(new Map(hpastartest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	hpamap.buildClusters();
	hpamap.buildEntrances();

	/* record size of graph and pathcache; make sure we don't screw anything up */
	graph  *absg = hpamap.getAbstractGraph(1);
	int numNodesExpected = absg->getNumNodes();
	int numEdgesExpected = absg->getNumEdges();
	int numCachedPathsExpected = hpamap.getPathCacheSize();
	
	node *start = hpamap.getNodeFromMap(2,1);
	node* goal = hpamap.getNodeFromMap(4,5);
	HPAStar2 hpastar;
	hpastar.setFastRefinement(true);
	p = hpastar.getPath(&hpamap, start,goal);	
	
	int expectedLength = 8;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path length wrong", expectedLength, (int)p->length());	

	CPPUNIT_ASSERT_MESSAGE("failed to find a valid path when one exists", p != 0);
	CPPUNIT_ASSERT_MESSAGE("start of path is wrong", start == p->n);
	path* cur = p->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+1 in path is wrong", hpamap.getNodeFromMap(1,2)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+2 in path is wrong", hpamap.getNodeFromMap(0,3)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+3 in path is wrong", hpamap.getNodeFromMap(1,4)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+4 in path is wrong", hpamap.getNodeFromMap(1,5)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+5 in path is wrong", hpamap.getNodeFromMap(2,5)== cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("node @ start+6 in path is wrong", hpamap.getNodeFromMap(3,5) == cur->n);
	cur = cur->next;
	CPPUNIT_ASSERT_MESSAGE("end of path is wrong", goal == p->tail()->n);
	
	double expectedDist =  8.24	;
	double dist = hpamap.distance(p);
	dist = ((int)(dist*100+0.5))/100.0;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance wrong", expectedDist, dist);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in abstract graph is wrong", numNodesExpected, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count in abstract graph is wrong", numEdgesExpected, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numCachedPathsExpected, hpamap.getPathCacheSize());	
}