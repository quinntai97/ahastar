#ifndef RREXPANSIONPOLICY_H
#define RREXPANSIONPOLICY_H

#include "SelectiveExpansionPolicy.h"

class IncidentEdgesExpansionPolicy;
class EmptyClusterAbstraction;
class RRExpansionPolicy : public SelectiveExpansionPolicy
{
	public:
		RRExpansionPolicy(EmptyClusterAbstraction*);
		virtual ~RRExpansionPolicy();

		virtual bool hasNext();
		virtual double cost_to_n();

	protected:
		virtual node* first_impl();
		virtual node* next_impl();
		virtual node* n_impl();

	private:
		int which;
		int max;
		IncidentEdgesExpansionPolicy* edgesPolicy;
		EmptyClusterAbstraction* map;
};

#endif
