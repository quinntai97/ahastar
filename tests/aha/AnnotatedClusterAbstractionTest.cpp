/*
 *  AnnotatedClusterAbstractionTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 21/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstractionTest.h"
#include "ExperimentManager.h"
#include "Map.h"
#include "AnnotatedClusterAbstraction.h"
#include "clusterAbstraction.h"
#include "AnnotatedAStarMock.h"

#include "AnnotatedClusterFactory.h"
#include "AnnotatedClusterMockFactory.h"
#include "AnnotatedClusterMock.h"
#include "AnnotatedCluster.h"
#include "TestConstants.h"
#include <mockpp/chaining/ChainingMockObjectSupport.h>


//CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterAbstractionTest );

using namespace std;
USING_NAMESPACE_MOCKPP

void AnnotatedClusterAbstractionTest::setUp()
{
	testmap = new Map(maplocation.c_str()); // TODO: need a separate, larger map to test this junk
	aca = new AnnotatedClusterAbstraction(testmap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);
	expmgr = new ExperimentManager();
	acmock_factory = new AnnotatedClusterMockFactory(this);
}

void AnnotatedClusterAbstractionTest::tearDown()
{
	delete aca;
	delete expmgr;
}

void AnnotatedClusterAbstractionTest::setupMockClusterExpectationsForBuildClusterTests(AnnotatedClusterMock* acm)
{
	acm->addNodesToClusterMocker.expects(once()).id(MOCKPP_PCHAR("addNodesMocker"));
}

void AnnotatedClusterAbstractionTest::setupMockClusterExpectationsForBuildEntranceTests(AnnotatedClusterMock* acm)
{
	setupMockClusterExpectationsForBuildClusterTests(acm);
	acm->buildEntrancesMocker.expects(once()).after(MOCKPP_PCHAR("addNodesMocker"));	
}

void AnnotatedClusterAbstractionTest::buildClustersShouldSplitTheMapAreaIntoCorrectNumberOfClusters()
{
	acmock_factory->setTest(1);
	/* figure out how many clusters the test map should have */
	int mapwidth = aca->getMap()->getMapWidth();
	int mapheight = aca->getMap()->getMapHeight();
	
	int numHorizontalClusters = mapwidth / aca->getClusterSize();
	if(mapwidth % aca->getClusterSize() > 0)
		numHorizontalClusters++;
		
	int numVerticalClusters = mapheight / aca->getClusterSize();
	if(mapheight % aca->getClusterSize() > 0)
		numVerticalClusters++;
		
	int totalExpectedClusters = numHorizontalClusters*numVerticalClusters;

	/* check for the correct # of clusters */
	aca->buildClusters(acmock_factory);
	CPPUNIT_ASSERT_EQUAL(totalExpectedClusters, aca->getNumClusters());
}

void AnnotatedClusterAbstractionTest::buildClustersShouldCalculateCorrectClusterSize()
{
	acmock_factory->setTest(1);

	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStarMock(), TESTCLUSTERSIZE);

	int clusterWidths[4] = {5,4,5,4};
	int clusterHeights[4] = {5,5,1,1};

	for(int i=0; i<aca->getNumClusters(); i++)
	{
		AnnotatedCluster* ac = aca->getCluster(i);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster height", clusterHeights[i], ac->getHeight());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect cluster width", clusterWidths[i], ac->getWidth());
	}
}

void AnnotatedClusterAbstractionTest::getClusterSizeShouldReturnSameValueAsConstructorParameter()
{
	CPPUNIT_ASSERT_EQUAL(TESTCLUSTERSIZE, aca->getClusterSize());
}

void AnnotatedClusterAbstractionTest::constructorShouldCreateANewGraphObject()
{
	int numGraphsExpected = 2;	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("actual graph count does not match expected count", numGraphsExpected, aca->getNumberOfAbstractionLevels());
}


void AnnotatedClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsLessThanZero()
{
	int expectedValue = 0;
	int clusterid=-1;
	AnnotatedCluster* result = aca->getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid < 0", true, result == 0);
}

void AnnotatedClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters()
{
	int expectedValue = 0;
	int clusterid=aca->getNumClusters()+1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid >= numclusters", true, aca->getCluster(clusterid) == 0);
}

void AnnotatedClusterAbstractionTest::getClusterShouldReturnRequestedClusterGivenAValidClusterId()
{
	acmock_factory->setTest(1);

	aca->buildClusters(acmock_factory);
	int clusterid=0;
	AnnotatedCluster* ac = aca->getCluster(clusterid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned wrong cluster", true, ac->getClusterId() == clusterid );
}

/* integration test */
void AnnotatedClusterAbstractionTest::buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraph()
{
	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStar(), TESTCLUSTERSIZE);
	
	AnnotatedClusterFactory* ac_factory = new AnnotatedClusterFactory();
	aca->buildClusters(ac_factory);
	delete ac_factory;
	
	int numExpectedClusters = 4;
	int numExpectedAbstractEdges = 17; // includes intercluster and intracluster transitions
	int numExpectedAbstractNodes = 10;
	
	aca->buildEntrances();

	graph* absg = aca->getAbstractGraph(1);
	
	//debugging
/*	edge_iterator ei = absg->getEdgeIter();
	edge* e = absg->edgeIterNext(ei);
	while(e)
	{
		node* f = absg->getNode(e->getFrom());
		node* t = absg->getNode(e->getTo());
		cout << "\n edge connects "<<f->getLabelL(kFirstData)<<","<<f->getLabelL(kFirstData+1)<< " and "<<t->getLabelL(kFirstData)<<","<<t->getLabelL(kFirstData+1);
		cout <<"(weight: "<<e->getWeight()<<" caps: "<<e->getCapability() << " clearance: "<<e->getClearance(e->getCapability())<<")";
		e = absg->edgeIterNext(ei);
	}
*/	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of clusters created", numExpectedClusters, aca->getNumClusters());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract nodes", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract edges", numExpectedAbstractEdges, absg->getNumEdges());
}

