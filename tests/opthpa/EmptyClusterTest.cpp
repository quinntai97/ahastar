#include "EmptyClusterTest.h"

#include "ClusterNode.h"
#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "EmptyClusterFactory.h"
#include "map.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EmptyClusterTest );

void EmptyClusterTest::setUp()
{
}

void EmptyClusterTest::tearDown()
{
}

void EmptyClusterTest::addNodesToClusterGrowsAMaximalSizeEmptyCluster()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
	EmptyCluster *cluster = new EmptyCluster(0, 0);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);

	int expectedHeight = 3;
	int expectedWidth = 5;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong height", expectedHeight,
			cluster->getHeight());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("cluster has wrong width", expectedWidth, 
			cluster->getWidth());
}

void EmptyClusterTest::addNodesToClusterSetsTheClusterIdOfEveryNodeInTheClusterArea()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(0, 0);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);

	int cheight = cluster->getHeight();
	int cwidth = cluster->getWidth();
	int cid = cluster->getId();

	for(int x=cluster->getHOrigin(); x<cluster->getHOrigin()+cwidth; x++)
	{
		for(int y=cluster->getVOrigin(); y<cluster->getVOrigin()+cheight; y++)
		{
			ClusterNode* cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE("null node?!", false, cur == 0);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("node has wrong clusterId", cid, 
					cur->getParentClusterId());
		}
	}
}

void EmptyClusterTest::addNodesToClusterFramesTheEmptyClusterWithAbstractNodes()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(0, 0);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);


	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = 12;
	int expectedNumAbstractEdges = 20;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());

	int x = cluster->getHOrigin();
	int y = cluster->getVOrigin();
	for( ; y<cluster->getVOrigin()+cluster->getHeight(); y++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along left border that isn't framed!", false, parent == 0);
	}

	y = cluster->getVOrigin()+cluster->getHeight()-1;
	for(x=cluster->getHOrigin(); x<cluster->getHOrigin()+cluster->getWidth(); x++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along bottom border that isn't framed!", false, parent == 0);
	}

	x = cluster->getHOrigin()+cluster->getWidth()-1;
	for(y=cluster->getVOrigin()+cluster->getHeight()-1; y>=cluster->getVOrigin(); y--)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along right border that isn't framed!", false, parent == 0);
	}

	y = cluster->getVOrigin();
	for(x=cluster->getHOrigin()+cluster->getWidth()-1; x>=cluster->getHOrigin(); x--)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along top border that isn't framed!", false, parent == 0);
	}
}
void EmptyClusterTest::addNodesToClusterFramesEmptyClustersWithHeightEqualTo1()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(0, 3);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);


	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = 3;
	int expectedNumAbstractEdges = 3;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());

	int x = cluster->getHOrigin();
	int y = cluster->getVOrigin();
	for( ; y<cluster->getVOrigin()+cluster->getHeight(); y++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along left border that isn't framed!", false, parent == 0);
	}

	y = cluster->getVOrigin()+cluster->getHeight()-1;
	for(x=cluster->getHOrigin(); x<cluster->getHOrigin()+cluster->getWidth(); x++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along bottom border that isn't framed!", false, parent == 0);
	}
}

void EmptyClusterTest::addNodesToClusterFramesEmptyClustersWithWidthEqualTo1()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(2, 5);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);


	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = 6;
	int expectedNumAbstractEdges = 6;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());

	int x = cluster->getHOrigin();
	int y = cluster->getVOrigin();
	for( ; y<cluster->getVOrigin()+cluster->getHeight(); y++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along left border that isn't framed!", false, parent == 0);
	}

	y = cluster->getVOrigin()+cluster->getHeight()-1;
	for(x=cluster->getHOrigin(); x<cluster->getHOrigin()+cluster->getWidth(); x++)
	{
		ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
		ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along bottom border that isn't framed!", false, parent == 0);
	}
}


