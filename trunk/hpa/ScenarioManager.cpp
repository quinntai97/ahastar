#include "ScenarioManager.h"
#include "mapAbstraction.h"

#include <fstream>
#include <iostream>

AbstractScenarioManager::~AbstractScenarioManager()
{
	for(unsigned int i=0; i < experiments.size(); i++)
	{
		delete experiments[i];
	}
	experiments.clear();
}

void AbstractScenarioManager::writeScenarioFile(const char* filelocation)
{
	if(experiments.size() == 0) // nothing to write
		return;
		
	float version = 2.1;	// v2.0 had distance after size/capability; 2.1 swaps them.
	std::ofstream scenariofile;
	scenariofile.precision(16);
	scenariofile.open(filelocation, std::ios::out);
	scenariofile << version<<std::endl;

	for(unsigned int i=0; i<experiments.size(); i++)
	{	
		Experiment*	cur = experiments.at(i);
		cur->print(scenariofile);
	}
	
	scenariofile.close();		
}

ScenarioManager::ScenarioManager()
{
}

ScenarioManager::~ScenarioManager()
{
}

void ScenarioManager::generateExperiments(mapAbstraction* absMap, 
		int numscenarios, int agentsize) throw(TooManyTriesException)
{
}

void ScenarioManager::loadScenarioFile(const char* filelocation)
{
}