void AnnotatedClusterAbstractionTest::buildEntrancesShouldCreateCorrectNumberOfTransitionsBetweenClustersAndAddTransitionsToAbstractGraphGivenALowQualityAbstraction()
{
	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStar(), TESTCLUSTERSIZE, ACAUtil::kLowQualityAbstraction);
	
	AnnotatedClusterFactory* ac_factory = new AnnotatedClusterFactory();
	aca->buildClusters(ac_factory);
	delete ac_factory;
	
	int numExpectedClusters = 4;
	int numExpectedAbstractEdges = 10; 
	int numExpectedAbstractNodes = 8;
	
	aca->buildEntrances();
	graph* absg = aca->getAbstractGraph(1);
	
	//debugging
/*	std::cout << "\nlow quality abstraction";
	edge_iterator ei = absg->getEdgeIter();
	edge* e = absg->edgeIterNext(ei);
	while(e)
	{
		node* f = absg->getNode(e->getFrom());
		node* t = absg->getNode(e->getTo());
		std::cout << "\n edge # "<<e->getEdgeNum()<<" uid "<<e->getUniqueID()<<" connects "<<f->getLabelL(kFirstData)<<","<<f->getLabelL(kFirstData+1)<< " and "<<t->getLabelL(kFirstData)<<","<<t->getLabelL(kFirstData+1) << " (clusters: "<<f->getParentCluster()<< " and "<<t->getParentCluster() << ")";
		std::cout <<"(weight: "<<e->getWeight()<<" caps: "<<e->getCapability() << " clearance: "<<e->getClearance(e->getCapability())<<")";
		e = absg->edgeIterNext(ei);
	}
*/	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of clusters created", numExpectedClusters, aca->getNumClusters());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract nodes", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of abstract edges", numExpectedAbstractEdges, absg->getNumEdges());

	/* check if expected edges exist */
	node* mynode = absg->getNode(aca->getNodeFromMap(0,4)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (0,4)", 3, mynode->getNumEdges());
	mynode = absg->getNode(aca->getNodeFromMap(0,5)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (0,5)", 2, mynode->getNumEdges());

	mynode = absg->getNode(aca->getNodeFromMap(5,1)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (5,1)", 2, mynode->getNumEdges());
	
	mynode = absg->getNode(aca->getNodeFromMap(5,4)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (5,4)", 3, mynode->getNumEdges());

	mynode = absg->getNode(aca->getNodeFromMap(4,4)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (4,4)", 2, mynode->getNumEdges());

	mynode = absg->getNode(aca->getNodeFromMap(4,5)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (4,5)", 3, mynode->getNumEdges());

	mynode = absg->getNode(aca->getNodeFromMap(5,5)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong # of edges for node @ (5,5)", 2, mynode->getNumEdges());
	
	/* check if clusters have correct number of abstract nodes */
	AnnotatedCluster* myac = aca->getCluster(0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster ac0 has wrong abstract node count", 3, (int)myac->getParents().size());

	myac = aca->getCluster(1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster ac1 has wrong abstract node count", 2, (int)myac->getParents().size());

	myac = aca->getCluster(2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster ac1 has wrong abstract node count", 2, (int)myac->getParents().size());

	myac = aca->getCluster(3);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster ac1 has wrong abstract node count", 1, (int)myac->getParents().size());


}

void AnnotatedClusterAbstractionTest::buildEntrancesShouldResultInOneCachedPathForEachAbstractEdge()
{
	delete aca; // map too big for this test; use a trivial one instead
	Map* tinymap = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(tinymap, new AnnotatedAStar(), TESTCLUSTERSIZE);
	
	AnnotatedClusterFactory* ac_factory = new AnnotatedClusterFactory();
	aca->buildClusters(ac_factory);
	delete ac_factory;
	
	int numExpectedCachedPaths = 17; 
	
	aca->buildEntrances();
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("buildEntrances resulted in incorrect number of paths in cache", numExpectedCachedPaths, (int)aca->pathCache.size());
}
void AnnotatedClusterAbstractionTest::buildEntrancesShouldAskEachClusterToCreateItsOwnEntrances()
{
	acmock_factory->setTest(2);

	int numExpectedClusters = 4;
	aca->buildClusters(acmock_factory);
	
	/* run test */
	aca->buildEntrances();
	
	for(int i=0;i<numExpectedClusters;i++)
	{
		AnnotatedClusterMock* acm = dynamic_cast<AnnotatedClusterMock*>(aca->getCluster(i));
		acm->verify();
	}
}

/* integration test; TODO: add proper support for mock clusters here */
void AnnotatedClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(6,5);
	graph* absg = aca->getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted node", true, aca->startid != -1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted node", true, aca->goalid != -1);
	

	node* absstart = absg->getNode(aca->startid);
	node* absgoal = absg->getNode(aca->goalid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add start node to abstract graph", true, start->getLabelL(kFirstData) == absstart->getLabelL(kFirstData) && start->getLabelL(kFirstData+1) == absstart->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add goal node to abstract graph", true, goal->getLabelL(kFirstData) == absgoal->getLabelL(kFirstData) && goal->getLabelL(kFirstData+1) == absgoal->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of nodes added to abstract graph", numAbstractNodes+2, absg->getNumNodes());
	
	delete acfactory;
}

/* integration test; TODO: add mock clusters here */
void AnnotatedClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldNotCreateNewAbstractNodesIfASuitableNodeAlreadyExistsInTheAbstractGraph()
{		
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node* start = aca->getNodeFromMap(4,1); // created in abstract graph while building entrances
	node* goal = aca->getNodeFromMap(6,5);
	graph* absg = aca->getAbstractGraph(1);	
	int numAbstractNodes = absg->getNumNodes();
	int numExpectedAbstractNodes = numAbstractNodes+1;
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("unexpectedly set startid value to something non-default", true, aca->startid == -1); // no new node created
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted goal node", true, aca->goalid != -1); // new goal node created
	
	node* absgoal = absg->getNode(aca->goalid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add goal node to abstract graph", true, goal->getLabelL(kFirstData) == absgoal->getLabelL(kFirstData) && goal->getLabelL(kFirstData+1) == absgoal->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of nodes added to abstract graph", numExpectedAbstractNodes, absg->getNumNodes());
	
	delete acfactory;
}

/* integration test; TODO: substitute production code with mock AA* and ACMock */
void AnnotatedClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldConnectAnyNewNodesToAllOtherAbstractNodesInTheParentCluster()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(6,5);
	graph* absg = aca->getAbstractGraph(1);	
	int numAbstractEdges = absg->getNumEdges();
	int numExpectedAbstractEdges = numAbstractEdges+5; // 3 new edges in start cluster and 2 new edges in goal cluster
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);	
	
	//debugging -- man, I really need a printgraph helper method
/*	edge_iterator ei = absg->getEdgeIter();
	edge* e = absg->edgeIterNext(ei);
	while(e)
	{
		node* f = absg->getNode(e->getFrom());
		node* t = absg->getNode(e->getTo());
		cout << "\n edge connects "<<f->getLabelL(kFirstData)<<","<<f->getLabelL(kFirstData+1)<< " and "<<t->getLabelL(kFirstData)<<","<<t->getLabelL(kFirstData+1);
		cout <<"(weight: "<<e->getWeight()<<" caps: "<<e->getCapability() << " clearance: "<<e->getClearance(e->getCapability())<<")";
		e = absg->edgeIterNext(ei);
	}	

*/	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of edges added to abstract graph", numExpectedAbstractEdges, absg->getNumEdges());
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableStartNodeParameters()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node* start = aca->getNodeFromMap(2,1);
	node* goal = aca->getNodeFromMap(5,3);	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);	
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionGivenNonTraversableGoalNodeParameters()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(5,3);	
	aca->insertStartAndGoalNodesIntoAbstractGraph(goal, start);	
}


void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeHasAbstractionLevelGreaterThanZero()
{
	node* n1 = new node("");
	node* n2 = new node("");
	
	n1->setTerrainType(kGround);
	n1->setClearance(kGround,1);
	n2->setTerrainType(kGround);
	n2->setClearance(kGround,1);

		
	bool exceptionThrown = false;
	try 
	{
		aca->insertStartAndGoalNodesIntoAbstractGraph(n1,n2);
	}
	catch(NodeHasNonZeroAbstractionLevelException& e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_MESSAGE("failed to throw exception when start node has label kAbstractionLevel > 0", exceptionThrown == true);
	
	exceptionThrown = false;
	try 
	{
		aca->insertStartAndGoalNodesIntoAbstractGraph(n2,n1);
	}
	catch(NodeHasNonZeroAbstractionLevelException& e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_MESSAGE("failed to throw exception when goal node has label kAbstractionLevel > 0", exceptionThrown == true);
	
	delete n1;
	delete n2;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeIsNull()
{
	node* n1 = new node("");
	bool exceptionThrown = false;
	try 
	{
		aca->insertStartAndGoalNodesIntoAbstractGraph(NULL,n1);
	}
	catch(NodeIsNullException& e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_MESSAGE("failed to throw exception when start node is null", exceptionThrown == true);
	
	exceptionThrown = false;
	try 
	{
		aca->insertStartAndGoalNodesIntoAbstractGraph(n1,NULL);
	}
	catch(NodeIsNullException& e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_MESSAGE("failed to throw exception when goal node is null", exceptionThrown == true);
	
	delete n1;
}

/* integration test; TODO: substitute in mock objects */
void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromAbstractGraph()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();

	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(3,5);
	graph* absg = aca->getAbstractGraph(1);	

	aca->buildClusters(acfactory);
	aca->buildEntrances();

	int numExpectedAbstractNodes = absg->getNumNodes();
	int numExpectedAbstractEdges = absg->getNumEdges();
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	
	aca->removeStartAndGoalNodesFromAbstractGraph(); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", numExpectedAbstractEdges, absg->getNumEdges());
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldResetStartIDAndGoalIDToDefaultValues()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(3,5);

	aca->buildClusters(acfactory);
	aca->buildEntrances();
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	
	aca->removeStartAndGoalNodesFromAbstractGraph(); 
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to reset startid to default value -1", -1, aca->startid);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("faild to reset goalid to default value -1", -1, aca->goalid);
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldNotDeleteAnyNodesOriginallyInTheAbstractGraph()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	node* start = aca->getNodeFromMap(4,1); 
	node* goal = aca->getNodeFromMap(3,5);
	graph* absg = aca->getAbstractGraph(1);	

	aca->buildClusters(acfactory);
	aca->buildEntrances();

	int numExpectedAbstractNodes = absg->getNumNodes();
	int numExpectedAbstractEdges = absg->getNumEdges();
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	aca->removeStartAndGoalNodesFromAbstractGraph(); 	
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong when deleting start node only", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong when deleting start node only", numExpectedAbstractEdges, absg->getNumEdges());
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(goal, start);	
	aca->removeStartAndGoalNodesFromAbstractGraph(); 	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong when deleting goal node only", numExpectedAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong when deleting goal node only", numExpectedAbstractEdges, absg->getNumEdges());
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllNodesAndEdgesAddedByInsertionMethodFromParentClusters()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(3,5);

	aca->buildClusters(acfactory);
	aca->buildEntrances();

	int numExpectedStartClusterNodes = aca->getCluster(start->getParentCluster())->getParents().size();
	int numExpectedGoalClusterNodes = aca->getCluster(goal->getParentCluster())->getParents().size();

	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	aca->removeStartAndGoalNodesFromAbstractGraph(); 

	int numActualStartClusterNodes = aca->getCluster(start->getParentCluster())->getParents().size();
	int numActualGoalClusterNodes = aca->getCluster(goal->getParentCluster())->getParents().size();
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in start cluster is wrong", numExpectedStartClusterNodes, numActualStartClusterNodes);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count in goal cluster is wrong", numExpectedGoalClusterNodes, numActualGoalClusterNodes);
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldSet_kParent_LabelOfOriginalNodeEqualToIdIfANewNodeIsAddedToAbstractGraph()
{

	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(3,5);
	graph* absg = aca->getAbstractGraph(1);
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	
	node* absstart = absg->getNode(aca->startid);
	node* absgoal = absg->getNode(aca->goalid);
	
	int startExpectedValue = absstart->getNum();
	int startActualValue = start->getLabelL(kParent);
	int goalExpectedValue = absgoal->getNum();
	int goalActualValue = goal->getLabelL(kParent);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to set kParent label of original start node to id of new abstract node", startExpectedValue, startActualValue);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to set kParent label of original goal node to id of new abstract node", goalExpectedValue, goalActualValue);
	
	delete acfactory;	
}

void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldResetToDefault_kParent_LabelOfOriginalNodesForWhichANewNodeWasInsertedIntoAbstractGraph()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(6,5);

	aca->buildClusters(acfactory);
	aca->buildEntrances();
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);	
	aca->removeStartAndGoalNodesFromAbstractGraph(); 
	
	int expectedValue = -1;
	int actualStartValue = start->getLabelL(kParent);
	int actualGoalValue = goal->getLabelL(kParent);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("kParent label of original start node not reset to -1", expectedValue, actualStartValue);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("kParent label of original goal node not reset to -1", expectedValue, actualGoalValue);
		
	delete acfactory;
}

/* similar test to the one we do on adding endpoints in the cluster method
	TODO: investigate if we can create a reusable method for both
*/
void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldSet_kAbstractionLevel_LabelOfNewNodesToPointToTheCorrectAbstractGraph()
{

	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(3,5);
	graph* absg = aca->getAbstractGraph(1);
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	
	node* absstart = absg->getNode(aca->startid);
	node* absgoal = absg->getNode(aca->goalid);
	
	int startExpectedValue = start->getLabelL(kAbstractionLevel)+1;
	int startActualValue = absstart->getLabelL(kAbstractionLevel);
	int goalExpectedValue = goal->getLabelL(kAbstractionLevel)+1;
	int goalActualValue = absgoal->getLabelL(kAbstractionLevel);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to increment kAbstractionLevel label of new abstract start node", startExpectedValue, startActualValue);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to increment kAbstractionLevel label of new abstract goal node", goalExpectedValue, goalActualValue);
	
	delete acfactory;	
}

/* TODO: isolate distance method. relies on way too much production code working just to test a trivial function */
void AnnotatedClusterAbstractionTest::distanceShouldCalculateTheWeightOfTheShortestPathBetweenTwoNodes()
{
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(new Map(acmap.c_str()), new AnnotatedAStarMock());
	AnnotatedAStar aastar;
	
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 	
	
	node* start = ama->getNodeFromMap(2,1);
	node* goal = ama->getNodeFromMap(4,5);
	
	aastar.setCapability(kGround);
	aastar.setClearance(1);
	path* p = aastar.getPath(ama, start, goal); 
	double expectedDist = (int(ama->distance(p)*100+0.5))/100.0;
	double actualDist = (int(aca->distance(p)*100+0.5))/100.0;
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("distance fails to produce correct result", expectedDist, actualDist);
	
	delete ama;
	delete p;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(3,5);
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();
	
	//aca->pathCache;
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
			
/*	debugging
	cout << "\n nodes expanded: "<<aca->getNodesExpanded();
	cout << "\n nodes touched: "<<aca->getNodesTouched();
	cout << "\n peak memory: "<<aca->getPeakMemory();
	cout << "\n searchTime: "<<aca->getSearchTime();
*/

	CPPUNIT_ASSERT_MESSAGE("did not record anything for nodesExpanded", aca->getNodesExpanded() > 0);
	CPPUNIT_ASSERT_MESSAGE("did not record anything for nodesTouched", aca->getNodesTouched() > 0);
	CPPUNIT_ASSERT_MESSAGE("did not record anything for peakMemory", aca->getPeakMemory() > 0);
	CPPUNIT_ASSERT_MESSAGE("did not record anything for searchTime", aca->getSearchTime() > 0);
	
	delete acfactory;	
}

void AnnotatedClusterAbstractionTest::addPathToCacheShouldStoreAPathGivenAnEdge()
{
	graph* g = new graph();
	node* n1 = new node("");
	node* n2 = new node("");
	g->addNode(n1);
	g->addNode(n2);
	path* p = new path(n1, new path(n2, NULL));
		
	edge* e = new edge(p->n->getNum(), p->next->n->getNum(), 1.0);
	g->addEdge(e);
	
	aca->addPathToCache(e, p);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cache path count incorrect", 1, (int)aca->pathCache.size());
	CPPUNIT_ASSERT_MESSAGE("wrong path retrieved", p == aca->pathCache[e->getUniqueID()]);
	delete g;
}

void AnnotatedClusterAbstractionTest::addPathToCacheShouldDoNothingIfEdgeOrPathParametersAreNull()
{
	path* p = new path(NULL, NULL);
	edge* e = new edge(0, 1, 1);
	
	aca->addPathToCache(NULL, p);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)aca->pathCache.size());
	
	aca->addPathToCache(e, NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path added when edge param is null", 0, (int)aca->pathCache.size());
	
	delete p;
	delete e;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldAddToCacheAPathForEachNewlyCreatedEdge()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(6,5);
	graph* absg = aca->getAbstractGraph(1);
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	int numExpectedPathsInCache = absg->getNumEdges()+5; // 3 new cached paths in start cluster and 2 more in goal cluster
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of paths added to cache", numExpectedPathsInCache, (int)aca->pathCache.size());
	
	delete acfactory;	

}

void AnnotatedClusterAbstractionTest::removeStartAndGoalNodesFromAbstractGraphShouldDeleteAllPathsAddedByInsertionMethod()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	node* start = aca->getNodeFromMap(2,1); 
	node* goal = aca->getNodeFromMap(3,5);

	aca->buildClusters(acfactory);
	aca->buildEntrances();

	int numPathsInCacheBefore = aca->getPathCacheSize();
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	aca->removeStartAndGoalNodesFromAbstractGraph(); 
	int numPathsInCacheAfter = aca->getPathCacheSize();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path cache size is wrong", numPathsInCacheBefore, numPathsInCacheAfter);
	
	delete acfactory;	
}

void AnnotatedClusterAbstractionTest::getPathFromCacheShouldReturnAPathGivenAValidEdge()
{
	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	aca->addPathToCache(e,p);
	path* ret = aca->getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void AnnotatedClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnInvalidEdge()
{
	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	
	aca->addPathToCache(e,p);
	path* ret = aca->getPathFromCache(e);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an appropriate path from cache", p == ret);
	
	delete n;
	delete e;
}

void AnnotatedClusterAbstractionTest::getPathFromCacheShouldReturnZeroGivenAnEdgeThatHasNoCorrespondingPathInCache()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 


	node* n = new node("");
	path* p = new path(n, NULL);
	edge* e = new edge(0,1,1);
	edge* e2 = new edge(1,1,1);
	
	aca->addPathToCache(e,p);
	
	path* ret = aca->getPathFromCache(e2);
	
	CPPUNIT_ASSERT_MESSAGE("failed to return an NULL value given an invalid edge", NULL == ret);

	delete n;
	delete e;
	delete e2;
}

/* original h method makes certain assumptions about abstract nodes -- like they don't have x/y tile coordinates so we have to figure out 
where they are based on their children. ACA's abstraction technque is far simpler but unfortunately, the old method breaks when passing
one of our abstract nodes. We override the original function with the correct functionality to fix the issue */
void AnnotatedClusterAbstractionTest::hShouldProduceIdenticalResultsToOverriddenMethodInMapAbstractionGivenTwoValidNodeParameters()
{
	node* a = aca->getNodeFromMap(14,7);
	node* b = aca->getNodeFromMap(12,7);
	
	double expectedResult = aca->mapAbstraction::h(a,b);
	double result = aca->h(a,b);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("1. h failed to produce result identical to mapAbstraction", expectedResult, result);

/*	a = aca->getNodeFromMap(15,7);
	b = aca->getNodeFromMap(12,7);
	result = aca->h(a,b);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("2. h failed to produce result identical to mapAbstraction", fabs(aca->mapAbstraction::h(a,b)), fabs(result));*/
}

void AnnotatedClusterAbstractionTest::hShouldThrowExceptionGivenANullNodeParameter()
{
		aca->h(NULL, NULL);
}

void AnnotatedClusterAbstractionTest::setupDominanceRelationshipTestData(int dmCapability, int dmClearance, int dtCapability, int dtClearance)
{
	graph *absg = aca->getAbstractGraph(1);
	
	/* pick some arbitrary locations for the transitions on the low-level map */
	n1 = dynamic_cast<node*>(aca->getNodeFromMap(0,0)->clone());
	aca->getNodeFromMap(0,0)->setLabelL(kParent, n1->getNum());
	n2 = dynamic_cast<node*>(aca->getNodeFromMap(1,0)->clone());
	aca->getNodeFromMap(1,0)->setLabelL(kParent, n2->getNum());
	n3 = dynamic_cast<node*>(aca->getNodeFromMap(0,1)->clone());
	aca->getNodeFromMap(0,1)->setLabelL(kParent, n3->getNum());
	n4 = dynamic_cast<node*>(aca->getNodeFromMap(1,1)->clone());
	aca->getNodeFromMap(1,1)->setLabelL(kParent, n4->getNum());
	absg->addNode(n1);
	absg->addNode(n2);
	absg->addNode(n3);
	absg->addNode(n4);

	/* dummy up a couple of clusters; connected by 2 transitions initially */
	AnnotatedCluster* ac = new AnnotatedCluster(0, 0, 2, 1);
	ac->setClusterId(0);
	ac->getParents().push_back(n1);
	ac->getParents().push_back(n3);
	aca->addCluster(ac);	
	AnnotatedCluster* ac2 = new AnnotatedCluster(0,1,2,1);
	ac2->setClusterId(1);
	ac2->getParents().push_back(n2);
	ac2->getParents().push_back(n4);
	aca->addCluster(ac2);
	n1->setParentCluster(0);
	n2->setParentCluster(1);
	n3->setParentCluster(0);
	n4->setParentCluster(1);

	/* create inter-edges */
	e1 = new edge(n1->getNum(),n2->getNum(),1);
	e2 = new edge(n3->getNum(),n4->getNum(),1);
	e1->setClearance(dmCapability, dmClearance);  // dominant edge
	e2->setClearance(dtCapability, dtClearance); // dominated edge
	absg->addEdge(e1);
	absg->addEdge(e2);
	

	/* complete the circuit; an equivalent path between the endpoints of the dominated transition via the dominant transition must exist */
	intra1 = new edge(n1->getNum(), n3->getNum(), 1);
	intra1->setClearance(dtCapability,dtClearance);
	absg->addEdge(intra1);
	intra2 = new edge(n2->getNum(), n4->getNum(), 1);
	intra2->setClearance(dtCapability,dtClearance);
	absg->addEdge(intra2);
}


void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenTwoInteEdgesEitherOfWhichOrBothAreNull()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);	
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	/* test1; first transition parameter is null */
	absg->removeEdge(e1);	
	delete e1;
	e1 = 0;
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());
	
	/* test2; second transition parameter is null */
	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* test3; both transition parameters are null */
	absg->removeEdge(e2);
	delete e2;
	e2 = 0;
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra2", false, intra2->getMarked());
}

