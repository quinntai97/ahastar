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
	
	testHelper=0; // to be safe
	aca_mock = new AnnotatedClusterAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock(), cwidth);

	/* setup the mock ACA object. NB/TODO: need to replace neighbours with mock AnnotatedCluster objects?  */
	aca_mock->buildClustersMocker.expects(once());
	aca_mock->getClusterMocker.stubs().with(eq(0)).will(new ReturnStub<AnnotatedCluster*>(ac));
	aca_mock->getClusterMocker.stubs().with(eq(1)).will(new ReturnStub<AnnotatedCluster*>(new AnnotatedCluster(startx+cwidth, starty, cwidth, cheight)));
	aca_mock->getClusterMocker.stubs().with(eq(2)).will(new ReturnStub<AnnotatedCluster*>(new AnnotatedCluster(startx, starty+cwidth, cwidth, cheight)));
		
	AnnotatedMapAbstractionMock::loadClearanceInfo(string(aca_mock->getMap()->getMapName()), aca_mock);
	absg = aca_mock->getAbstractGraph(1);	

}

void AnnotatedClusterTest::createEntranceNodes()
{
	/* entrance 1 */
	e1_n1 = aca_mock->getNodeFromMap(4,1);
	e1_n1->setParentCluster(0);
	e1_n2 = aca_mock->getNodeFromMap(5,1);
	e1_n2->setParentCluster(1);

	/* entrance 2 */
	e2_n1 = aca_mock->getNodeFromMap(4,0);
	e2_n1->setParentCluster(0);
	e2_n2hardobst = aca_mock->getNodeFromMap(5,0);
	e2_n2hardobst->setParentCluster(1);
	
}

void AnnotatedClusterTest::setupExceptionThrownTestHelper()
{
	createEntranceNodes();
	
	testHelper = new exceptionThrownHelper();
	testHelper->setAbstractGraph(absg);
	testHelper->setAnnotatedCluster(ac);
	testHelper->setAnnotatedClusterAbstraction(aca_mock);
//	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodeIsHardObstacleException>(e2_n1,e2_n2hardobst);
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


void AnnotatedClusterTest::addInterEdgeShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph()
{
	createEntranceNodes();
	int expectedNodesInAbstractGraph=2;
	int expetedAbstractNodesInCluster=1;

	// nb: deletion responsibility is taken over by the graph object to which we add these nodes
	ac->addInterEdge(e1_n1,e1_n2, aca_mock);
	node_iterator ni = absg->getNodeIter();
	node* absn1 = absg->nodeIterNext(ni);
	node* absn2 = absg->nodeIterNext(ni);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract graph has incorrect node count",expectedNodesInAbstractGraph, absg->getNumNodes());
	CPPUNIT_ASSERT_MESSAGE("added first low-level node to abstract graph instead of a copy!", e1_n1 != absn1);
	CPPUNIT_ASSERT_MESSAGE("added second low-level node to abstract graph instead of a copy!",e1_n2 != absn2);	
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfFirstNodeParameterNodeIsNull()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'from' node is null object ");
	testHelper->setFailMessage(errmsg);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodeIsNullException>(NULL, e1_n1);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfSecondNodeParameterNodeIsNull()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'to' node is null object ");
	testHelper->setFailMessage(errmsg);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodeIsNullException>(e1_n1, NULL);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfParameterNodesPointToSameObject()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'to & 'from' nodes are the same object ");
	testHelper->setFailMessage(errmsg);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodesAreIdenticalException>(e1_n1, e1_n1);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'from' node has abstraction level != 0 ");
	testHelper->setFailMessage(errmsg);
	
	e1_n1->setLabelL(kAbstractionLevel, 1);

	testHelper->checkaddInterEdgeThrowsCorrectException<CannotBuildEntranceFromAbstractNodeException>(e1_n1, e1_n2);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'to' node has abstraction level != 0 ");
	testHelper->setFailMessage(errmsg);
	
	e1_n2->setLabelL(kAbstractionLevel, 1);

	testHelper->checkaddInterEdgeThrowsCorrectException<CannotBuildEntranceFromAbstractNodeException>(e1_n1, e1_n2);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfParameterNodesShareTheSameCluster()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'from' & 'to' nodes share the same cluster (entrance connecting origin cluster to itself is invalid)");
	testHelper->setFailMessage(errmsg);

	e1_n1->setParentCluster(0);
	e1_n2->setParentCluster(0);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<CannotBuildEntranceToSelfException>(e1_n1, e1_n2);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'from' node is a hard obstacle ");
	testHelper->setFailMessage(errmsg);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodeIsHardObstacleException>(e2_n2hardobst, e2_n1);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'to' node is a hard obstacle ");
	testHelper->setFailMessage(errmsg);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodeIsHardObstacleException>(e2_n1, e2_n2hardobst);
}

void AnnotatedClusterTest::addInterEdgeShouldThrowExceptionIfParameterNodesAreNotAdjacent()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when 'from' & 'to' nodes are not adjacent ");
	testHelper->setFailMessage(errmsg);

	node* nonAdjacentNeighbour = aca_mock->getNodeFromMap(6,1);
	
	testHelper->checkaddInterEdgeThrowsCorrectException<EntranceNodesAreNotAdjacentException>(e2_n1, nonAdjacentNeighbour);
}


