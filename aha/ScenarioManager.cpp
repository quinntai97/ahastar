#include "ScenarioManager.h"
#include "map.h"
#include "graph.h"
#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"
#include "AHAConstants.h"

#include <iostream>
#include <fstream>

/* 
TODO: add extra parameter to generatePaths: outputfilename and write the generated stuff directly to file instead of printing to console.
*/

using namespace std;

AbstractScenarioManager::~AbstractScenarioManager()
{
	experiments.clear();
}

/* generate some random set of experiments. 
	@params:
			numscenarios - number of experiments to generate.
			capability - capability superset. ie. if validterrain = kGround|kTrees, any experiment involving an agent with capability
							in the set {kGround, kTrees, kGround|kTrees} is OK.
			maxsize - maximal sized agent. any experiment with an agent of size [1-agentsize] is OK.
			
	NB: AnnotatedA* used to determine if a path exists. With a better map abstraction, a solution using pathable() would be somewhat better;
		TODO: perhaps pathable could return the length of the path instead of just true/false?? Double processing atm
*/			 
void AHAScenarioManager::generateExperiments(searchAlgorithm* searchalg, mapAbstraction* absMap, int numscenarios, int capability, int maxsize)
{

	assert(dynamic_cast<AbstractAnnotatedAStar*>(searchalg));
	graph *g = absMap->getAbstractGraph(0);
	const char* _map = absMap->getMap()->getMapName();
	
//	printf("\n# map\tfromx\tfromy\ttox\ttoy\tterrain\tagentsize\tA*len\n");
	node *r1, *r2;
	AHAExperiment* newexp;
	while(numscenarios > 0)
	{	
		/* randomly select a start and goal location */
		int terrain, size, tries=0;
		r1 = r2 = 0;
		path *p=0;
		
		do {
			if(tries == MAXTRIES) // couldn't generate any pathable problems (most likely, map has no valid terrain types)
			{
				//cerr << "AHAScenarioManager::generatePaths fatal: could only generate "<<experiments.size()<< " out of "<<numscenarios<<" paths for map at "<<_map<<endl;
				return;
			}

			terrain = validterrains[random()%NUMTERRAINS];
			size = agentsizes[random()%NUMAGENTSIZES];
			if((terrain&capability) == terrain && size >= 1 && size <= maxsize)
			{
				r1 = g->getRandomNode();
				r2 = g->getRandomNode();
				if((r1->getTerrainType()&terrain) == r1->getTerrainType() && (r2->getTerrainType()&terrain) == r2->getTerrainType())
					p = ((AbstractAnnotatedAStar*)searchalg)->getPath(absMap, r1, r2, terrain, size);
			}
			tries++;

		} while (!p || (r1 == r2));

		/* get the length of the path between the two points */
		double length = p->length();
		
		int x1, x2, y1, y2;
		x1 = r1->getLabelL(kFirstData); y1 = r1->getLabelL(kFirstData+1);
		x2 = r2->getLabelL(kFirstData); y2 = r2->getLabelL(kFirstData+1);
		
		newexp = new AHAExperiment(x1, y1, x2, y2, terrain, size, length, _map);
		this->addExperiment(newexp);
		
		delete p;
		numscenarios--;
	}
}

void AHAScenarioManager::loadScenarioFile(const char* filelocation)
{
	ifstream infile;
	infile.open(filelocation,ios::in);
	
	float version=0;
	infile >> version;
	
	if(version != 2.0)
		return;
	
	int xs, ys, xg, yg, terrain, agentsize;
	float dist;
	string mapfile;
	while(infile>>mapfile>>xs>>ys>>xg>>yg>>terrain>>agentsize>>dist)
		experiments.push_back(new AHAExperiment(xs, ys, xg, yg, terrain, agentsize, dist, mapfile));
	
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
		scenariofile << cur->getMapName() <<"\t"<<cur->getStartX() <<"\t"<<cur->getStartY()<<"\t"<<cur->getGoalX()<<"\t"<<cur->getGoalY()<<"\t"<<cur->getTerrain() <<"\t"<<cur->getAgentsize()<<"\t"<<cur->getDistance()<<std::endl;
	}
	
	scenariofile.close();		
}