/* in this test e1 capability is simpler than e2 capability but corridor is narrower so e1 does not dominate e2 */
void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesDominantEdgeGivenTwoInterEdgesWithIntersectingCapabilityButNoCorridorSizeDominance()
{
	int dmClearance=2;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	
	/* test1; param order e1, e2 */
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* test2; reverse parameter order */
	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());
}

/* in this test, we create edges with endpoints that do not exist in the abstract graph */
void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenOneOrTwoInteredgesWhichReferenceInvalidNodeIDsAsTheirEndpoints()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);	
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	/* test1; first parameter references nodeids not in the abstract graph */
	absg->removeEdge(e1);
	delete e1;
	e1 = new edge(99, 100, 1);
	e1->setClearance(dmCapability, dmClearance);
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* test2; reverse param order */
	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());
	
	delete e1;
}

/* in this test, we create two edges which represent transitions between different clusters */
void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithUniqueEndpointsWhichDoNotConnectTheSameSetOfClusters()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	
	n1->setParentCluster(0);
	n2->setParentCluster(1);
	n3->setParentCluster(2);
	n4->setParentCluster(3);

	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());
}

/* in this test, we're mainly testing the endpoint swapping code;
	a bug previously existed where endpoints would be swapped twice to try and match up their clusters when no match was possible */
