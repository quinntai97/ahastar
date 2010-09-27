#ifndef INCIDENTEDGESEXPANSIONPOLICY_H
#define INCIDENTEDGESEXPANSIONPOLICY_H

#include "SelectiveExpansionPolicy.h"

class graph;
class node;

class IncidentEdgesExpansionPolicy : public SelectiveExpansionPolicy
{
	public:
		IncidentEdgesExpansionPolicy(graph *g);
		virtual ~IncidentEdgesExpansionPolicy();
		virtual bool hasNext();
		virtual double cost_to_n();


	protected:
		virtual node* next_impl();
		virtual node* first_impl();
		virtual node* n_impl();

	private:
		int which;
		graph* g;
};

#endif
