#ifndef INCIDENTEDGESEXPANSIONPOLICY_H
#define INCIDENTEDGESEXPANSIONPOLICY_H

#include "ExpansionPolicy.h"

class graph;
class mapAbstraction;
class node;

class IncidentEdgesExpansionPolicy : public ExpansionPolicy
{
	public:
		IncidentEdgesExpansionPolicy(mapAbstraction*, node*);
		virtual ~IncidentEdgesExpansionPolicy();

		virtual node* next();
		virtual node* first();
		virtual node* n();
		virtual bool hasNext();

	private:
		int which;
};

#endif