void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesGivenAPairofEdgesWithSomeSharedEndpointsWhichDoNotConnectTheSameSetOfClusters()
{
	int dmClearance=2;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = kGround;
	graph* absg = aca->getAbstractGraph(1);

	n1 = new node("n1");
	n2 = new node("n2");
	n3 = new node("n3");
	absg->addNode(n1);
	absg->addNode(n2);
	absg->addNode(n3);
	n1->setParentCluster(1);
	n2->setParentCluster(1);
	n3->setParentCluster(2);
	
	e1 = new edge(n1->getNum(), n2->getNum(), 1);
	e1->setClearance(dmCapability, dmClearance);
	e2 = new edge(n3->getNum(), n1->getNum(), 1);
	e2->setClearance(dtCapability, dtClearance);
	absg->addEdge(e1);
	absg->addEdge(e2);

	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotUnmarkAnyEdgesWhichAreAlreadyMarked()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);

	e2->setMarked(true);
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly un-marked e2", true, e2->getMarked());
}


void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfNoCircuitExistsBetweenEndpointsOfTwoTransitionsWhereOneDomiantesTheOther()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);
		
	/* test1; break the circuit by removing the intra-edge connecting the endpoints in one cluster */
	absg->removeEdge(intra1); 
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* test2; as above, but with the intra-edge from the other cluster */
	absg->addEdge(intra1);
	absg->removeEdge(intra2); 
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	
	/* test3; remove both */
	absg->removeEdge(intra1);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e2", false, e2->getMarked());

	delete intra1;
	delete intra2;
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitDoNotHaveSufficentClearance()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);

	/* test1; intra-edge in cluster1 is too narrow to be traversed by all agents able to traverse the dominated transition (circuit is invalid) */
	intra1->setClearance(dtCapability, dtClearance-1);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());
	
	/* test2; as above, other intra-edge */
	intra1->setClearance(dtCapability, dtClearance);
	intra2->setClearance(dtCapability, dtClearance-1);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());

	/* test3; both */
	intra1->setClearance(dtCapability, dtClearance-1);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra2", false, intra2->getMarked());
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfTheIntraEdgesInTheCircuitAreNotTraversableByTheCapabilityOfTheDominatedTransition()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	
	/* test1; intra-edge in cluster1 is not traversable by all agents able to traverse the dominated transition (circuit is invalid) */
	intra1->setClearance(kWater, dtClearance);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());
	
	/* test2; as above, other intra-edge */
	intra1->setClearance(dtCapability, dtClearance);
	intra2->setClearance(kWater, dtClearance);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());

	/* test3; both */
	intra1->setClearance(kWater, dtClearance);
	aca->findAndMarkDominatedTransition(e1, e2);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test3; incorrectly marked intra2", false, intra2->getMarked());
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfEitherParameterIsAnIntraEdge()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);

	n1->setParentCluster(0);
	n2->setParentCluster(0);
	n3->setParentCluster(0);
	n4->setParentCluster(1);
	
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());

}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldNotMarkAnyEdgesIfBothParameterEdgesAreIntraEdgesThatBelongToSameCluster()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	
	n1->setParentCluster(0);
	n2->setParentCluster(0);
	n3->setParentCluster(0);
	n4->setParentCluster(0);
	
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e2", false, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());
}

