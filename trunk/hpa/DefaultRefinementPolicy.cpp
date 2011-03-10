#include "DefaultRefinementPolicy.h"

#include "DebugUtility.h"
#include "FlexibleAStar.h"
#include "IncidentEdgesExpansionPolicy.h"
#include "OctileHeuristic.h"
#include "path.h"

DefaultRefinementPolicy::DefaultRefinementPolicy(mapAbstraction* _map)
		: RefinementPolicy(_map)
{
	verbose = true;
	astar = new FlexibleAStar(new IncidentEdgesExpansionPolicy(_map),
				new OctileHeuristic());
	astar->verbose = false; 
	astar->markForVis = false;
}

DefaultRefinementPolicy::~DefaultRefinementPolicy()
{
	delete astar;
	astar = 0;
}

// NB: there is bug when trying to visualise all nodes expanded during
// each refinement search:
// If a node is expanded during one search and only generated during the
// next, the node generated draw color clobbers the expanded draw
// color. Could fix this in FlexibleAStar but the visualisation is still
// not correct until we can somehow represent nodes that have been 
// expanded multiple times (once per refinement).
path*
DefaultRefinementPolicy::refine(path* abspath)
{
	path* thepath = 0;
	for(path* current = abspath; current->next != 0; current = current->next)
	{
		node* start = map->getNodeFromMap(
				current->n->getLabelL(kFirstData), 
				current->n->getLabelL(kFirstData+1));
		node* goal =  map->getNodeFromMap(
				current->next->n->getLabelL(kFirstData), 
				current->next->n->getLabelL(kFirstData+1));
		path* segment = astar->getPath(map, start, goal); 

		if(verbose) 
		{
			std::cout << "refined segment: "<<std::endl; 
			DebugUtility debug(map, astar->getHeuristic());
			debug.printPath(segment); 
			std::cout << " distance: "<<map->distance(segment)<<std::endl; 
		}

		// append segment to refined path
		if(thepath == 0)
			thepath = segment;										
		path* tail = thepath->tail();	

		//avoid overlap between successive segments 
		//(i.e one segment ends with the same node as the next begins)
		if(tail->n->getNum() == segment->n->getNum()) 
		{
			tail->next = segment->next;
			segment->next = 0;
			delete segment;
		}
	}

	return thepath;
}
