#ifndef INCIDENTEDGESPOLICYFACTORY_H
#define INCIDENTEDGESPOLICYFACTORY_H

// IncidentEdgesPolicyFactory.h
//
// A factory for IncidentEdgesExpansionPolicy objects.
//
// @author: dharabor
// @created: 25/10/2010
//

#include "IExpansionPolicyFactory.h"

class graph;
class ExpansionPolicy;
class IncidentEdgesPolicyFactory : public IExpansionPolicyFactory
{
	public:
		IncidentEdgesPolicyFactory(graph* g);
		virtual ~IncidentEdgesPolicyFactory();

		virtual ExpansionPolicy* newExpansionPolicy();

	private:
		graph* g;
};

#endif