/* in this test we verify findDominant doesn't get confused if the cluster of e1->from isn't the same as e2->from 
(but both edges connect the same pair of clusters) */
void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWhereOneDominatesTheOtherBut__TO__and__FROM__AttributesOfEndpointsIsSwapped()
{
	int dmClearance=3;
	int dtClearance=2;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);
	
	/* test1; reverse order of to/from for dominant edge e1 */
	absg->removeEdge(e1);
	delete e1;
	e1 = new edge(n2->getNum(), n1->getNum(), 1); 
	e1->setClearance(dmCapability,dmClearance);
	absg->addEdge(e1);
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; failed to mark e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* reset test data for test2 */
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 	
	absg = aca->getAbstractGraph(1);

	/* test2; as test1 but swap endpoints of dominated edge */	
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	absg->removeEdge(e2);
	delete e2;
	e2 = new edge(n4->getNum(), n3->getNum(), 1);
	e2->setClearance(dtCapability, dtClearance);
	absg->addEdge(e2);
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; failed to maek e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithTheSameCapabilityButDifferentClearance()
{
	int dmClearance=3;
	int dtClearance=1;
	int dmCapability = kGround;
	int dtCapability = kGround;
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; failed to mark e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* reset test data */
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 
	absg = aca->getAbstractGraph(1);	

	/* test2; as above, param order reversed */
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; failed to mark e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());
}

