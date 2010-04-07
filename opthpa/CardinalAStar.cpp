#include "CardinalAStar.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"

CardinalAStar::CardinalAStar()
{
}

CardinalAStar::~CardinalAStar()
{
}

path* CardinalAStar::getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp)
{
	path *p = ClusterAStar::getPath(aMap, from, to, rp);
	return p;
}

// any edges which have non-integer costs are assumed to be diagonal and will
// not be evaluated/expanded.
bool CardinalAStar::evaluate(node* current, node* target, edge* e)
{
	bool retVal = ClusterAStar::evaluate(current, target, e);
	if(e->getWeight() != (int)e->getWeight())
		retVal = false;
	if(e->getWeight() > 2 && !retVal)
		std::cerr << "found a macro edge with non-integer costs! wtf?";

	return retVal;
}

