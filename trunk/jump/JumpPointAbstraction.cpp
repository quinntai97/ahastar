#include "JumpPointAbstraction.h"

#include "IEdgeFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "JumpPointsExpansionPolicy.h"
#include "map.h"

JumpPointAbstraction::JumpPointAbstraction(Map* _m, INodeFactory* _nf, 
		IEdgeFactory* _ef) : mapAbstraction(_m)
{
	abstractions.push_back(getMapGraph(_m));
	nf = _nf;
	ef = _ef;
}

JumpPointAbstraction::~JumpPointAbstraction()
{
	delete nf;
	delete ef;
}

mapAbstraction*
JumpPointAbstraction::clone(Map* _m)
{
	return new JumpPointAbstraction(_m);
}

bool 
JumpPointAbstraction::pathable(node* n, node* m)
{
	return true;
}

void
JumpPointAbstraction::verifyHierarchy()
{
}

void
JumpPointAbstraction::removeNode(node* n)
{
}

void 
JumpPointAbstraction::removeEdge(edge *e, unsigned int absLevel)
{
}

void 
JumpPointAbstraction::addNode(node *n)
{
}

void 
JumpPointAbstraction::addEdge(edge *e, unsigned int absLevel)
{
}

void 
JumpPointAbstraction::repairAbstraction()
{
}

graph* 
JumpPointAbstraction::getJumpPointGraph()
{
	graph* g = makeMapNodes(this->getMap());

	JumpPointsExpansionPolicy expander;
	for(int i=0; i < g->numNodes(); i++)
	{
		node* n = g->getNode(i);
		expander.expand(n); 

		for(node* neighbour = expander.n(); 
				neighbour != 0; 
				neighbour = expander.next())
		{
			edge* e = new edge(n->getNum, neighbour->getNum(),
					this->h(n, neighbour));
			n->addEdge(e);
		}
	}

	return g;
}