void AnnotatedClusterAbstractionTest::findAndMarkDominatedTransitionShouldMarkADominatedEdgeGivenTwoInterEdgesWithIntersectingCapabilitySets()
{	
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);
	
	aca->findAndMarkDominatedTransition(e1, e2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; failed to mark e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; incorrectly marked intra2", false, intra2->getMarked());

	/* reset test data */
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 	
	absg = aca->getAbstractGraph(1);

	/* test2; as above, param order reversed */
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	aca->findAndMarkDominatedTransition(e2, e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked e1", false, e1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; failed to mark e2", true, e2->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra1", false, intra1->getMarked());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test2; incorrectly marked intra2", false, intra2->getMarked());
}

void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldDeleteAnyDominatedEdgeButNotAnyEndpointsWhichConnectOtherInterEdges()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	int numExpectedEdges = 5;
	int numExpectedNodes = 6;
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	/* mark dominated edges */
	e2->setMarked(true);

	/* setup extra inter-edges from e2's endpoints to other clusters */
	node* n5 = dynamic_cast<node*>(aca->getNodeFromMap(0,2)->clone());
	node* n6 = dynamic_cast<node*>(aca->getNodeFromMap(1,2)->clone());
	absg->addNode(n5);
	absg->addNode(n6);
	n5->setParentCluster(2);
	n6->setParentCluster(3);
	aca->getNodeFromMap(0,2)->setLabelL(kParent, n5->getNum());
	aca->getNodeFromMap(1,2)->setLabelL(kParent, n6->getNum());
	edge* extraInterEdge = new edge(n3->getNum(), n5->getNum(), 1);
	edge* extraInterEdge2 = new edge(n4->getNum(), n6->getNum(), 1);
	extraInterEdge->setClearance(kTrees,2);
	extraInterEdge2->setClearance(kTrees,2);
	absg->addEdge(extraInterEdge);
	absg->addEdge(extraInterEdge2);	

	/* delete dominated edge but not its endpoints which are required by other inter-edges */
	aca->removeDominatedEdgesAndEndpoints();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. edge count wrong", numExpectedEdges, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. node count wrong", numExpectedNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; dominant edge missing from graph", true, n1->findAnnotatedEdge(n2, dmCapability, dmClearance, 100) == e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; dominated edge not deleted from graph", true, n3->findAnnotatedEdge(n4, dtCapability, dtClearance, 100) == 0);
}

