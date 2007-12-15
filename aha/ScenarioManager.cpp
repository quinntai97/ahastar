#include "ScenarioManager.h"
#include "map.h"
#include "graph.h"
#include "mapFlatAbstraction.h"
#include "aStar3.h"

#include <iostream>
#include <fstream>

/* 
TODO: add extra parameter to generatePaths: outputfilename and write the generated stuff directly to file instead of printing to console.
*/

using namespace std;

void AHAScenarioManager::generatePaths(const char* _map, const char* outfilename, int numscenarios, int validterrain, int agentsize)
{

	Map *m = new Map(_map);
	mapFlatAbstraction *absMap = new mapFlatAbstraction(m);
	graph *g = absMap->getAbstractGraph(0);
	float version = 2.0;
	
    ofstream scenariofile;
	scenariofile.open(outfilename, ios::out);
	scenariofile << version<<std::endl;
	
	printf("# map\tfromx\tfromy\ttox\ttoy\tA*len\n");
	node *r1, *r2;
	while(numscenarios > 0)
	{
		/* randomly select a start and goal location */
		do {
			r1 = g->getRandomNode();
			r2 = g->getRandomNode();
		} while (!absMap->pathable(r1, r2) || (r1 == r2));

		/* get the length of the path between the two points */
		aStar a;
		path *p;
		p = a.getPath(absMap, r1, r2);

		int cnt = 0;
		double length = 0;
		for (path *q = p; q; q = q->next)
		{
			if (q && q->next)
			{
					double t1, t2;
					t1 = q->n->getLabelL(kFirstData)-q->next->n->getLabelL(kFirstData);
					t2 = q->n->getLabelL(kFirstData+1)-q->next->n->getLabelL(kFirstData+1);
					length += sqrt(t1*t1+t2*t2);
			}
			cnt++;
		}
		
		int x1, x2, y1, y2;
		x1 = r1->getLabelL(kFirstData); y1 = r1->getLabelL(kFirstData+1);
		x2 = r2->getLabelL(kFirstData); y2 = r2->getLabelL(kFirstData+1);
		
		printf("%s\t%d\t%d\t%d\t%d\t%1.2f\n", _map, x1, y1, x2, y2, length);
		scenariofile << _map<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<"\t"<<length<<std::endl;
		
		delete p;
		numscenarios--;
	}
	
	scenariofile.close();
	delete absMap;
		
}

void AHAScenarioManager::loadScenario(const char* filelocation)
{
	return;
}
