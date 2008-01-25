/*
 *  ScenarioManagerTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

//#include <iostream>
//#include <fstream>
//#include <stdio.h>

#include "ScenarioManagerTest.h"
#include "ScenarioManager.h"
#include "TestConstants.h"
#include "ExperimentManager.h"
#include "AnnotatedAStarMock.h"
#include "AnnotatedMapAbstractionMock.h"

using namespace ExpMgrUtil;

CPPUNIT_TEST_SUITE_REGISTRATION( ScenarioManagerTest );

void ScenarioManagerTest::setUp()
{	
	aastar_mock = new AnnotatedAStarMock();
	ama_mock = new AnnotatedMapAbstractionMock(new Map(maplocation.c_str()), aastar_mock);
	expmgr = new ExperimentManager();
	sg = new AHAScenarioManager();
	numscenarios = 50;
	agentsize=2;
	capability=4;
	filever = 2.0;

	// generate some test data and write it to file. NB: data is randomly created each time the test is executed
	remove(tslocation.c_str()); // kill the data from last time (if any)
}

void ScenarioManagerTest::tearDown()
{
	delete sg;
	delete expmgr;
	delete ama_mock; // also deletes its assigned searchalg (here, aastar_mock)
}

void ScenarioManagerTest::NoExperimentsGeneratedWhenMapIsNotTraversable()
{
	aastar_mock->setCurrentTestExperiment(expmgr->getExperiment(kNotPathableStartIsHardObstacleLST));
	sg->generateExperiments(aastar_mock, ama_mock, 50,68,2);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("experiments generated when no pathable problems exist", 0, sg->getNumExperiments());
}

void ScenarioManagerTest::ScenarioFileIsNotCreatedWhenNoExperimentsExist()
{
	/* target function; no test data added */
	sg->writeScenarioFile(tslocation.c_str());
		
	/* criteria: scenario file doesn't exist (nothing to write) */
	testfile.open(tslocation.c_str(), ios::in);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("scenario file exists yet no data available to write", false, testfile.is_open());
}

void ScenarioManagerTest::ScenarioFileWrittenToDiskAndWellFormatted()
{
	int xs, ys, xg, yg, i=0, terrain, agentsize;
	double dist, ver;
	string map;

	/* need some test data */
	TestExperiment* texp = expmgr->getExperiment(kPathableToyProblemLST);
	sg->addExperiment(new AHAExperiment(texp->startx, texp->starty, texp->goalx, texp->goaly, texp->terrain, texp->size, texp->distance, texp->mapname.c_str())); 
	int numexperiments=1;

	/* target function */
	sg->writeScenarioFile(tslocation.c_str());
		
	/* criteria: scenario (test data) can be loaded from file */
	testfile.open(tslocation.c_str(), ios::in);
	CPPUNIT_ASSERT_MESSAGE("failed to open 'test.scenario'", testfile.is_open() != 0);
	
	/* criteria: check file version */
	testfile >> ver;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("'test.scenario' not version 2.0 file", 2.0, ver);
	
	/* Read in experiments from scenario file */
	while(testfile>>map>>xs>>ys>>xg>>yg>>terrain>>agentsize>>dist) // NB: could break if values are not int/float; how to handle it?
	{
		
		/* criteria: each line of the scenario file is consistent with the values inside an experiment object */
		AHAExperiment *exp = ((AHAExperiment*)sg->getNthExperiment(i));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("distance values inconsistent between object and scenario file", exp->getDistance(), dist);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("startx coordinates inconsistent between object and scenario file", exp->getStartX(), xs);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("starty coordinates inconsistent between object and scenario file", exp->getStartY(), ys);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("goalx coordinates inconsistent between object and scenario file", exp->getGoalX(), xg);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("goaly coordinates inconsistent between object and scenario file", exp->getGoalY(), yg);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("map name inconsistent between object and scenario file", 0, strcmp(exp->getMapName(), map.c_str()));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("agent size is inconsistent between object and scenario file", exp->getAgentsize(), agentsize);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("terrain is inconsistent between object and scenario file", exp->getTerrain(), terrain);
		
		i++;
	}
	
	testfile.close();

	/* criteria: check correct number of scenarios written to file */
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of scenarios in file does not match expectations", numexperiments, i);
	remove(tslocation.c_str()); // need to reset things for other tests to complete OK

	return;

}

void ScenarioManagerTest::GeneratedExperimentsAreValid()
{
	aastar_mock->setCurrentTestExperiment(expmgr->getExperiment(kPathableToyProblemLST));
	sg->generateExperiments(aastar_mock, ama_mock, numscenarios, capability, agentsize);

	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect number of experiments generated", numscenarios, sg->getNumExperiments());
	
	for(int i=0; i<sg->getNumExperiments(); i++)
	{
		AHAExperiment* cur = ((AHAExperiment*)sg->getNthExperiment(i));
		
		CPPUNIT_ASSERT_EQUAL_MESSAGE("experiment mapname not the same", 0, strcmp(maplocation.c_str(), cur->getMapName()));
		CPPUNIT_ASSERT_EQUAL_MESSAGE("found a non-valid terrain", cur->getTerrain(), (capability&cur->getTerrain()));
		CPPUNIT_ASSERT_MESSAGE("agent size non-valid",  cur->getAgentsize() >= 1 && cur->getAgentsize() <= agentsize);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("start == goal", true, !(cur->getStartX() == cur->getGoalX() && cur->getStartY() == cur->getGoalY()));
		CPPUNIT_ASSERT_MESSAGE("path distance <= 0", cur->getDistance() > 0);
	}
	
}