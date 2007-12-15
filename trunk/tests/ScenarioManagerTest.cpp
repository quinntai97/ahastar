/*
 *  ScenarioManagerTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "ScenarioManagerTest.h"
#include "TestHelper.h"


CPPUNIT_TEST_SUITE_REGISTRATION( ScenarioManagerTest );

void ScenarioManagerTest::setUp()
{	
	sg = new AHAScenarioManager();
	numscenarios = 50;
	filever = 2.0;

}

void ScenarioManagerTest::tearDown()
{
	delete sg;
}

void ScenarioManagerTest::GenerateScenarioTest()
{

	// TEST1: file at appropriate location
	remove(tslocation.c_str()); // NB: randomly created each time the test is executed
	sg->generatePaths(maplocation.c_str(), tslocation.c_str(), numscenarios, targetterrain, agentsize);
	ifstream testfile;
	testfile.open("test.scenario", ios::in);
	CPPUNIT_ASSERT_MESSAGE("failed to open 'test.scenario'", testfile.is_open() != 0);
	
	// TEST2: check file version (should be 2.0)
	float ver;
	testfile >> ver;
	CPPUNIT_ASSERT_MESSAGE("'test.scenario' not version 2.0 file", ver == 2.0);
	
	// Read in & store experiments
	
	string map;
	int xs, ys, xg, yg, i;
	float dist;
	while(testfile>>map>>xs>>ys>>xg>>yg>>dist) // NB: could break if values are not int/float; how to handle it?
	{
		// TEST3: each scenario includes all required parameters (all non-zero, positive integers)
		CPPUNIT_ASSERT_MESSAGE("expectedly found scenario that does not match test parameters", (xs>=0 && ys >=0 && xg >=0 && yg >=0 && dist>0));

		// TEST2: no scenarios with start/goal at same position
		CPPUNIT_ASSERT_MESSAGE("found a scenario with same start and goal locations", !(xs == xg && ys == yg));
		i++;
	}

	// TEST4: correct number of scenarios generated
	CPPUNIT_ASSERT_MESSAGE("number of scenarios in file does not match expectations", i == numscenarios);

	return;
}

void ScenarioManagerTest::LoadScenarioTest()
{
	sg->loadScenario(tslocation.c_str());
}
