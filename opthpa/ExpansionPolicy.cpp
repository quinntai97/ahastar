#include "ExpansionPolicy.h"
#include "mapAbstraction.h"
#include "graph.h"

ExpansionPolicy::ExpansionPolicy(mapAbstraction* map_, node* target_)
{
	this->map = map_;
	this->target = target_;
}

ExpansionPolicy::~ExpansionPolicy() 
{
}
