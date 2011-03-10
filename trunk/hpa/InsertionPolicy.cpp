#include "InsertionPolicy.h"

#include "graph.h"

InsertionPolicy::InsertionPolicy()
{
	resetMetrics();
	insertedNodes = new std::vector<node*>();
}

InsertionPolicy::~InsertionPolicy()
{

}

void InsertionPolicy::resetMetrics()
{
	insertNodesExpanded = 0;
	insertNodesTouched = 0;
	insertNodesGenerated = 0;
	insertSearchTime = 0;
}

void InsertionPolicy::addNode(node* n)
{
	insertedNodes->push_back(n);
}

void InsertionPolicy::removeNode(node* n)
{
	for(std::vector<node*>::iterator it = insertedNodes->begin();
			it != insertedNodes->end();
			it++)	
	{
		node* m = *it; 
		if(n->getUniqueID() == m->getUniqueID())
		{
			insertedNodes->erase(it);
			break;
		}
	}

}
