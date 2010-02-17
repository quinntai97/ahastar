#include "AHAScenarioManager.h"
#include "map.h"
#include "graph.h"
#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"
#include "AHAConstants.h"

#include <iostream>
#include <fstream>

using namespace std;

/* generate some random set of experiments. 
	@params:
			numscenarios - number of experiments to generate.
			minsize - each experiment must be traversable by an agent of this size.
			absMap - the map abstraction we want to generate experiments on
			
	NB: AnnotatedA* used to determine if a path exists. With a better map abstraction, a solution using pathable() would be somewhat better;

TODO: perhaps pathable could return the length of the path instead of just true/false?? 
Double processing atm.
*/			 
void AHAScenarioManager::generateExperiments(mapAbstraction* absMap, 
		int numscenarios, int minsize) throw(TooManyTriesException)
{

	assert(absMap != 0); // need a test here; throw exception if absMap is null
	assert(dynamic_cast<AbstractAnnotatedMapAbstraction*>(absMap) != 0);
	
	int tries=0;
	int generated=0;
	while(generated < numscenarios)
	{	
		if(tries >= MAXTRIES)
			throw TooManyTriesException(generated, numscenarios);
			
		int capability = capabilities[random()%NUMCAPABILITIES];
		AHAExperiment* exp = generateSingleExperiment(absMap,capability,minsize); // randomly selects a start and goal location 
		if(exp != NULL) 
		{
			this->addExperiment(exp);
			generated++;
		}
		tries++;
	}
}

AHAExperiment* AHAScenarioManager::generateSingleExperiment(mapAbstraction* absMap, 
		int capability, int size)
{
	graph *g = absMap->getAbstractGraph(0);
	const char* _map = absMap->getMap()->getMapName();

	node *r1, *r2;
	AHAExperiment* newexp;

	r1 = r2 = 0;
	path *p=0;

	r1 = g->getRandomNode();
	r2 = g->getRandomNode();
	if(r1->getClearance(capability) >= size && r2->getClearance(capability) >= size && r1 != r2)
	{
		AbstractAnnotatedAStar* searchalg = dynamic_cast<AbstractAnnotatedAStar*>(
		   dynamic_cast<AbstractAnnotatedMapAbstraction*>(absMap)->getSearchAlgorithm());

		searchalg->setCapability(capability);
		searchalg->setClearance(size);
		p = searchalg->getPath(absMap, r1, r2);
	}

	if(!p)
		return NULL;
		
	double length = p->length();	
	int x1, x2, y1, y2;
	
	x1 = r1->getLabelL(kFirstData); y1 = r1->getLabelL(kFirstData+1);
	x2 = r2->getLabelL(kFirstData); y2 = r2->getLabelL(kFirstData+1);
	newexp = new AHAExperiment(x1, y1, x2, y2, capability, size, length, _map);
	
	delete p;
	return newexp;
}

void AHAScenarioManager::loadScenarioFile(const char* filelocation)
{
	ifstream infile;
	infile.open(filelocation,ios::in);
	
	float version=0;
	infile >> version;
	
	if(version != 2.0)
		return;
	
	int xs, ys, xg, yg, capability, agentsize;
	float dist;
	string mapfile;
	while(infile>>mapfile>>xs>>ys>>xg>>yg>>capability>>agentsize>>dist)
		experiments.push_back(new AHAExperiment(xs, ys, xg, yg, capability, agentsize, dist, mapfile));
	
	infile.close();
}

void AHAScenarioManager::writeScenarioFile(const char* filelocation)
{
	if(experiments.size() == 0) // nothing to write
		return;
		
	float version = 2.0;	
	ofstream scenariofile;
	scenariofile.precision(16);
	scenariofile.open(filelocation, ios::out);
	scenariofile << version<<std::endl;

	AHAExperiment *cur;
	for(int i=0; i<experiments.size(); i++)
	{	
		cur = ((AHAExperiment*)experiments.at(i));
		scenariofile << cur->getMapName() <<"\t"<<cur->getStartX() <<"\t"<<cur->getStartY()<<"\t"<<cur->getGoalX()<<"\t"<<cur->getGoalY()<<"\t"<<cur->getCapability() <<"\t"<<cur->getAgentsize()<<"\t"<<cur->getDistance()<<std::endl;
	}
	
	scenariofile.close();		
}
