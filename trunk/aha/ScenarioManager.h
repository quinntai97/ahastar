/*
 *  ScenarioManager.h
 *  hog
 
	Similar to HOG's pathGeneration class but adds suport for scenario generation according to some set of restrictions:
		- agent size (generate valid paths for both large and small agents)
		- agent capabilities (generate paths guaranteed traversable given some terrain traversal capabilities)
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
	Replacement for HOG's pathGeneration and ScenarioLoader functionality. 
	AbstractScenarioManager brings these together into a single class for managing all aspects of scenario creation and loading of scenarios
	into Experiment classes. Specific requiements for scenario files can be achieved by implementing the virtual methods.
	No reference implementation currently;
	
	NB: Could provide HOG's behaviour as the default implementation.
	
	AHAScenarioManager provides a custom generator/loader. Doesn't need buckets or cares about map sizes. 
 
 * You should have received a copy of the GNU General Public License
 * along with HOG; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "map.h"
#include "aStar.h"
#include "scenarioLoader.h"


/* NB: HOG's stuff sucks. Need a ScenarioManager here and combine functionality from pathGeneration and ScenarioLoader. */
class AbstractScenarioManager 
{
	public:
		Experiment* getNthExperiment(int which) { if(which < experiments.size()) return &(experiments[which]); return 0; }
		virtual void generatePaths(const char* _map, const char* outfilename, int numscenarios, int validterrain, int agentsize) = 0;
		virtual void loadScenario(const char* filelocation) = 0;
	
	protected: 
		std::vector<Experiment> experiments;

};

class AHAScenarioManager: public AbstractScenarioManager
{
	public: 
		virtual void generatePaths(const char* _map, const char* outfilename, int numscenarios, int validterrain, int agentsize);
		virtual void loadScenario(const char* filelocation);
};

#endif