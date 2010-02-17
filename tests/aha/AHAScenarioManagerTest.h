/*
 *  ScenarioManagerTest.h
 *	hog
 
	Scope:
		- Given a map and some agent characteristics (size, traversal caps), random path problems are generated and written to a .scenario file
		- Each entry in the scenario file has an equivalent Experiment object in the ScenarioManager with identical attributes
		- The entries in  the scenario file match some rules about valid path problems:
			* start/goal are not the same
			* terrain type exists (>0)
			* size of agent required to traverse the path is >0

	Out of scope: Pathability between the start and goal locations; the ScenarioManager relies on AnnotatedA* to generate sets of 
			      pathable coordinates so, if AA* passes all tests, logically, the stuff output by the ScenarioManager is also OK in that regard.
 
	TODO: Check if the validity rules stuff is even needed here. Sounds like AA* should check this junk anyway.
	
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AHASCENARIOMANAGERTEST_H
#define AHASCENARIOMANAGERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class AHAScenarioManager;
class AnnotatedAStarMock;
class AnnotatedMapAbstractionMock;
class ExperimentManager;
class TooManyTriesException;

class AHAScenarioManagerTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AHAScenarioManagerTest );
	CPPUNIT_TEST( generateExperimentsProducesCorrectNumberOfExperiments );
	CPPUNIT_TEST( generateExperimentsProducesExperimentsWithDifferentCapabilityTypes );
	CPPUNIT_TEST( generateSingleExperimentReturnsAValidExperimentForTheGivenParameters );
	CPPUNIT_TEST( generateSingleExperimentReturnsNullIfNoPairOfStartAndGoalNodesCouldBeLocatedForTheGivenParameters );
	CPPUNIT_TEST( loadScenarioShouldCreateExperimentObjectsGivenAFileThatDescribesExperimentAttributes );
	CPPUNIT_TEST_EXCEPTION( NoExperimentsGeneratedWhenMapIsNotTraversable, TooManyTriesException );
	CPPUNIT_TEST( ScenarioFileIsNotCreatedWhenNoExperimentsExist );
	CPPUNIT_TEST( ScenarioFileWrittenToDiskAndWellFormatted );

	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		
		/* todo: mock ama pathable() should sometimes return true and other times false (depends on map passed down -- name maybe?)
		  */
		void generateSingleExperimentReturnsAValidExperimentForTheGivenParameters();
		void generateSingleExperimentReturnsNullIfNoPairOfStartAndGoalNodesCouldBeLocatedForTheGivenParameters();
		void generateExperimentsProducesCorrectNumberOfExperiments();
		void generateExperimentsProducesExperimentsWithDifferentCapabilityTypes();
		void NoExperimentsGeneratedWhenMapIsNotTraversable();
		void ScenarioFileIsNotCreatedWhenNoExperimentsExist();
		void ScenarioFileWrittenToDiskAndWellFormatted();
		void loadScenarioShouldCreateExperimentObjectsGivenAFileThatDescribesExperimentAttributes();


	private:
		AHAScenarioManager* sg;
		AnnotatedAStarMock* aastar_mock;
		AnnotatedMapAbstractionMock* ama_mock;
		ExperimentManager* expmgr;
		
		int capability;
		int agentsize;
		int numscenarios;
		float filever;
		std::ifstream testfile;

};

#endif
