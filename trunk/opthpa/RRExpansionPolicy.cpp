#include "RRExpansionPolicy.h"

#include "IncidentEdgesExpansionPolicy.h"
#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "MacroNode.h"
#include "graph.h"

RRExpansionPolicy::RRExpansionPolicy(EmptyClusterAbstraction* map) :
	SelectiveExpansionPolicy()
{
	this->map = map;
	max = 1;

	first();
	edgesPolicy = 0;
}

RRExpansionPolicy::~RRExpansionPolicy()
{
	delete edgesPolicy;
}

node* RRExpansionPolicy::first_impl()
{
	which = 0;
	if(edgesPolicy == 0)
	{
		edgesPolicy = new IncidentEdgesExpansionPolicy(
			map->getAbstractGraph(target->getLabelL(kAbstractionLevel)));	
	}
	
	edgesPolicy->expand(target);
	return edgesPolicy->first();
}

node* RRExpansionPolicy::n_impl()
{
	if(which >= max)
		return 0;

	node* retVal = edgesPolicy->n();
	if(retVal == 0)
	{
		MacroNode* t = static_cast<MacroNode*>(target);

		EmptyCluster* room = map->getCluster(t->getParentClusterId());
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

		node* neighbour = map->getNodeFromMap(nx, ny);

		if(neighbour->getLabelL(kAbstractionLevel) != 
				t->getLabelL(kAbstractionLevel))
		{
			graph* g = map->getAbstractGraph(t->getLabelL(kAbstractionLevel));
			neighbour = g->getNode(neighbour->getLabelL(kParent));
			assert(neighbour);
		}
	}
	return retVal;
}

node* RRExpansionPolicy::next_impl()
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

double RRExpansionPolicy::cost_to_n()
{
	if(edgesPolicy->n())
		return edgesPolicy->cost_to_n();
	else
		return map->h(target, n());
}
