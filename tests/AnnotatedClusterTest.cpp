/*
 *  AnnotatedClusterTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 27/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "AnnotatedClusterTest.h"
#include "TestConstants.h"
#include "AHAConstants.h"
#include "AnnotatedCluster.h"
#include "AnnotatedClusterAbstractionMock.h"
#include "AnnotatedAStarMock.h"
#include "AnnotatedMapAbstractionMock.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterTest );

TestEntrance::TestEntrance(int fromx, int fromy, int tox, int toy, int capability, int clearance, int fromClusterId, int toClusterId)
{
	this->fromx = fromx;
	this->fromy = fromy; 
	this->tox = tox;
	this->toy = toy; 

	this->capability = capability;
	this->clearance = clearance; 
	
	this->fromClusterId = fromClusterId;
	this->toClusterId = toClusterId;
}

int TestEntrance::getClearance(int agentcapability)
{
	if((this->capability & agentcapability) == capability)
		return clearance;

	return 0;
}

void AnnotatedClusterTest::setUp()
{
	cwidth = 5;
	cheight = cwidth;
	interedge_weight=1.0;
	int startx=0; int starty=0;

	ac = new AnnotatedCluster(startx, starty, cwidth, cheight);
	ac->setClusterId(0);
	
	testHelper=0; // to be safe
	aca_mock = new AnnotatedClusterAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock(), cwidth);

	/* setup the mock ACA object. NB/TODO: need to replace neighbours with mock AnnotatedCluster objects?  */
	aca_mock->buildClustersMocker.expects(once());
	aca_mock->getClusterMocker.stubs().with(eq(0)).will(new ReturnStub<AnnotatedCluster*>(ac));
	AnnotatedCluster* adj1 = new AnnotatedCluster(startx, starty+cwidth, cwidth, 1);
	adj1->setClusterId(2);
	aca_mock->getClusterMocker.stubs().with(eq(2)).will(new ReturnStub<AnnotatedCluster*>(adj1));
	
	AnnotatedCluster* adj2 = new AnnotatedCluster(startx+cwidth, starty, 4, cheight);
	adj2->setClusterId(1);
	aca_mock->getClusterMocker.stubs().with(eq(1)).will(new ReturnStub<AnnotatedCluster*>(adj2));

	AnnotatedCluster* adj3 = new AnnotatedCluster(startx+cwidth, starty+cwidth, 4, 1);
	adj3->setClusterId(3);
	aca_mock->getClusterMocker.stubs().with(eq(3)).will(new ReturnStub<AnnotatedCluster*>(adj3));

		
	AnnotatedMapAbstractionMock::loadClearanceInfo(string(aca_mock->getMap()->getMapName()), aca_mock);
	absg = aca_mock->getAbstractGraph(1);	

}

void AnnotatedClusterTest::createEntranceNodes()
{
	/* entrance 1 */
	e1_n1 = aca_mock->getNodeFromMap(4,1);
	e1_n1->setParentCluster(0);
	e1_n2 = aca_mock->getNodeFromMap(5,1);
	e1_n2->setParentCluster(2);
	e1_capability = e1_n1->getTerrainType()|e1_n2->getTerrainType();
	e1_clearance = e1_n1->getClearance(e1_capability)>e1_n2->getClearance(e1_capability)?e1_n2->getClearance(e1_capability):e1_n1->getClearance(e1_capability);		


	/* entrance 2 */
	e2_n1 = aca_mock->getNodeFromMap(4,0);
	e2_n1->setParentCluster(0);
	e2_n2hardobst = aca_mock->getNodeFromMap(5,0);
	e2_n2hardobst->setParentCluster(1);
	e2_capability = e2_n1->getTerrainType()|e2_n2hardobst->getTerrainType();
	e2_clearance = e2_n1->getClearance(e2_capability)>e2_n2hardobst->getClearance(e2_capability)?e2_n2hardobst->getClearance(e2_capability):e2_n1->getClearance(e2_capability);		

	e3_n1 = aca_mock->getNodeFromMap(0,4);
	e3_n1->setParentCluster(0);
	e3_n2 = aca_mock->getNodeFromMap(0,5);
	e3_n2->setParentCluster(1);	
	e3_capability = kGround;
	e3_clearance = e3_n1->getClearance(e3_capability)>e3_n2->getClearance(e3_capability)?e3_n2->getClearance(e3_capability):e3_n1->getClearance(e3_capability);		


	e4_n1 = aca_mock->getNodeFromMap(0,4);
	e4_n1->setParentCluster(0);
	e4_n2 = aca_mock->getNodeFromMap(0,5);
	e4_n2->setParentCluster(1);	
	e4_capability = (kGround|kTrees);
	e4_clearance = e4_n1->getClearance(e4_capability)>e4_n2->getClearance(e4_capability)?e4_n2->getClearance(e4_capability):e4_n1->getClearance(e4_capability);		
	
}

void AnnotatedClusterTest::setupExceptionThrownTestHelper(graph* g, AnnotatedCluster* myac, AnnotatedClusterAbstraction* aca, std::string& errmsg)
{
	createEntranceNodes();
	
	testHelper = new exceptionThrownHelper();
	testHelper->setAbstractGraph(g);
	testHelper->setAnnotatedCluster(myac);
	testHelper->setAnnotatedClusterAbstraction(aca);
	testHelper->setFailMessage(errmsg);
}

void AnnotatedClusterTest::tearDown()
{
	delete aca_mock;
	if(ac)
		delete ac;
	
	if(testHelper)
		delete testHelper;
}

void AnnotatedClusterTest::addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions()
{
	int expectedTotalNodes = cwidth*cheight;	
	ac->addNodesToCluster(aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not match expectations", expectedTotalNodes, ac->getNumNodes());	
}

void AnnotatedClusterTest::addNodeShouldIncrementByOneTotalNodesInCluster()
{
	int expectedTotalNodes = 1;
	node* targetnode = aca_mock->getNodeFromMap(0,0);
	bool result = ac->addNode(targetnode);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to return true when adding a traversable node to the cluster", true, result);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not meet expected total", expectedTotalNodes, ac->getNumNodes());
}