void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldDeleteAllMarkedEdgesAndAnyEndpointsWhichAreNotRequiredByOtherInterEdges()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	int numExpectedEdges = 1;
	int numExpectedNodes = 2;
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	/* mark dominated edges */
	e2->setMarked(true);

	/* delete dominated edge and its endpoints */
	aca->removeDominatedEdgesAndEndpoints();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. edge count wrong", numExpectedEdges, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. node count wrong", numExpectedNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; dominant edge missing from graph", true, n1->findAnnotatedEdge(n2, dmCapability, dmClearance, 100) == e1);
}

void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldNotDeleteAnythingIfNoEdgesAreMarked()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	int numExpectedEdges = 4;
	int numExpectedNodes = 4;
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);
	graph* absg = aca->getAbstractGraph(1);

	/* nothing marked so nothing to delete */
	aca->removeDominatedEdgesAndEndpoints();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. edge count wrong", numExpectedEdges, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1. node count wrong", numExpectedNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; dominant edge missing from graph", true, n1->findAnnotatedEdge(n2, dmCapability, dmClearance, 100) == e1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("test1; dominated edge not deleted from graph", true, n3->findAnnotatedEdge(n4, dtCapability, dtClearance, 100) == e2);
}

void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldRemove__kParent__LabelOfLowLevelNodesThatWereAbstractedByADeletedNode()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dmCapability, dmClearance, dtCapability, dtClearance);

	node* n1ll = aca->getNodeFromMap(n1->getLabelL(kFirstData), n1->getLabelL(kFirstData+1));
	node* n2ll = aca->getNodeFromMap(n2->getLabelL(kFirstData), n2->getLabelL(kFirstData+1));
	node* n3ll = aca->getNodeFromMap(n3->getLabelL(kFirstData), n3->getLabelL(kFirstData+1));
	node* n4ll = aca->getNodeFromMap(n4->getLabelL(kFirstData), n4->getLabelL(kFirstData+1));

	int expectedValue = -1; // default value for a node without a parent in the abstract graph
	
	/* deletete dominated endpoints and check if low-level nodes have had their kParent labels updated (dominant endpoints should not be touched) */
	e2->setMarked(true);
	aca->removeDominatedEdgesAndEndpoints();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln1 has wrong kParent label", (int)n1->getNum(), (int)n1ll->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln2 has wrong kParent label", (int)n2->getNum(), (int)n2ll->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln3 has wrong kParent label", expectedValue, (int)n3ll->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln4 has wrong kParent label", expectedValue, (int)n4ll->getLabelL(kParent));
}

