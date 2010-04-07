#include "OHAStar.h"

OHAStar::OHAStar()
{
}

OHAStar::~OHAStar()
{
}

path* OHAStar::getPath(graphAbstraction* aMap, node* from, node* to,
		reservationProvider* rp)
{
	return 0;
}


bool OHAStar::evaluate(node* current, node* target, edge* e)
{
	return false;
}

void OHAStar::relaxEdge(heap *nodeHeap, graph *g, edge *e, int source, 
		int nextNode, node *to)
{
}

path* OHAStar::extractBestPath(graph *g, unsigned int current)
{
	return 0;
}

