/*
 *  ScenarioManagerMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 7/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AHASCENARIOMANAGERMOCK_H
#define AHASCENARIOMANAGERMOCK_H

#include "ScenarioManager.h"

// i'm not sure this mock is required
class AHAScenarioManagerMock : AbstractScenarioManager
{
	
	public:
		void generateExperiments(searchAlgorithm *, mapAbstraction*, int numscenarios, int validterrain, int agentsize) =0;
		void loadScenarioFile(const char* filelocation) =0;
		void writeScenarioFile(const char* filelocation) =0;
};

#endif