/* this is a similar test to the one for node::clone. We repeat the same checks here because the implementation of node insertion into
the abstract graph is opaque from the test's point-of-view. There's no way to tell if we used clone or something else, but the behaviour
is that a deep-copy of the original node is required -- albeit with an incremented kAbstractionLevel parameter */
void AnnotatedClusterTest::addInterEdgeShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes()
{
	createEntranceNodes();
	ac->addInterEdge(e1_n1,e1_n2, aca_mock);	
	
	/* check if annotations are ok */
	node_iterator ni = absg->getNodeIter();
	node* absn1 = absg->nodeIterNext(ni);
	node* absn2 = absg->nodeIterNext(ni);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect kAbstractionLevel label",e1_n1->getLabelL(kAbstractionLevel)+1, absn1->getLabelL(kAbstractionLevel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect kAbstractionLevel label",e1_n2->getLabelL(kAbstractionLevel)+1, absn2->getLabelL(kAbstractionLevel));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect x-coordinate label",e1_n1->getLabelL(kFirstData), absn1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect x-coordinate label",e1_n2->getLabelL(kFirstData), absn2->getLabelL(kFirstData));	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect y-coordinate label",e1_n1->getLabelL(kFirstData+1), absn1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect y-coordinate label",e1_n2->getLabelL(kFirstData+1), absn2->getLabelL(kFirstData+1));
}

void AnnotatedClusterTest::addInterEdgeShouldConnectAbstractNodesWithANewAnnotatedEdge()
{
	createEntranceNodes();

	int expectedEdgeCapability = (e1_n1->getTerrainType()|e1_n2->getTerrainType());
	int expectedClearance =e1_n1->getClearance(expectedEdgeCapability);
	expectedClearance = expectedClearance <e1_n2->getClearance(expectedEdgeCapability)?expectedClearance:e1_n2->getClearance(expectedEdgeCapability);	

	ac->addInterEdge(e1_n1,e1_n2, aca_mock); // target
	
	/* check results */
	node_iterator ni = absg->getNodeIter();
	node* absn1 = absg->nodeIterNext(ni);
	node* absn2 = absg->nodeIterNext(ni);
	edge* e = absg->findEdge(absn1->getNum(),absn2->getNum());
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("could not find inter-edge connecting abstract nodes", true, e != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge capability incorrect", expectedEdgeCapability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge clearance incorrect", expectedClearance, e->getClearance(expectedEdgeCapability));
}

void AnnotatedClusterTest::addInterEdgeShouldAddEachEntranceEndpointToItsCluster()
{
	createEntranceNodes();

	int expectedNumAbstractNodes = 1;
	ac->addInterEdge(e1_n1,e1_n2, aca_mock); 
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
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when ACA parameter is null");
	testHelper->setFailMessage(errmsg);

	int capability = kWater;
	testHelper->checkBuildVerticalEntrancesThrowsCorrectException<AnnotatedClusterAbstractionIsNullException>(capability, NULL);
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
	TestEntrance te2(3,4,3,5,kGround,1,0,1);

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
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance1 clearance incorrect", entrance->getClearance(te1.getCapability()), te1.getClearance(te1.getCapability()));
	entrance = absg->findEdge(te2_endpoint2->getNum(), te2_endpoint1->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("entrance2 clearance incorrect", entrance->getClearance(te2.getCapability()), te2.getClearance(te2.getCapability()));
}

void AnnotatedClusterTest::buildHorizontalEntrancesShouldThrowExceptionGivenAnInvalidACAParameter()
{
	setupExceptionThrownTestHelper();
	std::string errmsg("failed to throw exception when ACA parameter is null");
	testHelper->setFailMessage(errmsg);

	int capability = kWater;
	testHelper->checkBuildHorizontalEntrancesThrowsCorrectException<AnnotatedClusterAbstractionIsNullException>(capability, NULL);
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