void AnnotatedClusterTest::addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId()
{
	int expectedTotalNodes = 1;
	node* targetnode = aca_mock->getNodeFromMap(0,0);
	ac->addNode(targetnode);
	
	CPPUNIT_ASSERT_EQUAL(ac->getClusterId(), targetnode->getParentCluster());
}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster()
{

	int expectedTotalNodes = 0;
	bool exceptionThrown = false;
	
	node* toAssign = aca_mock->getNodeFromMap(1,1); // pick a node, any node and assign it to some cluster 
	toAssign->setParentCluster(10);
	
	try
	{
		ac->addNode(toAssign);
	}
	catch(NodeIsAlreadyAssignedToClusterException e)
	{
		exceptionThrown = true;
	}

	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception when adding an already assigned node to cluster", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total nodes count unexpectedly changed while trying to add already assigned node to cluster",expectedTotalNodes, ac->getNumNodes());

}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenClusterIsFull()
{
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	
	/* create some test data and fill the cluster */
	for(int i=0; i<expectedTotalNodes; i++)
	{
		node* newnode = new node("");
		newnode->setTerrainType(kGround);
		newnode->setClearance(kGround,1);
		ac->addNode(newnode);
		delete newnode;
	}
	try
	{
		node* toAssign = aca_mock->getNodeFromMap(1,1); // pick a node, any node
		ac->addNode(toAssign);
	}
	catch(ClusterFullException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception when adding node to a full cluster", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total nodes count unexpectedly changed while trying to add node to a full cluster",expectedTotalNodes, ac->getNumNodes());
	
	

}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenNodeParameterIsNull()
{
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;

	try
	{
		ac->addNode(NULL);
	}
	catch(NodeIsNullException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);	
}

