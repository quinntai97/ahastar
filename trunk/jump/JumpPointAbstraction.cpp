#include "JumpPointAbstraction.h"

#include "IEdgeFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "JumpPointsExpansionPolicy.h"
#include "map.h"

JumpPointAbstraction::JumpPointAbstraction(Map* _m, INodeFactory* _nf, 
		IEdgeFactory* _ef) : mapAbstraction(_m)
{
	nf = _nf;
	ef = _ef;
	abstractions.push_back(getJumpPointGraph());
}

JumpPointAbstraction::~JumpPointAbstraction()
{
	delete nf;
	delete ef;
}

mapAbstraction*
JumpPointAbstraction::clone(Map* _m)
{
	return new JumpPointAbstraction(_m, nf->clone(), ef->clone());
}

bool 
JumpPointAbstraction::pathable(node* n, node* m)
{
	return true;
}

void
JumpPointAbstraction::verifyHierarchy()
{
	graph* g = abstractions[0];

	if(g->getNumEdges() != 0)
	{
		repairAbstraction();
		return;
	}

	for(int i=0; i<g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		if(n->getNumIncomingEdges() != 0)
		{
			repairAbstraction();
			return;
		}
	}
}

void
JumpPointAbstraction::removeNode(node* n)
{
	graph* g = abstractions[0];

	edge_iterator it = n->getEdgeIter();
	for(edge* e = n->edgeIterNext(it); e != 0; e = n->edgeIterNext(it))
	{
		n->removeEdge(e);
	}
	g->removeNode(n);
}

void 
JumpPointAbstraction::removeEdge(edge *e, unsigned int absLevel)
{
	if(absLevel != 0)
		return;

	graph* g = abstractions[0];
	node* from = g->getNode(e->getFrom());	
	if(from == 0)
		return;

	from->removeEdge(e);
}

void 
JumpPointAbstraction::addNode(node *n)
{
	graph* g = abstractions[0];
	g->addNode(n);
}

void 
JumpPointAbstraction::addEdge(edge *e, unsigned int absLevel)
{
	if(absLevel != 0)
		return;

	graph* g = abstractions[0];
	node* from = g->getNode(e->getFrom());
	if(from == 0)
		return;

	from->addEdge(e);
}

void 
JumpPointAbstraction::repairAbstraction()
{
	graph* g = abstractions[0];
	edge_iterator it = g->getEdgeIter();
	for(edge *e = g->edgeIterNext(it); e != 0; e = g->edgeIterNext(it))
	{
		g->removeEdge(e);
		delete e;
	}
	
	for(int i=0; i < g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		edge_iterator it = n->getEdgeIter();
		for(edge* e = n->edgeIterNext(it); e != 0; e = n->edgeIterNext(it))
		{
			node* n1 = g->getNode(e->getFrom());
			node* n2 = g->getNode(e->getTo());
			n1->removeEdge(e);
			n2->removeEdge(e);
			delete e;
		}
	}

	delete g;

	abstractions[0] = getJumpPointGraph();
}

graph* 
JumpPointAbstraction::getJumpPointGraph()
{
	graph* g = makeMapNodes(this->getMap(), nf);
	Map* m = getMap();

	JumpPointsExpansionPolicy expander;
	for(int i=0; i < g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		for(int j = 1; j<9; j++)
		{
			int nx = n->getLabelL(kFirstData);
			int ny = n->getLabelL(kFirstData+1);
			switch(j)
			{
				case 1:
					n->backpointer = g->getNode(m->getNodeNum(nx, ny-1));
					break;
				case 2:
					n->backpointer = g->getNode(m->getNodeNum(nx+1, ny-1));
					break;
				case 3:
					n->backpointer = g->getNode(m->getNodeNum(nx+1, ny));
					break;
				case 4:
					n->backpointer = g->getNode(m->getNodeNum(nx+1, ny+1));
					break;
				case 5:
					n->backpointer = g->getNode(m->getNodeNum(nx, ny+1));
					break;
				case 6:
					n->backpointer = g->getNode(m->getNodeNum(nx-1, ny+1));
					break;
				case 7:
					n->backpointer = g->getNode(m->getNodeNum(nx-1, ny));
					break;
				case 8:
					n->backpointer = g->getNode(m->getNodeNum(nx-1, ny-1));
					break;
			}

			// find a jump point in the direction (parent(n), n)
			if(n->backpointer)
			{
				expander.expand(n); 
				for(node* neighbour = expander.n(); 
						neighbour != 0; 
						neighbour = expander.next())
				{
					edge* e = new edge(n->getNum(), neighbour->getNum(),
							this->h(n, neighbour));
					n->addEdge(e);
				}
			}
		}
	}

	return g;
}
