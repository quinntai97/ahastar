#include "RRExpansionPolicy.h"

#include "IncidentEdgesExpansionPolicy.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "MacroNode.h"
#include "graph.h"

RRExpansionPolicy::RRExpansionPolicy() :
	ExpansionPolicy()
{
	first();
	edgesPolicy = new IncidentEdgesExpansionPolicy();
}

RRExpansionPolicy::~RRExpansionPolicy()
{
	delete edgesPolicy;
}

node* RRExpansionPolicy::first()
{
	which = 0;
	edgesPolicy->expand(target, map);
	return edgesPolicy->first();
}

node* RRExpansionPolicy::n()
{
	if(which >= max)
		return 0;

	node* retVal = edgesPolicy->n();
	if(retVal == 0)
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
			case EmptyCluster::TOP:
				nx = tx;
				ny = room->getVOrigin()+room->getHeight()-1;
				break;
			case EmptyCluster::BOTTOM:
				nx = tx;
				ny = room->getVOrigin();
				break;
			case EmptyCluster::RIGHT:
				nx = room->getHOrigin();
				ny = ty;
				break;
			case EmptyCluster::LEFT:
				nx = room->getHOrigin()+room->getWidth()-1;
				ny = ty;
				break;
			default:
				nx = ny = -1;
				break;
		}

		node* neighbour = aMap->getNodeFromMap(nx, ny);

		if(neighbour->getLabelL(kAbstractionLevel) != 
				t->getLabelL(kAbstractionLevel))
		{
			graph* g = aMap->getAbstractGraph(t->getLabelL(kAbstractionLevel));
			neighbour = g->getNode(neighbour->getLabelL(kParent));
			assert(neighbour);
		}
	}
	return retVal;
}

node* RRExpansionPolicy::next()
{
	node* retVal = edgesPolicy->next();
	if(!retVal)
	{
		which++;
		retVal = n();
	}
	return retVal;
}

bool RRExpansionPolicy::hasNext()
{
	if(which < max)
		return true;
	return false;
}
