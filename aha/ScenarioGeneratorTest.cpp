/*
 *  ScenarioGeneratorTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "ScenarioGeneratorTest.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

void ScenarioGeneratorTest::setUp()
{	
	sg = new ScenarioGenerator();
	numscenarios = 50;
	filever = 2.0

}

void ScenarioGeneratorTest::tearDown()
{
	delete sg;
}

void ScenarioGeneratorTest::GenerateScenarioTest()
{

	// TEST1: file at appropriate location
	remove(targetfile); // NB: randomly created each time the test is executed
	sg.generatePaths(tslocation, targetterrain, agentsize);
	ofstream testfile;
	testfile.open("test.scenario", ios::in);
	CPPUNIT_ASSERT_MESSAGE("failed to open 'test.scenario'", testfile.is_open() != 0);
	
	// TEST2: check file version (should be 2.0)
	float ver;
	string first;
	sfile>>first;
	CPPUNIT_ASSERT_MESSAGE("'test.scenario' not version 2.0 file", first == 2.0);
	
	// Read in & store experiments
	
	string map;
	int xs, ys, sg, yg, i=0;
	float dist;
	while(sfile>>map>>xs>>ys>>xg>>yg>>dist) 
	{
		// TEST3: each scenario includes all required parameters (all non-zero, positive integers)
		CPPUNIT_ASSERT_MESSAGE("expectedly found scenario that does not match test parameters", (xs>=0 && ys >=0 && xg >=0 && yg >=0, &&dist>0));

		// TEST2: no scenarios with start/goal at same position
		CPPUNIT_ASSERT_MESSAGE("found a scenario with same start and goal locations", (xs != xg && xg != yg))
		i++;
	}

	// TEST4: correct number of scenarios generated
	CPPUNIT_ASSERT_MESSAGE("number of scenarios in file does not match expectations", i == numscenarios);

	return;
}

void ScenarioGeneratorTest::LoadScenarioTest()
{
	sg.loadScenario(tslocation);
	return;
}
