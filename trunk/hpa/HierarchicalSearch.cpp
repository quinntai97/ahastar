#include "HierarchicalSearch.h"

#include "InsertionPolicy.h"
#include "path.h"
#include "RefinementPolicy.h"
#include "statCollection.h"

#include "timer.h"

HierarchicalSearch::HierarchicalSearch(InsertionPolicy* _inspol,
		searchAlgorithm* _alg, RefinementPolicy* _refpol) : searchAlgorithm()
{
	insertPolicy = _inspol;
	alg = _alg;
	refinePolicy = _refpol;
}

HierarchicalSearch::~HierarchicalSearch()
{
	delete insertPolicy;
	delete alg;
	delete refinePolicy;
}


path* HierarchicalSearch::getPath(graphAbstraction *aMap, node *from, 
		node *to, reservationProvider *rp)
{
	resetMetrics();

	node* start = insertPolicy->insert(from);
	node* goal = insertPolicy->insert(to);

	path* abspath = alg->getPath(aMap, start, goal, rp);
	path* refinedPath = refinePolicy->refine(abspath);
	delete abspath;

	insertPolicy->remove(start);
	insertPolicy->remove(goal);

	nodesExpanded = alg->getNodesExpanded() + insertPolicy->getInsertNodesExpanded();
	nodesGenerated = alg->getNodesGenerated() + insertPolicy->getInsertNodesGenerated();
	nodesTouched = alg->getNodesTouched() + insertPolicy->getInsertNodesTouched();
	searchTime = alg->getSearchTime() + insertPolicy->getInsertSearchTime();

	return refinedPath;
}

void HierarchicalSearch::logFinalStats(statCollection* stats)
{
	alg->logFinalStats(stats);
	if(insertPolicy)
	{
		stats->addStat("insNodesExpanded",getName(),getInsertNodesExpanded());
		stats->addStat("insNodesTouched",getName(),getInsertNodesTouched());
		stats->addStat("insNodesGenerated",getName(),getInsertNodesGenerated());
		stats->addStat("insSearchTime",getName(),getInsertSearchTime());
	}
}

void HierarchicalSearch::resetMetrics()
{
	if(insertPolicy)
		insertPolicy->resetMetrics();

	nodesExpanded = 0;
	nodesTouched = 0;
	nodesGenerated = 0;
	searchTime = 0;
}

