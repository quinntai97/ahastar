#include "FourConnectedExpansionPolicy.h"

FourConnectedExpansionPolicy::FourConnectedExpansionPolicy(
		graphAbstraction* map_, node* target_) : ExpansionPolicy(map_, target_)
{
}

FourConnectedExpansionPolicy::~FourConnectedExpansionPolicy()
{
}

// Each 4-connected target node has four neighbours: {above, below, left,
// right}. This 
//
// @return: the current neighbour -- or 0 if the neighbour does not exist.
node* FourConnectedExpansionPolicy::n() const
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
// When all neighbours have been iterated over the current neighbour is set to 
// an EOL character and no further iteration is performed for subsequent calls
// of this function -- unless first() is invoked beforehand.
//
// NB: if the "next" neighbour does not exist, this method will be called
// recursively until the next neighbour that does exist is found or the end of
// the neighbours list is reached.
void FourConnectedExpansionPolicy::next()
{
	which++;
	if(0 <= which < 4)
		if(n() == 0) next();
	else
		which = -2;
}

// returns the first neighbour of the target node, resetting at the same time
// the value of the "current" neighbour.
//
// @return: first neighbour of the target node, or 0 if no neighbours exist
node* FourConnectedExpansionPolicy::first()
{
	which = -1;
	next();
	return n();
}
