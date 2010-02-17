#include "ScenarioManager.h"
#include "mapAbstraction.h"

AbstractScenarioManager::~AbstractScenarioManager()
{
	for(int i=0; i < experiments.size(); i++)
	{
		delete experiments[i];
	}
	experiments.clear();
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

void ScenarioManager::writeScenarioFile(const char* filelocation)
{
}

