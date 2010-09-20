#include "FCRRExpansionPolicy.h"

FCRRExpansionPolicy::FCRRExpansionPolicy(mapAbstraction* m, node* t) :
	IncidentEdgesExpansionPolicy(m, t)
{
}

FCRRExpansionPolicy::~FCRRExpansionPolicy()
{
}

node* FCRRExpansionPolicy::first()
{
	which_macro = 0;
}

node* FCRRExpansionPolicy::n()
{
}

void FCRRExpansionPolicy::next()
{
}
