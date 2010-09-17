#include "ExpansionPolicy.h"
#include "graphAbstraction.h"
#include "graph.h"

ExpansionPolicy::ExpansionPolicy(graphAbstraction* map_, node* target_)
{
	this->map = map_;
	this->target = target_;
}

ExpansionPolicy::~ExpansionPolicy() 
{
}
