/*
 *  HPAClusterAbstractionTest.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterAbstractionTest.h"
#include "HPAClusterAbstraction.h"
#include "TestConstants.h"

#include "HPAClusterFactory.h"
#include "ClusterNodeFactory.h"
#include "ClusterAStarFactory.h"
#include "EdgeFactory.h"
#include "NodeFactory.h"
#include "EdgeFactory.h"

#include "HPAClusterMock.h"
#include "ClusterNodeMock.h"
#include "ClusterAStarMock.h"
#include "NodeMock.h"
#include <mockpp/chaining/ChainingMockObjectSupport.h>

CPPUNIT_TEST_SUITE_REGISTRATION( HPAClusterAbstractionTest );

void HPAClusterAbstractionTest::setUp()
{
	this->nf = new ClusterNodeFactory();
	this->ef = new EdgeFactory();
	this->cf = new HPAClusterFactory();
	this->caf = new ClusterAStarFactory();
}

void HPAClusterAbstractionTest::tearDown()
{
}

void HPAClusterAbstractionTest::buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters()
{		
	HPAClusterAbstraction hpacaMap(new Map(emptymap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	int totalExpectedClusters = 4;

	/* check for the correct # of clusters */
	hpacaMap.buildClusters();
	CPPUNIT_ASSERT_EQUAL(totalExpectedClusters, hpacaMap.getNumClusters());
}

void HPAClusterAbstractionTest::buildClustersShouldCalculateCorrectClusterSize()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);

	int clusterWidths[4] = {5,5,4,4};
	int clusterHeights[4] = {5,1,5,1};

	hpacaMap.buildClusters();
	cluster_iterator it = hpacaMap.getClusterIter();
	for(int i=0; i<4; i++)
	{
		HPACluster* cluster = hpacaMap.clusterIterNext(it);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster height", clusterHeights[i], cluster->getHeight());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster width", clusterWidths[i], cluster->getWidth());
	}
}

void HPAClusterAbstractionTest::getClusterSizeShouldReturnSameValueAsConstructorParameter()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	CPPUNIT_ASSERT_EQUAL(TESTCLUSTERSIZE, hpacaMap.getClusterSize());
}

void HPAClusterAbstractionTest::constructorShouldCreateANewGraphObject()
{	
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	int numGraphsExpected = 2;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("actual graph count does not match expected count", numGraphsExpected, hpacaMap.getNumberOfAbstractionLevels());
}

void HPAClusterAbstractionTest::constructorThrowsExceptionGivenAnIncompatibleNodeFactoryParameter()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, new NodeFactory(), ef, TESTCLUSTERSIZE);
}


void HPAClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsLessThanZero()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	int expectedValue = 0;
	int clusterid=-1;
	HPACluster* result = hpacaMap.getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid < 0", expectedValue, (int)result);
}

void HPAClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	int expectedValue = 0;
	int clusterid=hpacaMap.getNumClusters()+1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid >= numclusters", expectedValue, (int)hpacaMap.getCluster(clusterid));
}

void HPAClusterAbstractionTest::getClusterShouldReturnRequestedClusterGivenAValidClusterId()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	HPACluster *cluster = new HPACluster(0,0,5,5);
	int clusterId = 0;
	cluster->setClusterId(clusterId);
	hpacaMap.clusters[clusterId] = cluster;

	HPACluster* c = hpacaMap.getCluster(0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned null instead of a cluster", true, c!=NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned wrong cluster", clusterId, c->getClusterId());
}

void HPAClusterAbstractionTest::addPathToCacheShouldStoreAPathGivenAnEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	graph* g = new graph();
	node* n1 = nf->newNode("");
	node* n2 = nf->newNode("");
	g->addNode(n1);
	g->addNode(n2);
	path* p = new path(n1, new path(n2, NULL));
		
	edge* e = new edge(p->n->getNum(), p->next->n->getNum(), 1.0);
	g->addEdge(e);
	
	hpacaMap.addPathToCache(e, p);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cache path count incorrect", 1, (int)hpacaMap.pathCache.size());
	CPPUNIT_ASSERT_MESSAGE("wrong path retrieved", p == hpacaMap.pathCache[e->getUniqueID()]);
	delete g;
}

