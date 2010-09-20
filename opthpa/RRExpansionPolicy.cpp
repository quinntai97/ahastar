#include "RRExpansionPolicy.h"

RRExpansionPolicy::RRExpansionPolicy(mapAbstraction* m, node* t) :
	IncidentEdgesExpansionPolicy(m, t)
{
	first();
}

node* RRExpansionPolicy::first()
{
	which_macro = 0;
	IncidentEdgesExpansionPolicy::first();

	return n();

	// better way to reset counters?
	// does it make sense for first and next to not return anything?
	//
	/*
	 *  policy = new policy();
	 *  for(node* n = policy.n(); n != 0; n = policy->next())
	 *
	 *  maybe first and next should return nodes and drop n() ?
	 *  or make n private? 
	 */
}

node* RRExpansionPolicy::n()
{
	node* neighbour = 0;
	if(which_macro < max_macro)
		neighbour = n_macro();
	else
		neighbour = IncidentEdgesExpansionPolicy::n();

	return neighbour;
}

void RRExpansionPolicy::next()
{
	if(which_macro < max_macro)
		which_macro++;
	else
		IncidentEdgesExpansionPolicy::next();
}
