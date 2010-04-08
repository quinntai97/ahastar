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

