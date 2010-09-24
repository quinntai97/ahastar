#include "FCRRExpansionPolicy.h"

FCRRExpansionPolicy::FCRRExpansionPolicy() :
	ExpansionPolicy()
{
}

FCRRExpansionPolicy::~FCRRExpansionPolicy()
{
}

node* FCRRExpansionPolicy::first()
{
	which_macro = 0;
	return 0;
}

node* FCRRExpansionPolicy::n()
{
	return 0;
}

node* FCRRExpansionPolicy::next()
{
	return 0;
}

bool FCRRExpansionPolicy::hasNext()
{
	return false;
}
