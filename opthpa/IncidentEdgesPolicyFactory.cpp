#include "IncidentEdgesPolicyFactory.h"

#include "IncidentEdgesExpansionPolicy.h"
#include "graph.h"

IncidentEdgesPolicyFactory::IncidentEdgesPolicyFactory(graph* g)
{
	this->g = g;
}

IncidentEdgesPolicyFactory::~IncidentEdgesPolicyFactory()
{
}

ExpansionPolicy* IncidentEdgesPolicyFactory::newExpansionPolicy()
{
	IncidentEdgesExpansionPolicy* policy = new IncidentEdgesExpansionPolicy(g);
	return policy;
}
