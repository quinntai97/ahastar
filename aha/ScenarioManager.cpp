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

/* TODO: replace aastar with call to pathable in AnnotatedClusterAbstraction later on (not very efficient atm) */
void AHAScenarioManager::generatePaths(const char* _map, const char* outfilename, int numscenarios, int validterrain, int agentsize)
{

	Map *m = new Map(_map);
	AnnotatedMapAbstraction absMap(m);	
	graph *g = absMap.getAbstractGraph(0);
	AnnotatedAStar aastar;
	float version = 2.0;	

    ofstream scenariofile;
	scenariofile.open(outfilename, ios::out);
	scenariofile << version<<std::endl;
	
	printf("# map\tfromx\tfromy\ttox\ttoy\tterrain\tagentsize\tA*len\n");
	node *r1, *r2;
	while(numscenarios > 0)
	{	
		/* randomly select a start and goal location */
		int terrain, agentsize, tries=0;
		path *p;
		do {
			terrain = validterrains[random()%NUMTERRAINS];
			agentsize = agentsizes[random()%NUMAGENTSIZES];
			r1 = g->getRandomNode();
			r2 = g->getRandomNode();
			p = aastar.getPath(&absMap, r1, r2, terrain, agentsize);
			if(tries == MAXTRIES) 
			{
				cerr << "couldn't generate any paths for map at "<<m->getMapName()<<endl;
				exit(-1);
			}
			tries++;
		} while (!p || (r1 == r2));

		/* get the length of the path between the two points */
		double length = p->length();
		
		int x1, x2, y1, y2;
		x1 = r1->getLabelL(kFirstData); y1 = r1->getLabelL(kFirstData+1);
		x2 = r2->getLabelL(kFirstData); y2 = r2->getLabelL(kFirstData+1);
		
		printf("%s\t%d\t%d\t%d\t%d\t%1.2f\n", _map, x1, y1, x2, y2, length);
		scenariofile << _map<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<"\t"<<terrain<<"\t"<<agentsize<<"\t"<<length<<std::endl;
		
		delete p;
		numscenarios--;
	}
	
	scenariofile.close();		
}

void AHAScenarioManager::loadScenario(const char* filelocation)
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
		experiments.push_back(AHAExperiment(xs, ys, xg, yg, terrain, agentsize, dist, mapfile));
	
	infile.close();
}