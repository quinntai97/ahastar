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
#include "AnnotatedMapAbstractionMock.h"
#include "AnnotatedAStarMock.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterTest );

void AnnotatedClusterTest::setUp()
{
	cwidth = 5;
	cheight = cwidth;
	ama_mock = new AnnotatedMapAbstractionMock(new Map(acmap.c_str()), new AnnotatedAStarMock());
	ama_mock->loadClearanceInfo(acmap);
}

void AnnotatedClusterTest::optionalSetUp()
{
	ama_mock->createAbstractGraph();	
	/* entrance 1 */
	e1_n1 = ama_mock->getNodeFromMap(4,1);
	e1_n1->setParentCluster(0);
	e1_n2 = ama_mock->getNodeFromMap(5,1);
	e1_n2->setParentCluster(1);

	/* entrance 2 */
	e2_n1 = ama_mock->getNodeFromMap(4,0);
	e2_n1->setParentCluster(0);
	e2_n2hardobst = ama_mock->getNodeFromMap(5,0);
	e2_n2hardobst->setParentCluster(1);

	absg = ama_mock->getAbstractGraph(1);
}

void AnnotatedClusterTest::tearDown()
{
	delete ama_mock;
	if(ac)
		delete ac;
}

void AnnotatedClusterTest::addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions()
{
	int startx=0; int starty=0;
	int expectedTotalNodes = cwidth*cheight;
	ac = new AnnotatedCluster(startx, starty, cwidth, cheight);
	
	ac->addNodesToCluster(ama_mock);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not match expectations", expectedTotalNodes, ac->getNumNodes());	
	
	delete ac;
	ac = 0;
}

void AnnotatedClusterTest::addNodeShouldIncrementByOneTotalNodesInCluster()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = 1;
	ac = new AnnotatedCluster(0, 0, cwidth, cheight);
	node* targetnode = ama_mock->getNodeFromMap(x,y);
	bool result = ac->addNode(ama_mock->getNodeFromMap(x,y));
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to return true when adding a traversable node to the cluster", true, result);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("total number of nodes in cluster does not meet expected total", expectedTotalNodes, ac->getNumNodes());
}

void AnnotatedClusterTest::addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId()
{
	int x = 0; int y = 0;
	int expectedTotalNodes = 1;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	node* targetnode = ama_mock->getNodeFromMap(x,y);
	ac->addNode(ama_mock->getNodeFromMap(x,y));
	
	CPPUNIT_ASSERT_EQUAL(ac->getClusterId(), targetnode->getParentCluster());
}

void AnnotatedClusterTest::addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster()
{

	int x = 0; int y = 0;
	int expectedTotalNodes = 0;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	
	node* toAssign = ama_mock->getNodeFromMap(1,1); // pick a node, any node and assign it to some cluster 
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
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);
	
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
		node* toAssign = ama_mock->getNodeFromMap(1,1); // pick a node, any node
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
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);

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
	int x = 0; int y = 0;
	int expectedTotalNodes = cwidth*cheight;
	bool exceptionThrown = false;
	ac = new AnnotatedCluster(x, y, cwidth, cheight);

	try
	{
		ac->addNodesToCluster(NULL);
	}
	catch(AnnotatedMapAbstractionIsNullException e)
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
		ac = new AnnotatedCluster(0,0,0,1);
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
		ac = new AnnotatedCluster(0,0,1,0);
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
		ac = new AnnotatedCluster(-1,0,1,1);
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
		ac = new AnnotatedCluster(0,-1,1,1);
	}
	catch(InvalidClusterOriginCoordinatesException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);

}


