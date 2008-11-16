/*
 *  HPAClusterTest.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterTest.h"

#include "HPAClusterAbstractionMock.h"
#include "ClusterAStarMock.h"
#include "TestConstants.h"

#include "HPAClusterAbstraction.h"
#include "ClusterAStarFactory.h"
#include "HPAClusterFactory.h"
#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "ClusterNode.h"

#include "reservationProvider.h"
#include "graph.h"
#include "constants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HPAClusterTest );

void HPAClusterTest::setUp()
{

}

void HPAClusterTest::tearDown()
{

}

void HPAClusterTest::addParentThrowsExceptionGivenANULLNodeParameter()
{
	HPACluster cluster(0,0,5,5);
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	cluster.addParent(NULL, &hpamap);
}

void HPAClusterTest::addParentThrowsExceptionGivenANULL_HPAClusterAbstraction_Parameter()
{
	HPACluster cluster(0,0,5,5);
	node n("test");
	cluster.addParent(&n, NULL);
}


void HPAClusterTest::addParentThrowsExceptionGivenANodeParameterThatIsNotOfType_ClusterNode()
{
	HPACluster cluster(0,0,5,5);

	node n("test");
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	cluster.addParent(&n, &hpamap);
}

void HPAClusterTest::addParentThrowsExceptionGivenANodeParameterThatIsAlreadyAssignedToAnotherCluster()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode n("test");
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	n.setParentClusterId(0);
	cluster.setClusterId(1);
	cluster.addParent(&n, &hpamap);
}

void HPAClusterTest::addParentThrowsExceptionGivenANodeParameterThatHasNotBeenAddedToTheAbstractGraph()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode n("test");
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	int expectedClusterId = 0;
	cluster.setClusterId(expectedClusterId);
	cluster.addParent(&n, &hpamap);
}

void HPAClusterTest::addParentUpdatesParentClusterIdOfNodeParameterToIdOfCluster()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode *n = new ClusterNode("test");
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	hpamap.getAbstractGraph(1)->addNode(n);

	int expectedClusterId = 0;
	cluster.setClusterId(expectedClusterId);
	cluster.addParent(n, &hpamap);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("parentClusterId attribute of parent node not set", expectedClusterId, n->getParentClusterId()); 
}

void HPAClusterTest::addParentAddsNodeToParentsCollection()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode *n = new ClusterNode("test");
	HPAClusterAbstraction hpamap(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	hpamap.getAbstractGraph(1)->addNode(n);

	int expectedClusterId = 0;
	int numExpectedParentNodes=1;
	cluster.setClusterId(expectedClusterId);
	cluster.addParent(n, &hpamap);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("not not added to parents collection", numExpectedParentNodes, (int)cluster.parents.size()); 
}

void HPAClusterTest::addParentShouldCreateEdgesToRepresentAllValidPathsBetweenNewNodeAndExistingClusterEndpoints()
{
	HPAClusterAbstraction *hpamap = new HPAClusterAbstraction(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	int clusterId = 0;
	HPACluster *cluster = new HPACluster(0,0,5,5);
	cluster->setClusterId(clusterId);
	
	/* add all low-level nodes to the cluster */
	for(int i=0; i<5; i++)
		for(int j=0; j<5; j++)
		{
			node* n = hpamap->getNodeFromMap(i,j);
			cluster->nodes[n->getUniqueID()] = n; 
		}

	/* create paent nodes  & add to abstract graph */
	ClusterNode *other = new ClusterNode("other");
	other->setLabelL(kFirstData, 1);
	other->setLabelL(kFirstData+1, 0);
	cluster->parents[other->getUniqueID()] = other;

	ClusterNode *n = new ClusterNode("test");
	n->setLabelL(kFirstData, 0);
	n->setLabelL(kFirstData+1, 0);
	
	graph* g =  hpamap->getAbstractGraph(1);
	g->addNode(n);
	g->addNode(other);

	/* add "n" to the cluster */
	cluster->addParent(n, hpamap);
	int numExpectedGraphEdges=1;
	int numExpectedGraphNodes=2;
	int numExpectedPathsInCache=1;
			
	CPPUNIT_ASSERT_EQUAL_MESSAGE("edge count wrong", numExpectedGraphEdges, g->getNumEdges()); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node count wrong", numExpectedGraphNodes, g->getNumNodes()); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("pathCache count wrong", numExpectedPathsInCache, hpamap->getPathCacheSize());

	edge* e = g->getRandomEdge();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("could not match edge fromId to known node", true, e->getFrom() == n->getNum() || e->getFrom() == other->getNum());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("could not match edge toId to known node", true, e->getTo() == n->getNum() || e->getTo() == other->getNum());
	
	path* p = hpamap->getPathFromCache(e);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong path added to cache", true, p != NULL);

	delete cluster;
	delete hpamap;
}

