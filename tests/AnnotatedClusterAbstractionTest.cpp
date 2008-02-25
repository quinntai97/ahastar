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
#include "TestConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedClusterAbstractionTest );

using namespace std;

void AnnotatedClusterAbstractionTest::setUp()
{
	testmap = new Map(maplocation.c_str()); // TODO: need a separate, larger map to test this junk
	aca = new AnnotatedClusterAbstraction(testmap, new AnnotatedAStarMock());
	expmgr = new ExperimentManager();
}

void AnnotatedClusterAbstractionTest::tearDown()
{
	delete aca;
	delete expmgr;
}

/*void AnnotatedClusterAbstractionTest::experimentInit(const string &maploc)
{
}*/

void AnnotatedClusterAbstractionTest::buildClusterShouldReturnNullGivenAnInvalidMapCoordinate()
{
	int startx = -1;
	int starty = -1;

	Cluster *cluster = aca->buildCluster(startx, starty);
	CPPUNIT_ASSERT_EQUAL(NULL, (int)cluster);
}

void AnnotatedClusterAbstractionTest::buildClusterShouldReturnAClusterAndIncrementNumberOfClustersGivenAValidMapCoordinate()
{
	int startx = 2;
	int starty = 2;
	
	int numclusters = aca->getNumClusters();
	Cluster *cluster = aca->buildCluster(startx, starty);
	cluster = &(static_cast<Cluster&>(*cluster));
	CPPUNIT_ASSERT_EQUAL(true, cluster != NULL);
	CPPUNIT_ASSERT_EQUAL(numclusters+1, aca->getNumClusters());
}

void AnnotatedClusterAbstractionTest::buildClusterShouldReturnNullGivenAValidMapCoordinateAndTerrainIsAHardObstacle()
{
	int startx = 0;
	int starty = 0;
	
	Cluster *cluster = aca->buildCluster(startx, starty);
	CPPUNIT_ASSERT_EQUAL(true, cluster == NULL);
}

void AnnotatedClusterAbstractionTest::addNodesToClusterAssignsAllNodesInAreaMarkedByDimensions()
{
/*	int cid = 0
	int startx = 2;
	int starty = 1;
	int cwidth = 3;
	int cheight = 3;
	AnnotatedCluster *cluster = new Cluster(cid, 0, 0, startx, starty, cwidth, cheight);
	aca->addNodesToCluster(cluster);
	
	CPPUNIT_ASSERT_EQUAL(true, cluster->getNumNodes() == cwidth*cheight);
	for(int x=startx; x<startx+cwidth; x++)
		for(int y=starty; y<starty+cheight; y++)
		{
			node* curnode = aca->getNodeFromMap(x,y);
			CPPUNIT_ASSERT_EQUAL(true, cluster->findNode(curnode->getNum());
		}
	*/
}