void EmptyClusterTest::addNodesToClusterFramesEmptyClusterWhenOriginIsNotOnTheMapBorder()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(5,1);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = 4;
	int expectedNumAbstractEdges = 4;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());

	for(int y=cluster->getVOrigin() ; y<cluster->getVOrigin()+cluster->getHeight(); y++)
	{
		for(int x = cluster->getHOrigin(); x<cluster->getHOrigin()+cluster->getWidth(); x++)
		{
			ClusterNode *cur = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(x, y));
			ClusterNode *parent = dynamic_cast<ClusterNode*>(g->getNode(cur->getLabelL(kParent)));
			CPPUNIT_ASSERT_EQUAL_MESSAGE("found a node along left border that isn't framed!", false, parent == 0);
		}
	}

}

void EmptyClusterTest::addNodesToClusterAddsMacroEdgesBetweenNodesOnOppositeBorders()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster = new EmptyCluster(3,4);
	ecmap.addCluster(cluster);
	cluster->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = 10;
	int expectedNumAbstractEdges = 15;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());
}

void EmptyClusterTest::buildVerticalEntrancesCreatesAllPossibleTransitionPoints()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster1 = new EmptyCluster(0,0);
	EmptyCluster *cluster2 = new EmptyCluster(5,1);
	ecmap.addCluster(cluster1);
	ecmap.addCluster(cluster2);
	cluster1->addNodesToCluster(&ecmap);
	cluster2->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = g->getNumNodes(); // before
	int expectedNumAbstractEdges = g->getNumEdges(); // before

	cluster1->buildVerticalEntrances(&ecmap);

	expectedNumAbstractEdges += 2; // # of transition points added
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());
}

void EmptyClusterTest::buildHorizontalEntrancesCreatesAllPossibleTransitionPoints()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster1 = new EmptyCluster(2,5);
	EmptyCluster *cluster2 = new EmptyCluster(3,4);
	ecmap.addCluster(cluster1);
	ecmap.addCluster(cluster2);
	cluster1->addNodesToCluster(&ecmap);
	cluster2->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = g->getNumNodes(); // before
	int expectedNumAbstractEdges = g->getNumEdges(); // before

	cluster2->buildHorizontalEntrances(&ecmap);

	expectedNumAbstractEdges += 5; // # of transition points added
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());

}

void 
EmptyClusterTest::buildHorizontalEntrancesCreatesAllDiagonalTransitionPoints()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster1 = new EmptyCluster(1,2);
	EmptyCluster *cluster2 = new EmptyCluster(1,0);

	ecmap.addCluster(cluster1);
	ecmap.addCluster(cluster2);
	cluster1->addNodesToCluster(&ecmap);
	cluster2->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = g->getNumNodes(); // before
	int expectedNumAbstractEdges = g->getNumEdges(); // before

	cluster2->setAllowDiagonals(true);
	cluster2->buildHorizontalEntrances(&ecmap);

	expectedNumAbstractEdges += 10; // 4 straight transitions, 6 diagonal
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", 
			expectedNumAbstractNodes, g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", 
			expectedNumAbstractEdges, g->getNumEdges());
}

void EmptyClusterTest::buildVerticalEntrancesCreatesAllDiagonalTransitionPoints()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());

	EmptyCluster *cluster1 = new EmptyCluster(3,0);
	EmptyCluster *cluster2 = new EmptyCluster(0,0);
	ecmap.addCluster(cluster1);
	ecmap.addCluster(cluster2);
	cluster1->addNodesToCluster(&ecmap);
	cluster2->addNodesToCluster(&ecmap);

	int abstractionLevel = 1;
	graph *g = ecmap.getAbstractGraph(abstractionLevel);
	int expectedNumAbstractNodes = g->getNumNodes(); // before
	int expectedNumAbstractEdges = g->getNumEdges(); // before

	cluster2->setAllowDiagonals(true);
	cluster2->buildVerticalEntrances(&ecmap);

	expectedNumAbstractEdges += 7; // 3 straight transitions and 4 diagonal
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract node count is wrong", expectedNumAbstractNodes, 
			g->getNumNodes());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("abstract edge count is wrong", expectedNumAbstractEdges, 
			g->getNumEdges());
}
