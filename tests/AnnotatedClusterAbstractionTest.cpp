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
#include "TestConstants.h"
#include <mockpp/chaining/ChainingMockObjectSupport.h>


CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterAbstractionTest );

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
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid < 0", expectedValue, (int)result);
}

void AnnotatedClusterAbstractionTest::getClusterShouldReturnZeroWhenIdParameterIsGreaterThanNumberOfClusters()
{
	int expectedValue = 0;
	int clusterid=aca->getNumClusters()+1;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Non-zero return value when clusterid >= numclusters", expectedValue, (int)aca->getCluster(clusterid));
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
	int numExpectedAbstractEdges = 16; // includes intercluster and intracluster transitions
	int numExpectedAbstractNodes = 10;
	
	aca->buildEntrances();

	graph* absg = aca->getAbstractGraph(1);
	
/*	debugging
	edge_iterator ei = absg->getEdgeIter();
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

void AnnotatedClusterAbstractionTest::buildEntrancesShouldAskEachClusterToCreateItsOwnEntrances()
{
	acmock_factory->setTest(2);

	int numExpectedClusters = 4;
	aca->buildClusters(acmock_factory);
	
	/* run test */
	aca->buildEntrances();
	
	for(int i=0;i<4;i++)
	{
		AnnotatedClusterMock* acm = dynamic_cast<AnnotatedClusterMock*>(aca->getCluster(i));
		acm->verify();
	}
}

/* integration test; TODO: add proper support for mock clusters here */
void AnnotatedClusterAbstractionTest::insertStartAndGoalNodesIntoAbstractGraphShouldAddTwoNewNodesIntoTheAbstractGraphAndParentClusters()
{
	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(5,3);
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
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();

	node* start = aca->getNodeFromMap(4,1); // created in abstract graph while building entrances
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);	
	int numAbstractNodes = absg->getNumNodes();
	int numExpectedAbstractNodes = numAbstractNodes+1;
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted node", true, aca->startid == -1); // no new node created
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store id of newly inserted node", true, aca->goalid != -1); // new goal node created
	
	node* absstart = absg->getNode(start->getLabelL(kParent));
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
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);	
	int numAbstractEdges = absg->getNumEdges();
	int numExpectedAbstractEdges = numAbstractEdges+4;
	
	aca->insertStartAndGoalNodesIntoAbstractGraph(start, goal);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of edges added to abstract graph", numExpectedAbstractEdges, absg->getNumEdges());
	
	delete acfactory;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldThrowExceptionIfStartOrGoalNodeHasAbstractionLevelGreaterThanZero()
{
	node* n1 = new node("");
	node* n2 = new node("");
		
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
	node* goal = aca->getNodeFromMap(5,3);
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
	node* goal = aca->getNodeFromMap(5,3);

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
	node* goal = aca->getNodeFromMap(5,3);
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
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);	

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
	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
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
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);	

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
	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
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
	
	node* start = ama->getNodeFromMap(2,1);
	node* goal = ama->getNodeFromMap(4,5);
	
	path* p = aastar.getPath(ama, start, goal, kGround, 1);
	
	double expectedDist = (int(ama->distance(p)*100+0.5))/100.0;
	double actualDist = (int(aca->distance(p)*100+0.5))/100.0;
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("distance fails to produce correct result", expectedDist, actualDist);
	
	delete ama;
}

void AnnotatedClusterAbstractionTest::insertStartAndGoalIntoAbstractGraphShouldRecordStatisticsToMeasureInsertionEffort()
{
	delete aca;
	Map* m  = new Map(acmap.c_str());
	aca = new AnnotatedClusterAbstraction(m,new AnnotatedAStar(), TESTCLUSTERSIZE); 

	node* start = aca->getNodeFromMap(0,0);	
	node* goal = aca->getNodeFromMap(5,3);
	graph* absg = aca->getAbstractGraph(1);
	int numAbstractNodes = absg->getNumNodes();
	
	AnnotatedClusterFactory* acfactory = new AnnotatedClusterFactory();
	aca->buildClusters(acfactory);
	aca->buildEntrances();
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
