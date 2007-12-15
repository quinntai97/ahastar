/*
 *  ScenarioGeneratorTest.h
 *  hog
	Tests: 
		- generateScenarioTest
		- loadScenarioTest
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENARIOGENERATORTEST_H
#define SCENARIOGENERATORTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ScenarioGenerator.h"
#include "TestHelper.h"

using namespace CppUnit;

class ScenarioGeneratorTest: TestFixture
{
	CPPUNIT_TEST_SUITE( ScenarioGeneratorTest );
	CPPUNIT_TEST( GenerateScenarioTest );
	CPPUNIT_TEST( LoadScenarioTest );
	CPPUNIT_TEST_SUITE_END();

	public:
		ScenarioGeneratorTest();
		void setUp();
		void tearDown();
		
		void GenerateScenarioTest();
		void LoadScenarioTest();

	private:
		ScenarioGenerator* sg;
		int targetterrain;
		int agentsize;
		int numscenarios;
		float filever;

};

#endif