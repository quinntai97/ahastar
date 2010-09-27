#ifndef INCIDENTEDGESEXPANSIONPOLICY_H
#define INCIDENTEDGESEXPANSIONPOLICY_H

#include "SelectiveExpansionPolicy.h"

class graph;
class mapAbstraction;
class node;

class IncidentEdgesExpansionPolicy : public SelectiveExpansionPolicy
{
	public:
		IncidentEdgesExpansionPolicy(mapAbstraction* map);
		virtual ~IncidentEdgesExpansionPolicy();
		virtual bool hasNext();
		virtual double cost_to_n();


	protected:
		virtual node* next_impl();
		virtual node* first_impl();
		virtual node* n_impl();

	private:
		int which;
		mapAbstraction* map;
};

#endif
