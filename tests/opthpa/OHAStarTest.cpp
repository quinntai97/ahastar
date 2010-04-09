#include "OHAStarTest.h"

#include "EdgeFactory.h"
#include "EmptyClusterAbstraction.h"
#include "EmptyClusterFactory.h"
#include "MacroNodeFactory.h"
#include "map.h"
#include "OHAStar.h"
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OHAStarTest );

void OHAStarTest::setUp()
{
}

void OHAStarTest::tearDown()
{
}

void OHAStarTest::relaxEdgeUpdatesPriorityOfNodeWithRespectToMacroParent()
{
	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());

	graph *g = ecmap.getAbstractGraph(0);
	heap openlist;

	MacroNode* mp  = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(2, 0));
	MacroNode* from =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(2, 2));
	MacroNode* to = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(1,2));
	MacroNode* goal =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(3, 4));

	mp->setLabelF(kTemporaryLabel, alg.h(mp, goal));
	from->setLabelF(kTemporaryLabel, alg.h(mp, from) + alg.h(from, goal));
	from->setMacroParent(mp);
	to->setLabelF(kTemporaryLabel, DBL_MAX);
	openlist.add(to);

	edge e(from->getNum(), to->getNum(), 1.0);

	double expectedFCost = ROOT_TWO+1 + alg.h(to, goal);
	alg.relaxEdge(&openlist, g, &e, from->getNum(), to->getNum(), goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("node priority mismatch", expectedFCost, to->getLabelF(kTemporaryLabel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node has wrong macro parent", true, &*to->getMacroParent() == &*mp);
}

void OHAStarTest::
relaxEdgeUpdatesPriorityOfNodeAndSetsMacroParentToItselfGivenAnInterEdge()
{
	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());
	ecmap.buildClusters();

	graph *g = ecmap.getAbstractGraph(0);
	heap openlist;

	MacroNode* mp  = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(2, 0));
	MacroNode* from =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(0, 2));
	MacroNode* to = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(0,3));
	MacroNode* goal =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(3, 4));

	mp->setLabelF(kTemporaryLabel, alg.h(mp, goal));
	from->setLabelF(kTemporaryLabel, alg.h(mp, from) + alg.h(from, goal));
	from->setMacroParent(mp);
	to->setLabelF(kTemporaryLabel, DBL_MAX);
	openlist.add(to);

	edge e(from->getNum(), to->getNum(), 1.0);

	double expectedFCost = alg.h(mp, from) + e.getWeight() + alg.h(to, goal);
	alg.relaxEdge(&openlist, g, &e, from->getNum(), to->getNum(), goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("node priority mismatch", expectedFCost, to->getLabelF(kTemporaryLabel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node has wrong macro parent", true, &*to->getMacroParent() == &*to);
}

void OHAStarTest::relaxEdgeUpdatesPriorityOfNodeThatHasNoMacroParent()
{
	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());
	ecmap.buildClusters();

	graph *g = ecmap.getAbstractGraph(0);
	heap openlist;

	MacroNode* from =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(0, 0));
	MacroNode* to = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(0,3));
	MacroNode* goal =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(3, 4));

	from->setLabelF(kTemporaryLabel, alg.h(from, goal));
	to->setLabelF(kTemporaryLabel, DBL_MAX);
	openlist.add(to);

	edge e(from->getNum(), to->getNum(), 3.0);

	double expectedFCost = e.getWeight() + alg.h(to, goal);
	alg.relaxEdge(&openlist, g, &e, from->getNum(), to->getNum(), goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("node priority mismatch", expectedFCost, to->getLabelF(kTemporaryLabel));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("node has wrong macro parent", true, to->getMacroParent() == 0);
}

