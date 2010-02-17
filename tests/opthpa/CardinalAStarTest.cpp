#include "CardinalAStarTest.h"

#include "CardinalAStar.h"
#include "CardinalAStarFactory.h"
#include "ClusterNode.h"
#include "ClusterNodeFactory.h"
#include "EdgeFactory.h"
#include "EmptyClusterAbstraction.h"
#include "EmptyClusterFactory.h"
#include "graph.h"
#include "HPAStar2.h"
#include "map.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CardinalAStarTest );

void CardinalAStarTest::setUp()
{
}

void CardinalAStarTest::tearDown()
{
}

void CardinalAStarTest::evaluateReturnsFalseWhenTwoNodesAreConnectedByANonCardinalEdge()
{
	EmptyClusterAbstraction ecmap(new Map(emptymap.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
	
	ClusterNode* first = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,0));	
	ClusterNode* second = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,1));

	CardinalAStar astar;
	edge e(first->getNum(), second->getNum(), 1.42);
	bool result = astar.evaluate(first, second, &e);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate incorrectly returned true for non-cardinal edge", 
			false, result);
}

void CardinalAStarTest::evaluateReturnsTrueWhenTwoNodesAreConnectedByACardinalEdge()
{
	EmptyClusterAbstraction ecmap(new Map(emptymap.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
	
	ClusterNode* first = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,0));	
	ClusterNode* second = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,1));

	CardinalAStar astar;
	edge e(first->getNum(), second->getNum(), 1);
	bool result = astar.evaluate(first, second, &e);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("evaluate incorrectly returned true for non-cardinal edge",
		   	true, result);
}

void CardinalAStarTest::getPathReturnsAnOptimalLengthSolution()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
	
	ClusterNode* start = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,0));	
	ClusterNode* goal = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(3,4));

	double expectedLength = 9;

	CardinalAStar astar;
	path* p = astar.getPath(&ecmap, start, goal);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("returned a path of non-optimal length", 
			expectedLength, ecmap.distance(p));
	delete p;
}

void CardinalAStarTest::ItsGoTime()
{
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new ClusterNodeFactory(), new EdgeFactory());
	ecmap.buildClusters();
	ecmap.buildEntrances();
	
	ClusterNode* start = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(2,1));	
	ClusterNode* goal = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(3,4));

	HPAStar2 hpa(new CardinalAStarFactory());
	path* p = hpa.getPath(&ecmap, start, goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("no path found :(", false, p == 0);
	p->print(true);
	delete p;
	std::cout << std::endl;

	start = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(0,0));	
	goal = dynamic_cast<ClusterNode*>(ecmap.getNodeFromMap(4,2));
	p = hpa.getPath(&ecmap, start, goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("no inter-cluster path found :(", false, p == 0);
	p->print(true);
	delete p;

}
