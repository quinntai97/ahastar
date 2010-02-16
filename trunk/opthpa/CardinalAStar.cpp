#include "CardinalAStar.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"


path* CardinalAStar::getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp)
{
	path* p = ClusterAStar::getPath(aMap, from, to, rp);
	if(dynamic_cast<EmptyClusterAbstraction*>(aMap))
		p = refine(p);

	return p;
}

// any edges which have non-integer costs are assumed to be diagonal and will
// not be evaluated/expanded.
bool CardinalAStar::evaluate(node* current, node* target, edge* e)
{
	bool retVal = CardinalAStar::evaluate(current, target, e);
	if(e->getWeight() != (int)e->getWeight())
		retVal = false;

	return retVal;
}

// refines an abstract path 
path* CardinalAStar::refine(path* p)
{
	return p;
}