void HPAClusterAbstractionTest::addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	path* p = new path(NULL, NULL);
	edge* e = new edge(0, 1, 1);
	
	hpacaMap.addPathToCache(NULL, p);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)hpacaMap.pathCache.size());
	
	hpacaMap.addPathToCache(e, NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)hpacaMap.pathCache.size());
	
	delete p;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnAPathGivenAValidEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	node* n = nf->newNode("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	hpacaMap.addPathToCache(e,p);
	path* ret = hpacaMap.getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnInvalidEdge()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	
	node* n = nf->newNode("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	hpacaMap.addPathToCache(e,p);
	path* ret = hpacaMap.getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void HPAClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);

	node* n = nf->newNode("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	edge* e2 = new edge(1,1,1);
	
	hpacaMap.addPathToCache(e,p);
	
	path* ret = hpacaMap.getPathFromCache(e2);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an NULL value given an invalid edge", NULL == ret);

	delete n;
	delete e;
	delete e2;
}

/* deletion of start & goal involves: 
	- removal of nodes and all edges from graph. 
	- removal from parent clusters. 
	- resetting startid and goalid to -1 */
void HPAClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph()
{
	/* setup test data */
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	
	/* add two clusters to store the start and goal */
	HPAClusterMock* startCluster = new HPAClusterMock(0,0,5,5);
	HPAClusterMock* goalCluster = new HPAClusterMock(5,0,5,5);
	int startClusterId = 0;
	int goalClusterId = 1;
	startCluster->setClusterId(startClusterId);
	goalCluster->setClusterId(goalClusterId);
	hpacaMap.clusters[startClusterId] = startCluster;
	hpacaMap.clusters[goalClusterId] = goalCluster;
				
	/* create start & goal nodes. create also 2 additional neighbours to represent transition points in the start and goal clusters which are connected to s & g */	
	ClusterNode* start = dynamic_cast<ClusterNode*>(nf->newNode(hpacaMap.getNodeFromMap(2,1))); 
	start->setParentClusterId(startClusterId);
	ClusterNode* snbor = dynamic_cast<ClusterNode*>(nf->newNode(hpacaMap.getNodeFromMap(4,1)));
	snbor->setParentClusterId(startClusterId);
	ClusterNode* goal = dynamic_cast<ClusterNode*>(nf->newNode(hpacaMap.getNodeFromMap(7,1)));
	goal->setParentClusterId(goalClusterId);
	ClusterNode* gnbor = dynamic_cast<ClusterNode*>(nf->newNode(hpacaMap.getNodeFromMap(5,1)));
	gnbor->setParentClusterId(goalClusterId);
	graph* absg = hpacaMap.getAbstractGraph(1);
	absg->addNode(start);
	absg->addNode(goal);
	absg->addNode(snbor);
	absg->addNode(gnbor);
	edge* sedge = new edge(start->getNum(), snbor->getNum(), 1.0);
	absg->addEdge(sedge);
	edge *gedge = new edge(goal->getNum(), gnbor->getNum(), 1.0);
	absg->addEdge(gedge);
	edge* interedge = new edge(snbor->getNum(), gnbor->getNum(), 1.0);
	absg->addEdge(interedge);

	/* mock insertion of s&g */
	int startid = start->getNum();
	int startUniqueId = start->getUniqueID();
	int goalid = goal->getNum();
	int goalUniqueId = goal->getUniqueID();
	hpacaMap.startid = startid;
	hpacaMap.goalid = goalid;
	
	/* add paths to represent each edge into path cache */
	path* gpath = new path(goal, NULL);
	path* spath = new path(start, NULL);
	path* otherpath = new path(snbor, NULL);
	hpacaMap.addPathToCache(sedge, spath);
	hpacaMap.addPathToCache(gedge, gpath);
	hpacaMap.addPathToCache(interedge, otherpath);
	
	
	/* expect HPACluster::removeParent to be called for on both the start and goal nodes */
	startCluster->removeParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::eq<int>(startid));
	goalCluster->removeParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::eq<int>(goalid));
	
	/* run the test */
	int numExpectedAbstractNodes = 2;
	int numExpectedAbstractEdges = 1;
	int numExpectedPathsInCache = 1;
	
	hpacaMap.removeStartAndGoalNodesFromAbstractGraph(); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", numExpectedAbstractEdges, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("start node not removed from graph", false, absg->getNode(startid)->getUniqueID() == startUniqueId);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("goal node not removed from graph", false, absg->getNode(goalid)->getUniqueID() == goalUniqueId);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("start & goal edges not deleted", true, (*absg->getEdgeIter())->getFrom() == snbor->getNum() && (*absg->getEdgeIter())->getTo() == gnbor->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("startid not reset after removal from graph", -1, hpacaMap.startid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("goalid not reset after removal from graph", -1, hpacaMap.startid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("pathcache size is wrong", numExpectedPathsInCache, hpacaMap.getPathCacheSize());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to delete start or goal path", (int)otherpath, (int)hpacaMap.getPathFromCache(interedge)); // interedge path all that remains
	
	goalCluster->verify();
	startCluster->verify();
}

void HPAClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullStartParameter() throw(std::invalid_argument)
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	node* start = NULL;
	node* goal = hpacaMap.getNodeFromMap(5,3);	
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);	
}

void HPAClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenANullGoalParameter() throw(std::invalid_argument)
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	node* goal = NULL;
	node* start = hpacaMap.getNodeFromMap(5,3);	
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);	

}

void HPAClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractStartParameter() throw(std::invalid_argument)
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	node* start = hpacaMap.getNodeFromMap(2,1);
	node* goal = hpacaMap.getNodeFromMap(5,3);
	start->setLabelL(kAbstractionLevel, 1);
		
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);
}

void HPAClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenAnAbstractGoalParameter() throw(std::invalid_argument)
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);
	node* start = hpacaMap.getNodeFromMap(2,1);
	node* goal = hpacaMap.getNodeFromMap(5,3);
	goal->setLabelL(kAbstractionLevel, 1);
		
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);
}

/* integration test; TODO: add proper support for mock clusters here */
void HPAClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);

	/* create the parent clusters for the start & goal nodes */
	int startClusterId = 0;
	int goalClusterId = 1;
	HPAClusterMock* startCluster = new HPAClusterMock(0,0,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	HPAClusterMock* goalCluster = new HPAClusterMock(5,5,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	
	startCluster->setClusterId(startClusterId);
	goalCluster->setClusterId(goalClusterId);
	hpacaMap.clusters[startClusterId] = dynamic_cast<HPACluster*>(startCluster);
	hpacaMap.clusters[goalClusterId] = dynamic_cast<HPACluster*>(goalCluster);
	
	/* create the starr & goal nodes & their abstract counterparts that will be inserted into the abstract graph */
	ClusterNodeMock* start = new ClusterNodeMock("start");
	ClusterNodeMock* startClone = new ClusterNodeMock("startClone");
	ClusterNodeMock* goal = new ClusterNodeMock("goal");
	ClusterNodeMock* goalClone = new ClusterNodeMock("goalClone");
	start->setLabelL(kAbstractionLevel, 0);
	start->setParentClusterId( startClusterId);	
	goal->setLabelL(kAbstractionLevel, 0);
	goal->setParentClusterId( goalClusterId);
	
	/* setup mock object expectations */
	start->cloneMocker.expects(MOCKPP_NS::once()).will(MOCKPP_NS::returnValue<graph_object*>(startClone));	
	goal->cloneMocker.expects(MOCKPP_NS::once()).will(MOCKPP_NS::returnValue<graph_object*>(goalClone));	

	startCluster->setExpectedAddParentParameters(startClone, &hpacaMap);
	startCluster->setExpectedOccurencesForAddParent(1);
	goalCluster->setExpectedAddParentParameters(goalClone, &hpacaMap);
	goalCluster->setExpectedOccurencesForAddParent(1);
//	startCluster->addParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::same<node*>(startClone), MOCKPP_NS::same<HPAClusterAbstraction*>(&hpacaMap));
//	goalCluster->addParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::same<node*>(goalClone), MOCKPP_NS::same<HPAClusterAbstraction*>(&hpacaMap));

	graph* absg = hpacaMap.getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted start node", true, hpacaMap.startid != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted goal node", true, hpacaMap.goalid != -1);

	node* absstart = absg->getNode(hpacaMap.startid);
	node* absgoal = absg->getNode(hpacaMap.goalid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add start node to abstract graph", true, startClone == absg->getNode(hpacaMap.startid));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add goal node to abstract graph", true, goalClone == absg->getNode(hpacaMap.goalid));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of nodes added to abstract graph", numAbstractNodes+2, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to set kParent label of original start node to id of new abstract node", (long)absstart->getNum(), start->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to set kParent label of original goal node to id of new abstract node", (long)absgoal->getNum(), goal->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to increment kAbstractionLevel label of new abstract start node", 1, (int)absstart->getLabelL(kAbstractionLevel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to increment kAbstractionLevel label of new abstract goal node", 1, (int)absgoal->getLabelL(kAbstractionLevel));
	
	startCluster->verify();
	goalCluster->verify();
	start->verify();
	goal->verify();
	startClone->verify();
	goalClone->verify();
	
	delete start;
	delete goal;
}

void HPAClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph()
{
	HPAClusterAbstraction hpacaMap(new Map(acmap.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);

	// create the parent clusters for the start & goal nodes 
	int startClusterId = 0;
	int goalClusterId = 1;
	HPAClusterMock* startCluster = new HPAClusterMock(0,0,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	HPAClusterMock* goalCluster = new HPAClusterMock(5,5,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	
	startCluster->setClusterId(startClusterId);
	goalCluster->setClusterId(goalClusterId);
	hpacaMap.clusters[startClusterId] = dynamic_cast<HPACluster*>(startCluster);
	hpacaMap.clusters[goalClusterId] = dynamic_cast<HPACluster*>(goalCluster);
	
	// create the starr & goal nodes & their abstract counterparts that will be inserted into the abstract graph 
	ClusterNodeMock* start = new ClusterNodeMock("start");
	ClusterNodeMock* goal = new ClusterNodeMock("goal");
	start->setLabelL(kAbstractionLevel, 0);
	start->setParentClusterId( startClusterId);	
	start->setLabelL(kParent, 42);
	goal->setLabelL(kAbstractionLevel, 0);
	goal->setParentClusterId( goalClusterId);
	goal->setLabelL(kParent, 42);
	
	// setup mock object expectations 
	start->cloneMocker.expects(MOCKPP_NS::never());
	goal->cloneMocker.expects(MOCKPP_NS::never());

//	startCluster->addParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::same<node*>(startClone), MOCKPP_NS::same<HPAClusterAbstraction*>(&hpacaMap));
//	goalCluster->addParentMocker.expects(MOCKPP_NS::once()).with(MOCKPP_NS::same<node*>(goalClone), MOCKPP_NS::same<HPAClusterAbstraction*>(&hpacaMap));
	
	graph* absg = hpacaMap.getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
	hpacaMap.insertStartAndGoalNodesIntoAbstractGraph(start, goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("startid incorrectly set", -1, hpacaMap.startid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("goalid incorrectly set", -1, hpacaMap.goalid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("unexpected change to number of nodes in abstract graph", numAbstractNodes, absg->getNumNodes());
	
	start->verify();
	goal->verify();
	
	delete start;
	delete goal;
}


void HPAClusterAbstractionTest::buildEntrancesCallsBuildEntranceMethodOnEachCluster()
{
	HPAClusterAbstraction hpacaMap(new Map(hpaentrancetest.c_str()), caf, cf, nf, ef, TESTCLUSTERSIZE);

	int c1Id = 0;
	int c2Id = 1;
	HPAClusterMock* c1 = new HPAClusterMock(0,0,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	HPAClusterMock* c2 = new HPAClusterMock(5,5,TESTCLUSTERSIZE,TESTCLUSTERSIZE);
	
	c1->setClusterId(c1Id);
	c2->setClusterId(c2Id);
	hpacaMap.clusters[c1Id] = dynamic_cast<HPACluster*>(c1);
	hpacaMap.clusters[c2Id] = dynamic_cast<HPACluster*>(c2);

	c1->setExpectedOccurencesForBuildEntrances(1);
	c1->setExpectedBuildEntrancesParameters(&hpacaMap);
	c2->setExpectedOccurencesForBuildEntrances(1);
	c2->setExpectedBuildEntrancesParameters(&hpacaMap);

	hpacaMap.buildEntrances();

	c1->verify();
	c2->verify();
}