void OHAStarTest::getPathFindsAnOptimalPathInTheAbstractGraph()
{
//	std::cout << "getPathFindsAnOptimalPathInTheAbstractGraph"<<std::endl;
	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());
	ecmap.setAllowDiagonals(true);
	ecmap.buildClusters();
	ecmap.buildEntrances();
	graph *g = ecmap.getAbstractGraph(1);

	node* st = ecmap.getNodeFromMap(1,0);
	node* gl = ecmap.getNodeFromMap(3,4);
	int sp = st->getLabelL(kParent);
	int gp = gl->getLabelL(kParent);
	node* spn = g->getNode(sp);
	node* gpn = g->getNode(gp);

	MacroNode* start  = dynamic_cast<MacroNode*>(g->getNode(st->getLabelL(kParent))); 
	MacroNode* goal =  dynamic_cast<MacroNode*>(g->getNode(gl->getLabelL(kParent)));

	double expectedPathLength = ROOT_TWO*4+2;
	int expectedNumSteps = 6;
	//alg.verbose = true;
	path *p = alg.getPath(&ecmap, start, goal);
	//alg.printPath(p);
	//	p->print(true);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("no path returned!", true, p!=0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path has wrong # of steps", expectedNumSteps, (int)p->length());

	double seglengths[5] = {ROOT_TWO+1, 1, ROOT_TWO, ROOT_TWO, ROOT_TWO};
	int sli = 0;

	path* tmp = p;
	while(tmp)
	{
		path* next = tmp->next;
		if(next)
		{
			MacroNode* n = dynamic_cast<MacroNode*>(tmp->n);
			MacroNode* nextn = dynamic_cast<MacroNode*>(next->n);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("path segment has wrong cost", seglengths[sli], 
					alg.h(n, nextn));
		}
		tmp = next;
		sli++;
	}
	delete p;
}

void OHAStarTest::getPathFindsAnOptimalPathInTheLowLevelGraph()
{
	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());
	ecmap.setAllowDiagonals(true);
	ecmap.buildClusters();
	ecmap.buildEntrances();
	graph *g = ecmap.getAbstractGraph(1);

	node* start = ecmap.getNodeFromMap(1,0);
	node* goal = ecmap.getNodeFromMap(3,4);

	double expectedPathLength = ROOT_TWO*4+2;
	int expectedNumSteps = 7;
	//alg.verbose = true;
	path *p = alg.getPath(&ecmap, start, goal);
	//alg.printPath(p);
	//	p->print(true);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("no path returned!", true, p!=0);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path has wrong # of steps", expectedNumSteps, (int)p->length());

//	double seglengths[5] = {ROOT_TWO+1, 1, ROOT_TWO, ROOT_TWO, ROOT_TWO};
//	int sli = 0;
	double length = 0;

	path* tmp = p;
	while(tmp)
	{
		path* next = tmp->next;
		if(next)
		{
			length += alg.h(tmp->n, next->n);

//			MacroNode* n = dynamic_cast<MacroNode*>(tmp->n);
//			MacroNode* nextn = dynamic_cast<MacroNode*>(next->n);
//			CPPUNIT_ASSERT_EQUAL_MESSAGE("path segment has wrong cost", seglengths[sli], 
//					alg.h(n, nextn));
		}
		tmp = next;
	//	sli++;
	}
	delete p;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path length oncorrect: ", expectedPathLength, length);
}

void OHAStarTest::extractBestPathWorksAsAdvertisedWhenPredecessorOfGoalIsItsMacroParent()
{
	CPPUNIT_ASSERT_EQUAL_MESSAGE("implement me", true, false);

	OHAStar alg;
	EmptyClusterAbstraction ecmap(new Map(hpastartest.c_str()), new EmptyClusterFactory(), 
			new MacroNodeFactory(), new EdgeFactory());
	ecmap.buildClusters();

	graph *g = ecmap.getAbstractGraph(0);
	heap openlist;

	MacroNode* start  = dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(2, 0));

	MacroNode* goal =  dynamic_cast<MacroNode*>(ecmap.getNodeFromMap(3, 4));
	double expectedPathLength = ROOT_TWO*4+2;
	int expectedNumSteps = 5;
	path *p = alg.getPath(&ecmap, start, goal);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("path has wrong # of steps", expectedNumSteps, (int)p->length());

	double seglengths[5] = {ROOT_TWO+1, 1, ROOT_TWO+1, ROOT_TWO+1, ROOT_TWO+1};
	int sli = 0;

	path* tmp = p;
	while(tmp)
	{
		path* next = tmp->next;
		if(next)
		{
			MacroNode* n = dynamic_cast<MacroNode*>(tmp->n);
			MacroNode* nextn = dynamic_cast<MacroNode*>(next->n);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("path segment has wrong cost", seglengths[sli], 
					alg.h(n, nextn));
		}
		tmp = next;
		sli++;
	}
}