void HPAClusterTest::addNodeShouldThrowExceptionWhenParameterNodeIsAssignedToAnotherCluster()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode n("test");
	n.setParentClusterId(0);
	cluster.setClusterId(1);
	cluster.addNode(&n);
}

void HPAClusterTest::addNodeShouldIncrementByOneTotalNodesInCluster()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode n("test");
	n.setLabelL(kFirstData, 0);
	n.setLabelL(kFirstData+1, 0);

	int numExpectedNodesInCluster = 1;
	cluster.addNode(&n);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong node count", numExpectedNodesInCluster, (int)cluster.nodes.size());
}

void HPAClusterTest::addNodeShouldThrowExceptionWhenParameterNodeHasCoordinatesOutsideClusterBoundary()
{
	HPACluster cluster(0,0,5,5);

	ClusterNode n("test");
	n.setLabelL(kFirstData, 6);
	n.setLabelL(kFirstData+1, 6);

	int numExpectedNodesInCluster = 1;
	cluster.addNode(&n);
}

void HPAClusterTest::addNodeShouldThrowExceptionWhenNodeParameterIsNull()
{
	HPACluster cluster(0,0,5,5);
	cluster.addNode(NULL);
}

void HPAClusterTest::addNodeShouldSetTheParameterNodeParentClusterIdEqualToTheCurrentClusterId()
{
	int clusterId = 1;
	HPACluster cluster(0,0,5,5);
	cluster.setClusterId(clusterId);

	ClusterNode n("test");
	n.setLabelL(kFirstData, 2);
	n.setLabelL(kFirstData+1, 2);

	cluster.addNode(&n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node has wrong parentClusterId attribute", clusterId, n.getParentClusterId());
}

void HPAClusterTest::addNodesToClusterShouldAssignAllNodesInAreaMarkedByHeightAndWidthDimensions()
{
	HPAClusterAbstraction *hpamap = new HPAClusterAbstraction(new Map(acmap.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);

	int clusterId = 0;
	HPACluster cluster(0,0,5,5);
	cluster.setClusterId(clusterId);
	
	cluster.addNodesToCluster(hpamap);
	int numExpectedNodesInCluster = 25;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong number of nodes added to cluster", numExpectedNodesInCluster, (int)cluster.nodes.size());
	
	delete hpamap;
}

void HPAClusterTest::addNodesToClusterShouldThrowExceptionWhenMapAbstractionParameterIsNull()
{
	int clusterId = 0;
	HPACluster *cluster = new HPACluster(0,0,5,5);
	cluster->setClusterId(clusterId);
	
	cluster->addNodesToCluster(NULL);
}

void HPAClusterTest::constructorShouldThrowExceptionWhenWidthDimensionParameterIsInvalid()
{
	int width = 0;
	int height = 5;
	
	HPACluster(0,0, width, height);
}

void HPAClusterTest::constructorShouldThrowExceptionWhenHeightDimensionParameterIsInvalid()
{
	int width = 5;
	int height = 0;
	
	HPACluster(0,0, width, height);
}

void HPAClusterTest::constructorShouldThrowExceptionWhenXOriginParameterIsInvalid()
{
	int width = 5;
	int height = 5;
	
	HPACluster(-1,0, width, height);
}

void HPAClusterTest::constructorShouldThrowExceptionWhenYOriginParameterIsInvalid()
{
	int width = 5;
	int height = 5;
	
	HPACluster(0,-1, width, height);
}

void HPAClusterTest::constructorShouldThrowExceptionWhenAlgorithmParameterIsNULL()
{
	int width = 5;
	int height = 5;
	
	HPACluster(0,0, width, height, NULL);	
}