void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldRepair__kParent__LabelsOfLowLevelsNodes()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dtCapability, dtClearance, dmCapability, dmClearance); // endpoints of dominated edge first in nodes array

	/* test that low-level nodes which are abstracted by the endpoints of the dominant edge have their kParent labels updated to reflect changes
	in abstract graph  */
	node* n3ll = aca->getNodeFromMap(n3->getLabelL(kFirstData), n3->getLabelL(kFirstData+1)); 
	node* n4ll = aca->getNodeFromMap(n4->getLabelL(kFirstData), n4->getLabelL(kFirstData+1));

	e1->setMarked(true);
	aca->removeDominatedEdgesAndEndpoints();	
	int n3ExpectedValue = n3->getNum(); // index updated after deletion 
	int n4ExpectedValue = n4->getNum();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln3 has wrong kParent label", n3ExpectedValue, (int)n3ll->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("lln4 has wrong kParent label", n4ExpectedValue, (int)n4ll->getLabelL(kParent));
}

/* annotated clusters store a list of abstract nodes in their perimeter. need to update/repair this list after removing dominated edges and endpoints */
void AnnotatedClusterAbstractionTest::removeDominatedEdgesAndEndpointsShouldRepairAbstractNodeCollectionInEachAffectedCluster()
{
	int dmClearance=3;
	int dtClearance=3;
	int dmCapability = kGround;
	int dtCapability = (kGround|kTrees);
	setupDominanceRelationshipTestData(dtCapability, dtClearance, dmCapability, dmClearance); // endpoints of dominated edge first in nodes array
	AnnotatedCluster* ac = aca->getCluster(0);
	AnnotatedCluster* ac2 = aca->getCluster(1);
	
	e1->setMarked(true);
	aca->removeDominatedEdgesAndEndpoints();
	
	int expectedNodesInCluster = 1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count in cluster 'ac' incorrect. failed to remove deleted endpoints?", expectedNodesInCluster, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count in cluster 'ac2' incorrect. failed to remove deleted endpoints?", expectedNodesInCluster, (int)ac2->getParents().size());
	node* aclast = ac->getParents().back();
	node* ac2last = ac2->getParents().back();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("found wrong node in 'ac' collection!", true, n3 == aclast);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("found wrong node in 'ac2' collection!", true, n4 == ac2last);
}
