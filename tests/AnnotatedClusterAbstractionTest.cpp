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

void AnnotatedClusterAbstractionTest::buildClusterReturnNullGivenAnInvalidMapCoordinate()
{
	int startx = -1;
	int starty = -1;

	Cluster *cluster = aca->buildCluster(startx, starty);
	CPPUNIT_ASSERT_EQUAL((int)cluster, NULL);
}