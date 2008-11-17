/*
 *  HPAStar2Test.cpp
 *  hog
 *
 *  Created by dharabor on 18/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAStar2Test.h"
#include "path.h"
#include "ClusterNode.h"
#include "HPAClusterAbstraction.h"
#include "map.h"
#include "TestConstants.h"
#include "ClusterAStarFactory.h"
#include "ClusterAStar.h"
#include "HPAClusterFactory.h"
#include "EdgeFactory.h"
#include "ClusterNodeFactory.h"
#include "mapFlatAbstraction.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HPAStar2Test );

void HPAStar2Test::setUp()
{
	p = NULL;
}

void HPAStar2Test::tearDown()
{
	if(p!=NULL)
		delete p;
}

ClusterNode* HPAStar2Test::getNode(int x, int y, HPAClusterAbstraction& hpamap)
{
	return dynamic_cast<ClusterNode*>(hpamap.getNodeFromMap(x,y));

}

void HPAStar2Test::getPathReturnNullWhenStartOrGoalNull()
{	
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);

	p = hpastar.getPath(&hpamap, NULL, n); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start node is null", int(p), NULL);

	p = hpastar.getPath(&hpamap, n, NULL); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when goal node is null", int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenStartAndGoalSameLocation()
{
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	ClusterNode* n2 = getNode(0,1,hpamap);
	n2->setLabelL(kFirstData+1, 0); // both nodes have (0,0) as their coordinates.

	p = hpastar.getPath(&hpamap, n, n2); 
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return null when start & goal have same coordinates.", int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenStartAndGoalNodesIdentical()
{
	string errmsg("getPath() failed to return null when start and goal nodes are identical");
	HPAClusterAbstraction hpamap(new Map(maplocation.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
	hpastar.setGraphAbstraction(&hpamap);
		
	ClusterNode* n = getNode(0,0,hpamap);
	p = hpastar.getPath(&hpamap, n, n); 

	CPPUNIT_ASSERT_EQUAL_MESSAGE(errmsg.c_str(), int(p), NULL);
}

void HPAStar2Test::getPathReturnNullWhenMapAbstractionParameterNull()
{
	mapFlatAbstraction mfa(new Map(maplocation.c_str()));
	HPAStar2 hpastar;
	node* pos = mfa.getNodeFromMap(1,2);
	node* n = mfa.getNodeFromMap(22,1);
	
	p = hpastar.getPath(NULL, pos, n);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with NULL map abstraction parameter", NULL, (int)p); 
}

void HPAStar2Test::getPathReturnNullWhenStartOrGoalHave_kAbstractionLevel_GreaterThanZero()
{
	HPAClusterAbstraction hpamap(new Map(hpaentrancetest.c_str()), new ClusterAStarFactory(), new HPAClusterFactory(), 
		new ClusterNodeFactory(), new EdgeFactory(), TESTCLUSTERSIZE);
	HPAStar2 hpastar;
		
	ClusterNode* n1 = getNode(0,0,hpamap);
	ClusterNode* n2 = getNode(2,2,hpamap);
	
	n1->setLabelL(kAbstractionLevel, 1);
	n2->setLabelL(kAbstractionLevel, 1);
	p = hpastar.getPath(&hpamap, n2, n1); 
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("getPath() failed to return false when called with from/to nodes with abstraction level > 0", NULL, (int)p); 
}