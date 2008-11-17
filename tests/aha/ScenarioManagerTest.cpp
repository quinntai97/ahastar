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

//CPPUNIT_TEST_SUITE_REGISTRATION( ScenarioManagerTest );

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
	string targetmap(HOGHOME);
	targetmap.append("maps/local/LINE.map"); // no paths for kGround when size > 1
	Map* m = new Map(targetmap.c_str());
	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(m, aastar);
	ama->annotateMap();

	sg->generateExperiments(ama, 50, 2);
	
	delete ama;
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
	int xs, ys, xg, yg, i=0, capability, agentsize;
	double dist, ver;
	string map;

	/* need some test data */
	TestExperiment* texp = expmgr->getExperiment(kPathableToyProblemLST);
	sg->addExperiment(new AHAExperiment(texp->startx, texp->starty, texp->goalx, texp->goaly, texp->caps, texp->size, texp->distance, texp->mapname.c_str())); 
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
	while(testfile>>map>>xs>>ys>>xg>>yg>>capability>>agentsize>>dist) // NB: could break if values are not int/float; how to handle it?
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
		CPPUNIT_ASSERT_EQUAL_MESSAGE("capability is inconsistent between object and scenario file", exp->getCapability(), capability);
		
		i++;
	}
	
	testfile.close();

	/* criteria: check correct number of scenarios written to file */
	CPPUNIT_ASSERT_EQUAL_MESSAGE("number of scenarios in file does not match expectations", numexperiments, i);
	remove(tslocation.c_str()); // need to reset things for other tests to complete OK

	return;

}

void ScenarioManagerTest::generateExperimentsProducesCorrectNumberOfExperiments()
{	
	string emptymap(HOGHOME);
	emptymap.append("tests/testmaps/emptymap.map");
	Map* m = new Map(emptymap.c_str()); // want to make sure we can always find an experiment 
	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(m, aastar);
	ama->annotateMap();

	sg->generateExperiments(ama, numscenarios, agentsize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrect number of experiments generated", numscenarios, sg->getNumExperiments());
	
	delete ama;
}

void ScenarioManagerTest::generateExperimentsProducesExperimentsWithDifferentCapabilityTypes()
{
	string targetmap(HOGHOME);
	targetmap.append("maps/local/demo.map");
	Map* m = new Map(targetmap.c_str());
	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(m, aastar);
	ama->annotateMap();

	int GroundExperiments=0;
	int MultiCapableExperiments=0; // not many trees on this map so we skip that test

	int numExperiments=10;
	sg->generateExperiments(ama,numExperiments,1);
	for(int i=0; i<numExperiments; i++)
	{

		AHAExperiment* exp =  ((AHAExperiment*)sg->getNthExperiment(i));
		int expcapability = exp->getCapability(); 
		if(expcapability == kGround)
			GroundExperiments++;
		if(expcapability == (kTrees|kGround))
			MultiCapableExperiments++;
	
	}
	
	CPPUNIT_ASSERT_MESSAGE("hmm.. no kGround experiments generated. wtf?", GroundExperiments > 0);
	CPPUNIT_ASSERT_MESSAGE("hmm.. no MultiCapable experiments generated. wtf?", MultiCapableExperiments > 0);

	delete ama;
}

// TODO: Replace AMA/AA* with mocks. proper mocks.
void ScenarioManagerTest::generateSingleExperimentReturnsAValidExperimentForTheGivenParameters()
{
	string emptymap(HOGHOME);
	emptymap.append("tests/testmaps/emptymap.map");
	Map* m = new Map(emptymap.c_str()); // want to make sure we can always find an experiment 
	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(m, aastar);
	ama->annotateMap();
	
	int minsize = 1;
	
	AHAExperiment* exp = sg->generateSingleExperiment(ama, capability, minsize);
	CPPUNIT_ASSERT_MESSAGE("incorrectly returned NULL instead of a valid experiment", exp != NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("experiment mapname not the same", 0, strcmp(emptymap.c_str(), exp->getMapName()));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("found an invalid capability for experiment", capability,  exp->getCapability());
	CPPUNIT_ASSERT_MESSAGE("agent size non-valid",  exp->getAgentsize() >= minsize);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("start == goal", true, !(exp->getStartX() == exp->getGoalX() && exp->getStartY() == exp->getGoalY()));
	CPPUNIT_ASSERT_MESSAGE("path distance <= 0", exp->getDistance() > 0);
	
	delete ama;
}

void ScenarioManagerTest::generateSingleExperimentReturnsNullIfNoPairOfStartAndGoalNodesCouldBeLocatedForTheGivenParameters()
{
	string targetmap(HOGHOME);
	targetmap.append("maps/local/pacman.map"); // no paths for kGround when size > 1
	Map* m = new Map(targetmap.c_str());
	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(m, aastar);
	ama->annotateMap();
		
	for(int i=0; i<1000; i++)
	{
		AHAExperiment* exp = sg->generateSingleExperiment(ama, capability, agentsize);
		CPPUNIT_ASSERT_MESSAGE("incorrectly returned an experiment when none should be possible with the given params", exp == NULL);
	}
	
	delete ama;
}

void ScenarioManagerTest::loadScenarioShouldCreateExperimentObjectsGivenAFileThatDescribesExperimentAttributes()
{
	std::string scenariofilelocation(HOGHOME);
	scenariofilelocation.append("tests/testscenarios/demo.map.scenario");
	std::string demomaplocation(HOGHOME);
	demomaplocation.append("maps/local/demo.map");
	AHAExperiment exp(14, 6, 17, 7, (kGround|kTrees), 2, 4, "maps/local/demo.map");
	
	sg->loadScenarioFile(scenariofilelocation.c_str());

	CPPUNIT_ASSERT_EQUAL_MESSAGE("loaded incorrect number of experiments from file", 1, sg->getNumExperiments());

	AHAExperiment *fileExp = ((AHAExperiment*)sg->getNthExperiment(0));
	CPPUNIT_ASSERT_MESSAGE("loaded experiment is null?!", fileExp != NULL);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("startx coordinates do not match", exp.getStartX(), fileExp->getStartX());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("starty coordinates do not match", exp.getStartY() , fileExp->getStartY());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("goalx coordinates do not match", exp.getGoalX(), fileExp->getGoalX());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("goaly coordinates do not match", exp.getGoalY(), fileExp->getGoalY());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("capabilities do not match", exp.getCapability(), fileExp->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("agentsize values do not match", exp.getAgentsize(), fileExp->getAgentsize());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("path distance values do not match", exp.getDistance(), fileExp->getDistance());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("mapname values do not match", 0, strcmp(exp.getMapName(), fileExp->getMapName()));
	

}