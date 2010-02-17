#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "scenarioLoader.h"
#include <vector>

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

class mapAbstraction;

class AbstractScenarioManager 
{
	public:
		AbstractScenarioManager(){};
		virtual ~AbstractScenarioManager();

		Experiment* getNthExperiment(int which) { if(which < (int)experiments.size()) return experiments[which]; return 0; }
		void addExperiment(Experiment* newexp) { experiments.push_back(newexp); }
		int getNumExperiments() { return experiments.size(); }
		
		virtual void generateExperiments(mapAbstraction* absMap, int numscenarios, int agentsize) throw(TooManyTriesException) = 0;
		virtual void loadScenarioFile(const char* filelocation) = 0;
		void writeScenarioFile(const char* filelocation);
		void clearExperiments() { experiments.clear(); }
	
	protected: 
		std::vector<Experiment*> experiments;		
};

class ScenarioManager : public AbstractScenarioManager
{
	public:
		ScenarioManager();
		virtual ~ScenarioManager();

		virtual void 
		generateExperiments(mapAbstraction* absMap, int numscenarios, int agentsize) 
		throw(TooManyTriesException);

		virtual void loadScenarioFile(const char* filelocation);
};

#endif

