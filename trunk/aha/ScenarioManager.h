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

#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "map.h"
#include "AnnotatedAStar.h"
#include "AnnotatedMapAbstraction.h"
#include "scenarioLoader.h"

namespace ScenarioManagerNS
{
	const int MAXTRIES=1000000;
}

using namespace ScenarioManagerNS;

class TooManyTriesException : public std::exception
{
	public:
		TooManyTriesException(int _generated, int _target) : generated(_generated), target(_target) { }
		const char* what() const throw()
		{
			std::string errmsg("\n well, this sucks. can't generate enough experiments for the given map. I managed to create ");
			//errmsg = errmsg + generated +" of " + target;
			//std::cout<<errmsg;
			return errmsg.c_str();
		}
	
	private:
		int generated, target;
};

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
		
	private:
		int capability, agentsize;
};

class AbstractScenarioManager 
{
	public:
		AbstractScenarioManager(){};
		virtual ~AbstractScenarioManager();

		Experiment* getNthExperiment(int which) { if(which < (int)experiments.size()) return experiments[which]; return 0; }
		void addExperiment(Experiment* newexp) { experiments.push_back(newexp); }
		int getNumExperiments() { return experiments.size(); }
		
		virtual void generateExperiments(AbstractAnnotatedMapAbstraction* absMap, int numscenarios, int agentsize) throw(TooManyTriesException) = 0;
		virtual void loadScenarioFile(const char* filelocation) = 0;
		virtual void writeScenarioFile(const char* filelocation) = 0;
		void clearExperiments() { experiments.clear(); }
	
	protected: 
		std::vector<Experiment*> experiments;		
};

class AHAScenarioManager: public AbstractScenarioManager
{
	#ifdef UNITTEST
		friend class ScenarioManagerTest;
	#endif
	
	public: 
		AHAScenarioManager() {};
		virtual void generateExperiments(AbstractAnnotatedMapAbstraction*, int numscenarios, int minagentsize) throw(TooManyTriesException);
		virtual void loadScenarioFile(const char* filelocation);
		virtual void writeScenarioFile(const char* filelocation);
	protected:
		AHAExperiment* generateSingleExperiment(AbstractAnnotatedMapAbstraction*, int capability, int minagentsize);
};

#endif

