/*
 *  ScenarioManager.h
 *  hog
 
	Similar to HOG's pathGeneration class but adds suport for scenario generation according to some set of restrictions:
		- agent size (generate valid paths for both large and small agents)
		- agent capabilities (generate paths guaranteed traversable given some capability traversal capabilities)

	Replacement for HOG's pathGeneration and ScenarioLoader functionality. 
	AbstractScenarioManager brings these together into a single class for managing all aspects of scenario creation and loading of scenarios
	into Experiment classes. Specific requiements for scenario files can be achieved by implementing the virtual methods.
	No reference implementation currently;
		
	NB: 1. AHAScenarioManager provides a custom generator/loader. Doesn't need buckets or cares about map sizes. 
		2. Could provide HOG's behaviour as the default implementation.
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Created by Nathan Sturtevant on 11/30/05.
 *  Copyright 2005 Nathan Sturtevant, University of Alberta. All rights reserved.
 *
 * This file is part of HOG.
 *
 * HOG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * HOG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with HOG; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef AHASCENARIOMANAGER_H
#define AHASCENARIOMANAGER_H

#include "map.h"
#include "AnnotatedAStar.h"
#include "AnnotatedMapAbstraction.h"
#include "scenarioLoader.h"
#include "ScenarioManager.h"

#include <iostream>
#include <fstream>

using namespace std;

class AHAExperiment : public Experiment
{
	public:
		AHAExperiment(int xs, int ys, int xg, int yg, int capability, int agentsize, float dist, string mapname) :
			Experiment(xs, ys, xg, yg, 0, dist, mapname), capability(capability), agentsize(agentsize) { }
		virtual ~AHAExperiment() { }
		
		int getCapability() { return capability; }
		void setCapability(int newcapability) { capability = newcapability; }
		int getAgentsize() { return agentsize; }
		void setAgentsize(int newsize) { agentsize = newsize; }

		virtual void print(std::ostream& out);
		
	private:
		int capability, agentsize;
};


class AHAScenarioManager: public AbstractScenarioManager
{
	#ifdef UNITTEST
		friend class AHAScenarioManagerTest;
	#endif
	
	public: 
		AHAScenarioManager() {};
		virtual void generateExperiments(mapAbstraction*, int numscenarios, int minagentsize) throw(TooManyTriesException);
		virtual void loadScenarioFile(const char* filelocation);

	protected:
		AHAExperiment* generateSingleExperiment(mapAbstraction*, int capability, int minagentsize);

		void loadV2ScenarioFile(ifstream&);
		void loadV21ScenarioFile(ifstream&);
};

#endif

