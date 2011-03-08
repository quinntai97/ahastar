#include "InsertionPolicy.h"

InsertionPolicy::InsertionPolicy()
{
	resetMetrics();
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
