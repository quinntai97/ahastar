/*
 *  ScenarioManagerTest.h
 *  hog
	Tests: 
		- generateScenarioTest
		- loadScenarioTest
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENARIOMANAGERTEST_H
#define SCENARIOMANAGERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ScenarioManager.h"
#include "TestHelper.h"
#include "map.h"

using namespace CppUnit;

class ScenarioManagerTest: public TestFixture
{
	CPPUNIT_TEST_SUITE( ScenarioManagerTest );
	CPPUNIT_TEST( GenerateScenarioTest );
	CPPUNIT_TEST( LoadScenarioTest );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		
		void GenerateScenarioTest();
		void LoadScenarioTest();

	private:
		AHAScenarioManager* sg;
		int targetterrain;
		int agentsize;
		int numscenarios;
		float filever;

};

#endif