void AnnotatedClusterTest::addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull()
{
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;

	try
	{
		ac->addNodesToCluster(NULL);
	}
	catch(AnnotatedClusterAbstractionIsNullException e)
	{
		exceptionThrown = true;
	}

	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid()
{

	bool exceptionThrown = false;
	try
	{
		AnnotatedCluster(0,0,0,1);
	}
	catch(InvalidClusterDimensionsException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid()
{

	bool exceptionThrown = false;

	try
	{
		AnnotatedCluster(0,0,1,0);
	}
	catch(InvalidClusterDimensionsException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterTest::constructorShouldThrowExceptionWhenXOriginParameterIsInvalid()
{
	bool exceptionThrown = false;

	try
	{
		AnnotatedCluster(-1,0,1,1);
	}
	catch(InvalidClusterOriginCoordinatesException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}
void AnnotatedClusterTest::constructorShouldThrowExceptionWhenYOriginParameterIsInvalid()
{
	bool exceptionThrown = false;
	try
	{
		AnnotatedCluster(0,-1,1,1);
	}
	catch(InvalidClusterOriginCoordinatesException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}

void AnnotatedClusterTest::addEntranceShouldTruncateClearanceTo_MAXAGENTSIZE_IfLarger()
{
	createEntranceNodes();
	e1_clearance=200;
	ac->addEntrance(e1_n1, e1_n2, e1_capability, e1_clearance, aca_mock);
	edge* e = absg->getRandomEdge();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("clearance value of new entrance not truncated", MAXAGENTSIZE, e->getClearance(e->getCapability()) );
}

/* integration test; depends on other methods. TODO: add in a mock to isolate code */
void AnnotatedClusterTest::addEntranceShouldSetEdgeWeightToExactlyOne()
{
	createEntranceNodes();
	ac->addEntrance(e1_n1, e1_n2, e1_capability, e1_clearance, aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many edges in abstract graph", 1, absg->getNumEdges());
	edge* e = absg->getRandomEdge();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance weight incorrect", 1, (int)e->getWeight());
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle()
{
	std::string errmsg("failed to throw exception when 'from' node is a hard obstacle ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);	
	testHelper->checkaddEntranceThrowsCorrectException<EntranceNodeIsNotTraversable>(e2_n2hardobst, e2_n1, e2_capability, 1);
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle()
{
	std::string errmsg("failed to throw exception when 'to' node is a hard obstacle ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);		
	testHelper->checkaddEntranceThrowsCorrectException<EntranceNodeIsNotTraversable>(e2_n1, e2_n2hardobst, e2_capability, 1);
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfCapabilityClearanceOfFirstParameterNodeIsNotEqualToOrGreaterThanClearanceParameter()
{
	std::string errmsg("failed to throw exception when 'from' node has capability clearance < clearance parameter ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);		
		
	e1_n1->setClearance(e1_capability, e1_clearance-1);
	testHelper->checkaddEntranceThrowsCorrectException<EntranceNodeIsNotTraversable>(e1_n1, e1_n2, e1_capability, e1_clearance);
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfCapabilityClearanceOfSecondParameterNodeIsNotEqualToOrGreaterThanClearanceParameter()
{
	std::string errmsg("failed to throw exception when 'to' node has capability clearance < clearance parameter ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);		
		
	e1_n1->setClearance(e1_capability, e1_clearance-1);
	testHelper->checkaddEntranceThrowsCorrectException<EntranceNodeIsNotTraversable>(e1_n2, e1_n1, e1_capability, e1_clearance);
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfClearanceParameterLessThanOrEqualToZero()
{
	std::string errmsg("failed to throw exception when clearance parameter zero ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);			
	testHelper->checkaddEntranceThrowsCorrectException<InvalidClearanceParameterException>(e1_n1, e1_n2, e1_capability, 0);

	errmsg.assign("failed to throw exception when clearance parameter < 0 ");
	testHelper->setFailMessage(errmsg);
	testHelper->checkaddEntranceThrowsCorrectException<InvalidClearanceParameterException>(e1_n1, e1_n2, e1_capability, -1);
}

void AnnotatedClusterTest::addEntranceShouldThrowExceptionIfCapabilityParameterIsInvalid()
{
	createEntranceNodes();
	std::string errmsg("failed to throw exception when capability parameter is not a valid capability ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);			
	testHelper->checkaddEntranceThrowsCorrectException<EntranceNodeIsNotTraversable>(e1_n1, e1_n2, kWater, e1_clearance);
}

/* builds two entrances along a veritcal border between two clusters. border area contains two contiguous areas separated by a hard obstacle */
void AnnotatedClusterTest::buildVerticalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheVerticalBorderBetweenTwoClusters()
{
	aca_mock->buildClusters();
	TestEntrance te1(4,1,5,1,kGround,2,0,1);
	TestEntrance te2(4,4,5,4,kGround,2,0,1);

	ac->buildVerticalEntrances(te1.getCapability(), aca_mock);

	/* check that the entrances were created in the expected locations */
	AnnotatedCluster *adjacentCluster = aca_mock->getCluster(aca_mock->getNodeFromMap(te2.getToX(), te2.getToY())->getParentCluster());
	node* te1_endpoint1 = ac->getParents().at(0);
	node* te1_endpoint2 = adjacentCluster->getParents().at(0);
	node* te2_endpoint1 = ac->getParents().at(1);
	node* te2_endpoint2 = adjacentCluster->getParents().at(1);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint1 incorrect", te1.getFromX(), (int)te1_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint1 incorrect", te1.getFromY(), (int)te1_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint2 incorrect", te1.getToX(), (int)te1_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint2 incorrect", te1.getToY(), (int)te1_endpoint2->getLabelL(kFirstData+1));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te2_endpoint1 incorrect", te2.getFromX(), (int)te2_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te2_endpoint1 incorrect", te2.getFromY(), (int)te2_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te2_endpoint2 incorrect", te2.getToX(), (int)te2_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te2_endpoint2 incorrect", te2.getToY(), (int)te2_endpoint2->getLabelL(kFirstData+1));
	
	/* check that the entrances have expected capability clearance annotations */
	edge* entrance = absg->findEdge(te1_endpoint2->getNum(), te1_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance1 clearance incorrect", entrance->getClearance(te1.getCapability()), te1.getClearance(te1.getCapability()));
	entrance = absg->findEdge(te2_endpoint2->getNum(), te2_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance2 clearance incorrect", entrance->getClearance(te2.getCapability()), te2.getClearance(te2.getCapability()));
}

void AnnotatedClusterTest::buildVerticalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter()
{
	std::string errmsg("failed to throw exception when ACA parameter is null");
	setupExceptionThrownTestHelper(absg, ac, NULL, errmsg);			

	int capability = kWater;
	testHelper->checkBuildVerticalEntrancesThrowsCorrectException<AnnotatedClusterAbstractionIsNullException>(capability);
}

void AnnotatedClusterTest::buildVerticalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter()
{
	int numNodesInAbstractGraphBefore = absg->getNumNodes();
	int numEdgesInAbstractGraphBefore = absg->getNumEdges();
	int numNodesInClusterBefore = ac->getParents().size();
	AnnotatedCluster* adjacentCluster = aca_mock->getCluster(1);
	int numNodesInClusterAdjacentBefore = adjacentCluster->getParents().size();
	
	int capability = kWater;
	ac->buildVerticalEntrances(capability, aca_mock);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in abstract graph", numNodesInAbstractGraphBefore, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many edges in abstract graph", numEdgesInAbstractGraphBefore, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in target cluster", numNodesInClusterBefore, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in adjacent cluster", numNodesInClusterBefore, (int)adjacentCluster->getParents().size());

}

/* builds two entrances along a veritcal border between two clusters. border area contains two contiguous areas separated by a hard obstacle */
void AnnotatedClusterTest::buildHorizontalEntrancesShouldCreateOneMaximallySizedEntrancePerContiguousAreaAlongTheHorizontalBorderBetweenTwoClusters()
{
	aca_mock->buildClusters();
	TestEntrance te1(0,4,0,5,kGround,1,0,1);
	TestEntrance te2(4,4,4,5,kGround,1,0,1);

	ac->buildHorizontalEntrances(te1.getCapability(), aca_mock);

	/* check that the entrances were created in the expected locations */
	AnnotatedCluster *adjacentCluster = aca_mock->getCluster(aca_mock->getNodeFromMap(te2.getToX(), te2.getToY())->getParentCluster());
	node* te1_endpoint1 = ac->getParents().at(0);
	node* te1_endpoint2 = adjacentCluster->getParents().at(0);
	node* te2_endpoint1 = ac->getParents().at(1);
	node* te2_endpoint2 = adjacentCluster->getParents().at(1);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint1 incorrect", te1.getFromX(), (int)te1_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint1 incorrect", te1.getFromY(), (int)te1_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint2 incorrect", te1.getToX(), (int)te1_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint2 incorrect", te1.getToY(), (int)te1_endpoint2->getLabelL(kFirstData+1));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te2_endpoint1 incorrect", te2.getFromX(), (int)te2_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te2_endpoint1 incorrect", te2.getFromY(), (int)te2_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te2_endpoint2 incorrect", te2.getToX(), (int)te2_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te2_endpoint2 incorrect", te2.getToY(), (int)te2_endpoint2->getLabelL(kFirstData+1));
	
	/* check that the entrances have expected capability clearance annotations */
	edge* entrance = absg->findEdge(te1_endpoint2->getNum(), te1_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance1 clearance incorrect", te1.getClearance(te1.getCapability()), entrance->getClearance(te1.getCapability()));
	entrance = absg->findEdge(te2_endpoint2->getNum(), te2_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance2 clearance incorrect", te1.getClearance(te1.getCapability()), entrance->getClearance(te2.getCapability()));
}

// integration test
void AnnotatedClusterTest::buildHorizontalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea()
{
	int testClusterSize=6;
	std::string testmap(HOGHOME);
	testmap.append("tests/testmaps/vdiamonds.map");
	AnnotatedClusterAbstraction aca(new Map(testmap.c_str()), new AnnotatedAStar(), testClusterSize);
	AnnotatedClusterFactory acf;
	aca.buildClusters(&acf);

	graph* absg = aca.getAbstractGraph(1);	
	int numNodesBefore=absg->getNumNodes();
	int numEdgesBefore=absg->getNumEdges();

	AnnotatedCluster* targetcluster = aca.getCluster(0);
	targetcluster->buildHorizontalEntrances(kGround, &aca);
		
	int numNodesActual=absg->getNumNodes();
	int numEdgesActual=absg->getNumEdges();
	int numNodesExpected=4;
	int numEdgesExpected=5; // 2 local maxima + 3 edges connecting the nodes inside each cluster
	
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
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("not enough nodes in abstract graph", numNodesExpected, numNodesActual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("not enough edges in abstract graph", numEdgesExpected, numEdgesActual);
	
	node* endpoint1 = absg->getNode(aca.getNodeFromMap(0,5)->getLabelL(kParent)); // local maxima for kGround
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second transition; endpoint1 is null", true, endpoint1 != 0);
	node* endpoint2 = absg->getNode(aca.getNodeFromMap(0,6)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second transition; endpoint2 is null", true, endpoint2 != 0);
	edge* myedge = endpoint1->findAnnotatedEdge(endpoint2,kGround,2, 1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("can't find edge for second transition", true, myedge != 0);

	endpoint1 = absg->getNode(aca.getNodeFromMap(3,5)->getLabelL(kParent)); // local maxima for kGround
	CPPUNIT_ASSERT_EQUAL_MESSAGE("third transition; endpoint1 is null", true, endpoint2 != 0);
	endpoint2 = absg->getNode(aca.getNodeFromMap(3,6)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("third transition; endpoint2 is null", true, endpoint2 != 0);
	myedge = endpoint1->findAnnotatedEdge(endpoint2,kGround,2, 1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("can't find edge for third transition", true, myedge != 0);
}

/* todo: fix with mock ACA otherwise this test will be hard to maintain */
void AnnotatedClusterTest::buildHorizontalEntrancesShouldCreateTwoTransitionsAtEachEndOfTheEntranceAreaIfAllNodesAlongEachClusterBorderHaveIdenticalClearance()
{
	std::string testmap(HOGHOME);
	testmap.append("tests/testmaps/clustertest.map");
	Map* m = new Map(testmap.c_str());

	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(m, new AnnotatedAStar(), cwidth);
	AnnotatedClusterFactory acf;
	aca->buildClusters(&acf);

	int capability = kGround;
	int expectedTransitionClearance=1;
	AnnotatedCluster* target = aca->getCluster(2);
	target->buildHorizontalEntrances(kGround, aca);
	
	int xposTransition1Cluster1 = 0; int yposTransition1Cluster1 = 4;
	int xposTransition1Cluster2 = 0; int yposTransition1Cluster2 = 5;
	int xposTransition2Cluster1 = 4; int yposTransition2Cluster1 = 4;
	int xposTransition2Cluster2 = 4; int yposTransition2Cluster2 = 5;
	
	graph* _absg = aca->getAbstractGraph(1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find expected number of abs nodes for horizontal entrances", 4, _absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find expected number of abs edges horizontal entrances", 4, _absg->getNumEdges());
	
	/* check that the entrances were created in the expected locations */
	AnnotatedCluster *adjacentCluster = aca->getCluster(3);
	node* te1_endpoint1 = ac->getParents().at(0);
	node* te1_endpoint2 = adjacentCluster->getParents().at(0);
	node* te2_endpoint1 = ac->getParents().at(1);
	node* te2_endpoint2 = adjacentCluster->getParents().at(1);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint1 incorrect", xposTransition1Cluster1, (int)te1_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint1 incorrect", yposTransition1Cluster1, (int)te1_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint2 incorrect", xposTransition1Cluster2, (int)te1_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint2 incorrect", yposTransition1Cluster2, (int)te1_endpoint2->getLabelL(kFirstData+1));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint1 incorrect", xposTransition2Cluster1, (int)te1_endpoint1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint1 incorrect", yposTransition2Cluster1, (int)te1_endpoint1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("xpos of te1_endpoint2 incorrect", xposTransition2Cluster2, (int)te1_endpoint2->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("ypos of te1_endpoint2 incorrect", yposTransition2Cluster2, (int)te1_endpoint2->getLabelL(kFirstData+1));
	
	/* check that the entrances have expected capability clearance annotations */
	edge* entrance = _absg->findEdge(te1_endpoint2->getNum(), te1_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance1 clearance incorrect", expectedTransitionClearance, entrance->getClearance(capability));

	entrance = absg->findEdge(te2_endpoint2->getNum(), te2_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance2 clearance incorrect", expectedTransitionClearance, entrance->getClearance(capability));	
	
	delete aca;
}

void AnnotatedClusterTest::buildHorizontalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongSouthernBorder()
{
	int clusterxorigin = 0;
	int clusteryorigin = 5;
	int capability = kGround; 
	int clearance = 1;
	int expectedNumClusterNodes = 0;
	int expectedNumAbstractGraphNodes = 0;
	int expectedNumAbstractGraphEdges = 0;

	/* cluster position not correct for this test; lets reposition it so it has no southern neighbour */
	ac->setHOrig(clusterxorigin);
	ac->setHOrig(clusteryorigin);
	ac->setWidth(4);
	ac->setHeight(1);
	ac->buildHorizontalEntrances(capability, aca_mock);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to cluster without eastern neighbour",expectedNumClusterNodes, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph when none required",expectedNumAbstractGraphNodes, absg->getNumNodes());  
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added edges to abstract graph when none required",expectedNumAbstractGraphEdges, absg->getNumEdges());	
}

void AnnotatedClusterTest::buildVerticalEntrancesShouldSkipClustersWhichHaveNoNeighboursAlongEasternBorder()
{
	int clusterxorigin = 5;
	int clusteryorigin = 5;
	int capability = kGround; 
	int clearance = 1;
	int expectedNumClusterNodes = 0;
	int expectedNumAbstractGraphNodes = 0;
	int expectedNumAbstractGraphEdges = 0;

	/* initial x/y/width/height not correct for this test; lets reposition the cluster so it has no neighbour along east border */
	aca_mock->buildClusters();
	ac->setHOrig(clusterxorigin);
	ac->setHOrig(clusteryorigin);
	ac->setWidth(4);
	ac->setHeight(1);
	
	ac->buildVerticalEntrances(capability, aca_mock);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to cluster without southern neighbour",expectedNumClusterNodes, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph when none required",expectedNumAbstractGraphNodes, absg->getNumNodes());  
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added edges to abstract graph when none required",expectedNumAbstractGraphEdges, absg->getNumEdges());	
}

void AnnotatedClusterTest::buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter()
{
	std::string errmsg("failed to throw exception when ACA parameter is null");
	setupExceptionThrownTestHelper(absg, ac, NULL, errmsg);

	int capability = kWater;
	testHelper->checkBuildHorizontalEntrancesThrowsCorrectException<AnnotatedClusterAbstractionIsNullException>(capability);
}

void AnnotatedClusterTest::buildHorizontalEntrancesShouldNotAddAnyEntrancesGivenAnInvalidCapabilityParameter()
{
	int numNodesInAbstractGraphBefore = absg->getNumNodes();
	int numEdgesInAbstractGraphBefore = absg->getNumEdges();
	int numNodesInClusterBefore = ac->getParents().size();
	AnnotatedCluster* adjacentCluster = aca_mock->getCluster(1);
	int numNodesInClusterAdjacentBefore = adjacentCluster->getParents().size();
	
	int capability = kWater;
	ac->buildHorizontalEntrances(capability, aca_mock);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in abstract graph", numNodesInAbstractGraphBefore, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many edges in abstract graph", numEdgesInAbstractGraphBefore, absg->getNumEdges());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in target cluster", numNodesInClusterBefore, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("too many nodes in adjacent cluster", numNodesInClusterBefore, (int)adjacentCluster->getParents().size());
}

/* NB: some endpoints shared and some edges re-used hence the expected node and edge count */
void AnnotatedClusterTest::builEntrancesShouldCreateCorrectNumberOfVerticalAndHorizontalTransitionsToOtherClusters()
{
	int expectedNumAbstractNodes = 9;
	int expectedNumAbstractEdges = 5;
	int expectedNumAbstractNodesInCluster = 4;

	aca_mock->buildClusters();
	ac->buildEntrances(aca_mock);
	

	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect edge count in abstract graph", expectedNumAbstractEdges, absg->getNumEdges());	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect node count in abstract graph", expectedNumAbstractNodes, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect node count in target cluster", expectedNumAbstractNodesInCluster, (int)ac->getParents().size());
}

void AnnotatedClusterTest::buildEntrancesShouldThrowExceptionGivenAnInvalidACAParameter()
{
	std::string errmsg("failed to throw exception when ACA parameter NULL ");
	setupExceptionThrownTestHelper(absg, ac, NULL, errmsg);	
	testHelper->checkBuildEntrancesThrowsCorrectException<AnnotatedClusterAbstractionIsNullException>();
}

void AnnotatedClusterTest::validateMapAbstractionThrowsExceptionGivenNullParameter()
{
	std::string errmsg("failed to throw exception when ACA parameter NULL ");
	setupExceptionThrownTestHelper(NULL, ac, NULL, errmsg);	
	testHelper->checkValidateMapAbstractionThrowsCorrectException<ValidateMapAbstractionException>();
}

void AnnotatedClusterTest::validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesHaveIdenticalCoordinates()
{
	std::string errmsg("failed to throw exception when parameter nodes have identical (x,y) coordinates ");
	setupExceptionThrownTestHelper(NULL, ac, NULL, errmsg);	
	
	node* e1_n1clone = dynamic_cast<node*>(e1_n1->clone());
	testHelper->checkValidateTransitionEndpointsThrowsCorrectException<ValidateTransitionEndpointsException>(e1_n1, e1_n1clone);
	
	delete e1_n1clone;
}

void AnnotatedClusterTest::validateTransitionEndpointsShouldThrowExceptionWhenParameterNodesAreNull()
{
	std::string errmsg("failed to throw exception when parameter nodes are not adjacent");
	setupExceptionThrownTestHelper(NULL, ac, NULL, errmsg);	
	testHelper->checkValidateTransitionEndpointsThrowsCorrectException<ValidateTransitionEndpointsException>(NULL, e1_n2);
	testHelper->checkValidateTransitionEndpointsThrowsCorrectException<ValidateTransitionEndpointsException>(e1_n1, NULL);
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph()
{
	createEntranceNodes();
	int expectedNodesInAbstractGraph=2;
	int expetedAbstractNodesInCluster=1;

	ac->addEndpointsToAbstractGraph(e1_n1, e1_n2, aca_mock);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract graph has incorrect node count",expectedNodesInAbstractGraph, absg->getNumNodes());
	
	node *absn1 = absg->getNode(e1_n1->getLabelL(kParent));
	node* absn2 = absg->getNode(e1_n2->getLabelL(kParent));
	CPPUNIT_ASSERT_MESSAGE("added first low-level node to abstract graph instead of a copy!", e1_n1 != absn1);
	CPPUNIT_ASSERT_MESSAGE("added second low-level node to abstract graph instead of a copy!",e1_n2 != absn2);	
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldSetAbstractNodesAsParentsOfNonAbstractNodes()
{
	createEntranceNodes();
	
	ac->addEndpointsToAbstractGraph(e1_n1, e1_n2, aca_mock);

	node *absn1 = absg->getNode(e1_n1->getLabelL(kParent));
	node* absn2 = absg->getNode(e1_n2->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent of n1 is null", true, absn1 != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent of n2 is null", true, absn2 != 0);

	CPPUNIT_ASSERT_MESSAGE("kParent label of n1 incorrect", e1_n1->getLabelL(kParent) == absn1->getNum());
	CPPUNIT_ASSERT_MESSAGE("kParent label of n2 incorrect", e1_n2->getLabelL(kParent) == absn2->getNum());
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldReuseExistingNodeEndpointsIfADifferentEntranceExistsAtSameLocation()
{
	createEntranceNodes();
	int numExpectedAbstractNodesInGraph = 2;
	int numExpectedAbstractNodeInCluster = 1;
	
	ac->addEndpointsToAbstractGraph(e3_n1,e3_n2,aca_mock); // add first
	ac->addEndpointsToAbstractGraph(e4_n1,e4_n2,aca_mock);	// add second; overlaps
	AnnotatedCluster* adjacentCluster = aca_mock->getCluster(1);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect abstract node count in abstract graph", numExpectedAbstractNodesInGraph, absg->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect abstract node count in target cluster ", numExpectedAbstractNodeInCluster, (int)ac->getParents().size());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect abstract node count in adjacent cluster ", numExpectedAbstractNodeInCluster, (int)adjacentCluster->getParents().size());
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldAddEachEntranceEndpointToItsCluster()
{
	createEntranceNodes();

	int expectedNumAbstractNodes = 1;
	ac->addEndpointsToAbstractGraph(e1_n1,e1_n2,aca_mock); 
	AnnotatedCluster *adjacentCluster = aca_mock->getCluster(e1_n2->getParentCluster());

	int numAbstractNodes = ac->getParents().size();
	int numAbstractNodesInNeighbour = adjacentCluster->getParents().size();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count in cluster incorrect", expectedNumAbstractNodes, numAbstractNodes );
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count in neighbouring cluster incorrect", expectedNumAbstractNodes, numAbstractNodesInNeighbour );

	/* make sure nodes added to cluster are also the same ones added to graph */
	node* endpoint1 = ac->getParents().at(0);
	node* endpoint2 = adjacentCluster->getParents().at(0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("endpoint1 node in cluster but not in graph", endpoint1, absg->getNode(endpoint1->getNum()));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("endpoint2 node in cluster but not in graph", endpoint2, absg->getNode(endpoint2->getNum()));
}

/* this is a similar test to the one for node::clone. We repeat the same checks here because the implementation of node insertion into
the abstract graph is opaque from the test's point-of-view. There's no way to tell if we used clone or something else, but the behaviour
is that a deep-copy of the original node is required -- albeit with an incremented kAbstractionLevel parameter */
void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes()
{
	createEntranceNodes();
	ac->addEndpointsToAbstractGraph(e1_n1,e1_n2,aca_mock);	
	
	/* check if annotations are ok */
	node* absn1 = absg->getNode(e1_n1->getLabelL(kParent));
	node* absn2 = absg->getNode(e1_n2->getLabelL(kParent));
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect kAbstractionLevel label",e1_n1->getLabelL(kAbstractionLevel)+1, absn1->getLabelL(kAbstractionLevel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect kAbstractionLevel label",e1_n2->getLabelL(kAbstractionLevel)+1, absn2->getLabelL(kAbstractionLevel));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect x-coordinate label",e1_n1->getLabelL(kFirstData), absn1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect x-coordinate label",e1_n2->getLabelL(kFirstData), absn2->getLabelL(kFirstData));	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect y-coordinate label",e1_n1->getLabelL(kFirstData+1), absn1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect y-coordinate label",e1_n2->getLabelL(kFirstData+1), absn2->getLabelL(kFirstData+1));
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent()
{
	std::string errmsg("failed to throw exception when 'from' & 'to' nodes are not adjacent ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);		

	node* nonAdjacentNeighbour = aca_mock->getNodeFromMap(6,1);
	testHelper->checkaddEndpointsToAbstractGraphThrowsCorrectException<EntranceNodesAreNotAdjacentException>(e2_n1, nonAdjacentNeighbour);
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster()
{
	std::string errmsg("failed to throw exception when 'from' & 'to' nodes share the same cluster (entrance connecting origin cluster to itself is invalid)");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);
	
	e1_n1->setParentCluster(0);
	e1_n2->setParentCluster(0);
	testHelper->checkaddEndpointsToAbstractGraphThrowsCorrectException<CannotBuildEntranceToSelfException>(e1_n1, e1_n2);
}

void AnnotatedClusterTest::addEndpointsToAbstractGraphShouldThrowExceptionIfParameterNodeHaveAnAbstractionLevelNotEqualToZero()
{
	std::string errmsg("failed to throw exception when parameter nodes have an abstraction level != 0 ");
	setupExceptionThrownTestHelper(absg, ac, (AnnotatedClusterAbstraction*)aca_mock, errmsg);

	e1_n1->setLabelL(kAbstractionLevel, 1);
	testHelper->checkaddEndpointsToAbstractGraphThrowsCorrectException<CannotBuildEntranceFromAbstractNodeException>(e1_n1, e1_n2);
	
	e1_n1->setLabelL(kAbstractionLevel, 0);
	e1_n2->setLabelL(kAbstractionLevel, 1);
	testHelper->checkaddEndpointsToAbstractGraphThrowsCorrectException<CannotBuildEntranceFromAbstractNodeException>(e1_n1, e1_n2);
}

void AnnotatedClusterTest::addTransitionToAbstractGraphShouldThrowExceptionWhenWeightIsNotGreaterThanZero()
{
	std::string errmsg("failed to throw exception when proposed transition has weight <= 0 ");
	setupExceptionThrownTestHelper(absg, ac, aca_mock, errmsg);

	node* abs_n1 = new node("");
	node* abs_n2 = new node("");
	absg->addNode(abs_n1);
	absg->addNode(abs_n2);
	
	testHelper->checkaddTransitionToAbstractGraphThrowsCorrectException<InvalidTransitionWeightException>(abs_n1, abs_n2, e1_capability, e1_clearance, 0);
}

void AnnotatedClusterTest::addTransitionToAbstractGraphShouldConnectAbstractNodesWithANewAnnotatedEdge()
{
	int capability = kGround;
	int clearance = 2;
	node* abs_n1 = new node("");
	node* abs_n2 = new node("");
	absg->addNode(abs_n1);
	absg->addNode(abs_n2);
	
	ac->addTransitionToAbstractGraph(abs_n1, abs_n2, capability, clearance, 1.0, aca_mock); 
	edge* e = abs_n1->findAnnotatedEdge(abs_n2, capability, clearance, 1.0);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("could not find inter-edge connecting abstract nodes", true, e != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge capability incorrect", capability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge clearance incorrect", clearance, e->getClearance(capability));
}

void AnnotatedClusterTest::addTransitionToAbstractGraphShouldAddANewPathToTheAnnotatedClusterAbstractionCacheForEachNewlyCreatedEdge()
{
	int capability = kGround;
	int clearance = 2;	
	
	node* n1 = dynamic_cast<node*>(aca_mock->getNodeFromMap(4,1)->clone());
	n1->setLabelL(kAbstractionLevel,1);
	n1->setParentCluster(0);
	node* n2 = dynamic_cast<node*>(aca_mock->getNodeFromMap(5,1)->clone());
	n2->setLabelL(kAbstractionLevel,1);
	n2->setParentCluster(2);
	
	absg->addNode(n1);
	absg->addNode(n2);
			
	int numExpectedPathsInCache = 1;
	ac->addTransitionToAbstractGraph(n1, n2, capability, clearance, 1.0, aca_mock); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add a new path to the cache", numExpectedPathsInCache, aca_mock->addPathsToCacheCounter);

	edge* e = n1->findAnnotatedEdge(n2, capability, clearance, 1.0);	
	path* p = aca_mock->getPathFromCache(e);
	node* expectedNode1 = aca_mock->getNodeFromMap(n1->getLabelL(kFirstData), n1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_MESSAGE("cached path has wrong expected node, 1", expectedNode1 == p->n);

	p = p->next;
	node* expectedNode2 = aca_mock->getNodeFromMap(n2->getLabelL(kFirstData), n2->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_MESSAGE("cached path has wrong expected node, 2", expectedNode2 == p->n);
}

void AnnotatedClusterTest::addTransitionToAbstractGraphShouldReuseExistingEdgeIfOneExists()
{
	createEntranceNodes();
	int numExpectedAbstractEdgesInGraph = 1;
	node* abs_e3n1 = new node("");
	node* abs_e3n2 = new node("");
	absg->addNode(abs_e3n1);
	absg->addNode(abs_e3n2);
	int capability1 = kGround;
	int capability2 = (kGround|kTrees);
	int clearance = 2.0;
	
	ac->addTransitionToAbstractGraph(abs_e3n1,abs_e3n2, capability1, clearance, 1.0, aca_mock);	
	ac->addTransitionToAbstractGraph(abs_e3n1,abs_e3n2, capability2, clearance, 1.0, aca_mock); // edge capability is superset of previous edge added

	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect edge count in abstract graph (failed to reuse existing edges)", numExpectedAbstractEdgesInGraph, absg->getNumEdges());
}

void AnnotatedClusterTest::addParentShouldThrowExceptionIfParameterNodeIsAlreadyAssignedToAnotherCluster()
{
	createEntranceNodes();
	std::string errmsg("failed to throw exception when adding an abstract node already assigned to another cluster");
	this->setupExceptionThrownTestHelper(NULL, ac, aca_mock, errmsg);
	
	e1_n1->setParentCluster(2);
	testHelper->checkAddParentThrowsCorrectException<NodeIsAlreadyAssignedToClusterException>(e1_n1);
}

void AnnotatedClusterTest::addParentShouldAddParameterNodeToAbstaractNodesListInCluster()
{
	node* n = new node("");	
	n->setParentCluster(-1);
	
	int nodesbefore = ac->getParents().size();
	ac->addParent(n, aca_mock);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add parent node to cluster", nodesbefore+1, (int)ac->getParents().size());
}

void AnnotatedClusterTest::addParentShouldNotAddAnyNodesAlreadyMarkedAsBelongingToTargetCluster()
{
	node* n = new node("");	
	n->setParentCluster(0);
	
	int nodesbefore = ac->getParents().size();
	ac->addParent(n, aca_mock);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add parent node to cluster", nodesbefore, (int)ac->getParents().size());
}

/* NB: We define "eligibility" as any capability which supersets the capabilities of both endpoints */
void AnnotatedClusterTest::connectEntranceEndpointsShouldCalculateTheShortestPathBetweenEachPairOfParentNodesForEachEligibleCapabilityGivenAHighQualityAbstraction()
{
	createEntranceNodes();
	int capability1 = kGround;
	int capability2 = (kGround|kTrees);	
	
	aca_mock->buildClusters();

	node* existingendpoint = dynamic_cast<node*>(e1_n1->clone());
	existingendpoint->setParentCluster(e1_n1->getParentCluster());
	ac->getParents().push_back(existingendpoint);
	
	node* newendpoint = dynamic_cast<node*>(e3_n1->clone());
	newendpoint->setParentCluster(e3_n1->getParentCluster());
	
	absg->addNode(existingendpoint);
	absg->addNode(newendpoint);
		
	// TODO: build a better AAStarMock
	delete aca_mock->getSearchAlgorithm();
	aca_mock->setSearchAlgorithm(new AnnotatedAStar());
	
	int numEdges = absg->getNumEdges();
	ac->connectEntranceEndpoints(newendpoint, aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find all connections between new and existing endpoints ", numEdges+3, absg->getNumEdges());
}

/* depending on the quality parameter indicated by the AnnotatedClusterAbstraction, we may wish to build the richest possible abstraction
or a smaller abstraction which contains less intra-cluster paths. In the latter case, before we try to connect two cluster nodes using
a complex multi-terrain capability, we first search for an already existing traversable edge between the two nodes that has a suitable clearance 

This is similar to prefering large highways (long & windy, big clearance, single capability) to small roads and tracks. The latter may be shorter
but the highway is more convenient. In our case, conveniece means an abstraction with a lower memory footprint 

In this test, for clearance 1, we should reuse a path with kGround capability instead of  building a new, shorter kGround|Trees path.
*/

void AnnotatedClusterTest::connectEntranceEndpointsShouldCalculateFirstTheSetOfShortestPathsWithLargestClearanceGivenALowQualityAbstraction()
{
	createEntranceNodes();
	int capability1 = kGround;
	int capability2 = (kGround|kTrees);	
	
	aca_mock->setGraphQualityParameter(ACAUtil::kLowQualityAbstraction);
	aca_mock->buildClusters();

	node* existingendpoint = dynamic_cast<node*>(e1_n1->clone());
	existingendpoint->setParentCluster(e1_n1->getParentCluster());
	ac->getParents().push_back(existingendpoint);
	
	node* newendpoint = dynamic_cast<node*>(e3_n1->clone());
	newendpoint->setParentCluster(e3_n1->getParentCluster());
		
	absg->addNode(existingendpoint);
	absg->addNode(newendpoint);
		
	// TODO: build a better AAStarMock
	delete aca_mock->getSearchAlgorithm();
	aca_mock->setSearchAlgorithm(new AnnotatedAStar());
	
	int numEdges = absg->getNumEdges();
	ac->connectEntranceEndpoints(newendpoint, aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find all connections between new and existing endpoints in AC0 for low quality abstraction", numEdges+2, absg->getNumEdges());	
	
	AnnotatedCluster* ac2 = new AnnotatedCluster(ac->getHOrig()+TESTCLUSTERSIZE, ac->getVOrig(), cwidth, cheight);
	ac->setClusterId(2);

	aca_mock->setGraphQualityParameter(ACAUtil::kLowQualityAbstraction);
	
	existingendpoint = dynamic_cast<node*>(aca_mock->getNodeFromMap(5,1)->clone());
	existingendpoint->setLabelL(kAbstractionLevel, 1);
	existingendpoint->setParentCluster(aca_mock->getNodeFromMap(5,1)->getParentCluster());
	ac2->getParents().push_back(existingendpoint);
	
	newendpoint = dynamic_cast<node*>(aca_mock->getNodeFromMap(5,4)->clone());
	newendpoint->setLabelL(kAbstractionLevel, 1);
	newendpoint->setParentCluster(aca_mock->getNodeFromMap(5,4)->getParentCluster());
	
	absg->addNode(existingendpoint);
	absg->addNode(newendpoint);
	
	numEdges = absg->getNumEdges();
	ac2->connectEntranceEndpoints(newendpoint, aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find all connections between new and existing endpoints in AC2 for low quality abstraction", numEdges+1, absg->getNumEdges());	
	
	delete ac2;
}
	
void AnnotatedClusterTest::connectEntranceEndpointsShouldAddAPathToTheAnnotatedClusterAbstractionCacheEachTimeTwoEndpointsAreConnected()
{
	createEntranceNodes();
	int capability1 = kGround;
	int capability2 = (kGround|kTrees);	
	
	aca_mock->buildClusters();

	node* existingendpoint = dynamic_cast<node*>(e1_n1->clone());
	existingendpoint->setParentCluster(e1_n1->getParentCluster());
	ac->getParents().push_back(existingendpoint);
	
	node* newendpoint = dynamic_cast<node*>(e3_n1->clone());
	newendpoint->setParentCluster(e3_n1->getParentCluster());
	
	absg->addNode(existingendpoint);
	absg->addNode(newendpoint);
		
	// TODO: build a better AAStarMock
	delete aca_mock->getSearchAlgorithm();
	aca_mock->setSearchAlgorithm(new AnnotatedAStar());

/*	path* cachedpath = aca_mock->lastAdded;
	int expectedLength = 7;
	
	int actualLength = cachedpath->length();
	int actualDist = aca_mock->distance(cachedpath);	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cached path (kGround capability) is not the correct length", expectedLength, actualLength);
*/	

	int numEdges = absg->getNumEdges();
	ac->connectEntranceEndpoints(newendpoint, aca_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to add to cache correct number of paths for current cluster", numEdges+3, (int)aca_mock->addPathsToCacheCounter);
	
	
}

void AnnotatedClusterTest::addParentsShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints()
{
	createEntranceNodes();
	int capability1 = kGround;
	int capability2 = (kGround|kTrees);	
	
	node* from = dynamic_cast<node*>(e1_n1->clone());
	from->setParentCluster(e1_n1->getParentCluster());
	absg->addNode(from);
	ac->getParents().push_back(from);

	delete aca_mock->getSearchAlgorithm();
	aca_mock->setSearchAlgorithm(new AnnotatedAStar());
	aca_mock->buildClusters();
	
	node* to = dynamic_cast<node*>(e3_n1->clone());
	absg->addNode(to);
	int expectedEdges = 3;
	
	ac->addParent(to, aca_mock);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to build required # of connections between cluster endpoints", expectedEdges, absg->getNumEdges());
}

// integration test
void AnnotatedClusterTest::buildVerticalEntrancesShouldCreateOneTransitionForEachLocalMaximaOfAnEntranceArea()
{
	int testClusterSize=9;
	std::string testmap(HOGHOME);
	testmap.append("tests/testmaps/hdiamonds.map");
	AnnotatedClusterAbstraction aca(new Map(testmap.c_str()), new AnnotatedAStar(), testClusterSize);
	AnnotatedClusterFactory acf;
	aca.buildClusters(&acf);

	graph* absg = aca.getAbstractGraph(1);	
	int numNodesBefore=absg->getNumNodes();
	int numEdgesBefore=absg->getNumEdges();

	AnnotatedCluster* targetcluster = aca.getCluster(0);
	targetcluster->buildVerticalEntrances(kGround, &aca);
		
	int numNodesActual=absg->getNumNodes();
	int numEdgesActual=absg->getNumEdges();
	int numNodesExpected=4;
	int numEdgesExpected=5; // 2 local maxima + 3 edges connecting the nodes inside each cluster
	
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
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("not enough nodes in abstract graph", numNodesExpected, numNodesActual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("not enough edges in abstract graph", numEdgesExpected, numEdgesActual);
	
	node* endpoint1 = absg->getNode(aca.getNodeFromMap(8,1)->getLabelL(kParent)); // local maxima for kGround
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second transition; endpoint1 is null", true, endpoint1 != 0);
	node* endpoint2 = absg->getNode(aca.getNodeFromMap(9,1)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second transition; endpoint2 is null", true, endpoint2 != 0);
	edge* myedge = endpoint1->findAnnotatedEdge(endpoint2,kGround,2, 1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("can't find edge for second transition", true, myedge != 0);

	endpoint1 = absg->getNode(aca.getNodeFromMap(8,5)->getLabelL(kParent)); // local maxima for kGround
	CPPUNIT_ASSERT_EQUAL_MESSAGE("third transition; endpoint1 is null", true, endpoint2 != 0);
	endpoint2 = absg->getNode(aca.getNodeFromMap(9,5)->getLabelL(kParent));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("third transition; endpoint2 is null", true, endpoint2 != 0);
	myedge = endpoint1->findAnnotatedEdge(endpoint2,kGround,2, 1);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("can't find edge for third transition", true, myedge != 0);
}