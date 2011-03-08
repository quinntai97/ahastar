#include "NoInsertionPolicy.h"
#include "graph.h"

NoInsertionPolicy::NoInsertionPolicy()
{
}

NoInsertionPolicy::~NoInsertionPolicy()
{
}

node* NoInsertionPolicy::insert(node* n)
{
	return n;
}

void NoInsertionPolicy::remove(node* n)
{
	return;
}
