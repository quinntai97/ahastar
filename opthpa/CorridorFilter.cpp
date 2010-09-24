#include "CorridorFilter.h"

CorridorFilter::CorridorFilter()
{
}

CorridorFilter::~CorridorFilter()
{
}

bool CorridorFilter::filter(node* n)
{
	if(corridorNodes == NULL) // corridor not set. every node should be considered.
		return true;
	
	if(corridorNodes->find(_n->getUniqueID()) != corridorNodes->end()) // already added
		return true;

	return false;
}

void CorridorFilter::setCorridorNodes(std::map<int, node*>* _nodes) 
{ 
	corridorNodes = _nodes; 
	assert(corridorNodes == _nodes); 
}
