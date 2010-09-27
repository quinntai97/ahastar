#include "FourConnectedExpansionPolicy.h"
#include "mapAbstraction.h"

FourConnectedExpansionPolicy::FourConnectedExpansionPolicy(mapAbstraction* map) 
	: ExpansionPolicy()
{
	this->map = map;
	first();
}

FourConnectedExpansionPolicy::~FourConnectedExpansionPolicy()
{
}

// Each 4-connected target node has four neighbours: {above, below, left,
// right}. 
//
// @return: the current neighbour -- or 0 if the neighbour does not exist.
node* FourConnectedExpansionPolicy::n()
{
	node* n = 0;
	switch(which)
	{
		case 0:
			n = map->getNodeFromMap(
					target->getLabelL(kFirstData),
					target->getLabelL(kFirstData+1)-1);
			break;

		case 1:
			n = map->getNodeFromMap(
					target->getLabelL(kFirstData)+1,
					target->getLabelL(kFirstData+1));
			break;
		
		case 2:
			n = map->getNodeFromMap(
					target->getLabelL(kFirstData),
					target->getLabelL(kFirstData+1)+1);
			break;
		case 3:
			n = map->getNodeFromMap(
					target->getLabelL(kFirstData)-1,
					target->getLabelL(kFirstData+1));
			break;
	}

	return n;
}

// Iterates over the neighbours of the target node, one node at a time.
// When all neighbours have been iterated over, 0 is returned.
node* FourConnectedExpansionPolicy::next()
{
	which++;
	if(0 <= which < 4)
		return n();
	return 0;
}

// returns the first neighbour of the target node, resetting at the same time
// the value of the "current" neighbour.
//
// @return: first neighbour of the target node, or 0 if no neighbours exist
node* FourConnectedExpansionPolicy::first()
{
	which = 0;
	return n();
}

bool FourConnectedExpansionPolicy::hasNext()
{
	if(0<= which < 4)
		return true;
	return false;
}
