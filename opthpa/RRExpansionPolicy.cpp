#include "RRExpansionPolicy.h"

RRExpansionPolicy::RRExpansionPolicy(mapAbstraction* m, node* t) :
	IncidentEdgesExpansionPolicy(m, t)
{
	first();
}

node* RRExpansionPolicy::first()
{
	IncidentEdgesExpansionPolicy::which = 0;
	which_macro = 0;

	return n();
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

node* RRExpansionPolict::n_macro()
{
	EmptyClusterAbstraction* aMap = static_cast<EmptyClusterAbstraction*>(map);	
	MacroNode* t = static_cast<MacroNode*>(target);

	EmptyCluster* room = aMap->getCluster(t->getParentClusterId());
	EmptyCluster::RoomSide side = room->whichSide(t);
	
	int tx = t->getLabelL(kFirstData);
	int ty = t->getLabelL(kFirstData+1);

	int nx, ny;
	switch(side)
	{
		case TOP:
			nx = tx;
			ny = room->getVOrigin()+room->getHeight()-1;
			break;
		case BOTTOM:
			nx = tx;
			ny = room->getVOrigin();
			break;
		case RIGHT:
			nx = room->getHOrigin();
			ny = ty;
			break;
		case LEFT:
			nx = room->getHOrigin()+room->getWidth()-1;
			ny = ty;
			break;
		default:
			nx = ny = -1;
			break;
	}

	node* neighbour = aMap->getNodeFromMap(nx, ny);

	if(neighbour->getLabelLk(kAbstractionLevel) != 
			t->getLabelL(kAbstractionLevel))
	{
		graph* g = aMap->getAbstractGraph(t->getLabelL(kAbstractionLevel));
		neighbour = g->getNode(neighbour->getLabelL(kParent));
		assert(neighbour);
	}
	return neighbour;
}

node* RRExpansionPolicy::next()
{
	if(which_macro < max_macro)
		which_macro++;
	else
		IncidentEdgesExpansionPolicy::next();

	return n();
}

