#include "InsertionPolicy.h"

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

}

void InsertionPolicy::removeNode(node* n)
{

}
