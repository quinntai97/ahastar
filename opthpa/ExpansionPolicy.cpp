#include "ExpansionPolicy.h"
#include "mapAbstraction.h"
#include "graph.h"

ExpansionPolicy::ExpansionPolicy()
{
	this->map = 0;
	this->target = 0;
}

ExpansionPolicy::~ExpansionPolicy() 
{
}

ExpansionPolicy::expand(node* t, mapAbstraction* m)
{
	this->target = t;
	this->map = m;
}
