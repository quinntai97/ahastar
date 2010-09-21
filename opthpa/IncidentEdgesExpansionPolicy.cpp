#include "IncidentEdgesExpansionPolicy.h"

#include "graph.h"
#include "mapAbstraction.h"

IncidentEdgesExpansionPolicy::IncidentEdgesExpansionPolicy(
		mapAbstraction* map, node* t) : ExpansionPolicy(map, t)
{
	first();
}

IncidentEdgesExpansionPolicy::~IncidentEdgesExpansionPolicy()
{
}

node* IncidentEdgesExpansionPolicy::first()
{
	which = 0;
	return n();
}

node* IncidentEdgesExpansionPolicy::n()
{
	node* neighbour = 0;
	if(which < target->getNumEdges())
	{
		edge* e = target->getEdge(which);
		assert(e);

		int neighbourid = e->getFrom()==target->getNum()?e->getTo():e->getFrom();
		graph* g = map->getAbstractGraph(target->getLabelL(kAbstractionLevel));
		neighbour = g->getNode(neighbourid);
	}
	return neighbour;
}

node* IncidentEdgesExpansionPolicy::next()
{
	node* retVal = 0;
	if(hasNext())
	{
		which++;
		retVal = n();
	}

	return retVal;
}

bool IncidentEdgesExpansionPolicy::hasNext()
{
	if((which+1) < target->getNumEdges())
		return true;
	return false;
}