void AnnotatedClusterTest::addEntranceToGraphShouldAddTwoNewAbstractNodesToAbstractGraphGivenAPairOfNodesInTheNonAbstractGraph()
{
	optionalSetUp();
	int expectedNodesInAbstractGraph=2;

	// nb: deletion responsibility is taken over by the graph object to which we add these nodes
	//ama_mock->buildAbstractGraph();
	ac->addEntranceToGraph(absg,e1_n1,e1_n2);
	graph* g = ama_mock->getAbstractGraph(1);
	node_iterator ni = g->getNodeIter();
	node* absn1 = g->nodeIterNext(ni);
	node* absn2 = g->nodeIterNext(ni);
		
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract graph has incorrect node count",expectedNodesInAbstractGraph, ama_mock->getAbstractGraph(1)->getNumNodes());
	CPPUNIT_ASSERT_MESSAGE("added first low-level node to abstract graph instead of a copy!", e1_n1 != absn1);
	CPPUNIT_ASSERT_MESSAGE("added second low-level node to abstract graph instead of a copy!",e1_n2 != absn2);

	
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfFirstNodeParameterNodeIsNull()
{
	optionalSetUp();

	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();

	
	try
	{
		ac->addEntranceToGraph(absg, NULL,e1_n1);
	}
	catch(EntranceNodeIsNullException e)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfSecondNodeParameterNodeIsNull()
{
	optionalSetUp();

	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();

	
	try
	{
		ac->addEntranceToGraph(absg,e1_n1, NULL);
	}
	catch(EntranceNodeIsNullException e)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfParameterNodesPointToSameObject()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();

	try
	{
		ac->addEntranceToGraph(absg,e1_n1,e1_n1);
	}
	catch(EntranceNodesAreIdenticalException e)
	{
			exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();

	
	e1_n1->setLabelL(kAbstractionLevel, 1);
	e1_n2->setLabelL(kAbstractionLevel, 2);
	
	try 
	{
		ac->addEntranceToGraph(absg,e1_n1,e1_n2);
	}
	catch(CannotBuildEntranceFromAbstractNodeException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL(true, exceptionThrown);
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeHasAnAbstractionLevelNotEqualToZero()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();
	
	e1_n1->setLabelL(kAbstractionLevel, 0);
	e1_n2->setLabelL(kAbstractionLevel, 1);
	
	try 
	{
		ac->addEntranceToGraph(absg,e1_n1,e1_n2);
	}
	catch(CannotBuildEntranceFromAbstractNodeException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfParameterNodesShareTheSameCluster()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();
	
	e1_n1->setParentCluster(0);
	e1_n2->setParentCluster(0);

	try 
	{
		ac->addEntranceToGraph(absg,e1_n1,e1_n2);
	}
	catch(CannotBuildEntranceToSelfException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfFirstParameterNodeIsAHardObstacle()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();
			
	try 
	{
		ac->addEntranceToGraph(absg, e2_n2hardobst,e1_n1);
	}
	catch(EntranceNodeIsHardObstacleException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
	
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfSecondParameterNodeIsAHardObstacle()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();
	
	try 
	{
		ac->addEntranceToGraph(absg,e1_n1, e2_n2hardobst);
	}
	catch(EntranceNodeIsHardObstacleException e)
	{
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
	
}

/* this is a similar test to the one for node::clone. We repeat the same checks here because the implementation of node insertion into
the abstract graph is opaque from the test's point-of-view. There's no way to tell if we used clone or something else, but the behaviour
is that a deep-copy of the original node is required -- albeit with an incremented kAbstractionLevel parameter */
void AnnotatedClusterTest::addEntranceToGraphShouldCreateAbstractNodesWhichHaveTheSameAnnotationsAsParameterNodes()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	ac->addEntranceToGraph(absg,e1_n1,e1_n2);	
	graph *g = ama_mock->getAbstractGraph(1);
	
	
	/* check if annotations are ok */
	node_iterator ni = g->getNodeIter();
	node* absn1 = g->nodeIterNext(ni);
	node* absn2 = g->nodeIterNext(ni);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has different terrain type",e1_n1->getTerrainType(), absn1->getTerrainType());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has different terrain type",e1_n2->getTerrainType(), absn2->getTerrainType());
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has different parent cluster",e1_n1->getParentCluster(), absn1->getParentCluster());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has different parent cluster",e1_n2->getParentCluster(), absn2->getParentCluster());
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has different clearance value for kTrees capability",e1_n1->getClearance(kTrees), absn1->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has different clearance value for kTrees capability",e1_n2->getClearance(kTrees), absn2->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has different clearance value for kTrees capability",e1_n1->getClearance(kTrees), absn1->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has different clearance value for kTrees capability",e1_n2->getClearance(kTrees), absn2->getClearance(kTrees));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has different clearance value for (kGround|kTrees) capability",e1_n1->getClearance((kGround|kTrees)), absn1->getClearance((kGround|kTrees)));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has different clearance value for (kGround|kTrees) capability",e1_n2->getClearance((kGround|kTrees)), absn2->getClearance((kGround|kTrees)));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect kAbstractionLevel label",e1_n1->getLabelL(kAbstractionLevel)+1, absn1->getLabelL(kAbstractionLevel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect kAbstractionLevel label",e1_n2->getLabelL(kAbstractionLevel)+1, absn2->getLabelL(kAbstractionLevel));

	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect x-coordinate label",e1_n1->getLabelL(kFirstData), absn1->getLabelL(kFirstData));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect x-coordinate label",e1_n2->getLabelL(kFirstData), absn2->getLabelL(kFirstData));	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("first abstract node has incorrect y-coordinate label",e1_n1->getLabelL(kFirstData+1), absn1->getLabelL(kFirstData+1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("second abstract node has incorrect y-coordinate label",e1_n2->getLabelL(kFirstData+1), absn2->getLabelL(kFirstData+1));
}

void AnnotatedClusterTest::addEntranceToGraphShouldConnectAbstractNodesWithANewEdge()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();

	int expectedEdgeCapability = (e1_n1->getTerrainType()|e1_n2->getTerrainType());
	int expectedClearance =e1_n1->getClearance(expectedEdgeCapability);
	expectedClearance = expectedClearance <e1_n2->getClearance(expectedEdgeCapability)?expectedClearance:e1_n2->getClearance(expectedEdgeCapability);	

	ac->addEntranceToGraph(absg,e1_n1,e1_n2); // target
	
	/* check results */
	graph *g = ama_mock->getAbstractGraph(1);
	node_iterator ni = g->getNodeIter();
	node* absn1 = g->nodeIterNext(ni);
	node* absn2 = g->nodeIterNext(ni);
	edge* e = g->findEdge(absn1->getNum(),absn2->getNum());
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("could not find inter-edge connecting abstract nodes", true, e != 0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge capability incorrect", expectedEdgeCapability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge clearance incorrect", expectedClearance, e->getClearance(expectedEdgeCapability));
}

void AnnotatedClusterTest::addEntranceToGraphShouldThrowExceptionIfParameterNodesAreNotAdjacent()
{
	optionalSetUp();
	//ama_mock->buildAbstractGraph();
	bool exceptionThrown = false;
	int numnodes = ama_mock->getAbstractGraph(1)->getNumNodes();
	node* nonAdjacentNeighbour = ama_mock->getNodeFromMap(6,1);
		
	try 
	{
		ac->addEntranceToGraph(absg,e1_n1, nonAdjacentNeighbour);
	}
	catch(EntranceNodesAreNotAdjacentException e)
	{	
		exceptionThrown = true;
	}
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to throw exception when building entrance between non-adjacent nodes", true, exceptionThrown);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly added nodes to abstract graph", numnodes, ama_mock->getAbstractGraph(1)->getNumNodes());
	
}

void AnnotatedClusterTest::buildHorizontalEntrancesShouldIdentifyAsEntrancesPairsOfNodesWithMaximalClearanceThatConnectAdjacentClusters()
{
	//ama_mock->buildAbstractGraph();	
}

