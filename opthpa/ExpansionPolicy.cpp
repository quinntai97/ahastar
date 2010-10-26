#include "ExpansionPolicy.h"
#include "mapAbstraction.h"
#include "graph.h"

ExpansionPolicy::ExpansionPolicy()
{
	this->target = 0;
	verbose = false;
}

ExpansionPolicy::~ExpansionPolicy() 
{
}

void ExpansionPolicy::expand(node* t)
{
	if(verbose)
	{
		std::cout << "expanding ";
		t->Print(std::cout);
		std::cout << std::endl;
	}
	this->target = t;
}
