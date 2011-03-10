#include "DefaultInsertionPolicy.h"

#include "AbstractCluster.h"
#include "ClusterNode.h"
#include "GenericClusterAbstraction.h"
#include "graph.h"

DefaultInsertionPolicy::DefaultInsertionPolicy(GenericClusterAbstraction* _map)
	: InsertionPolicy()
{
	map = _map;
}

DefaultInsertionPolicy::~DefaultInsertionPolicy()
{

}

// insert start and goal nodes into the abstract graph before running
// any search.
node*
DefaultInsertionPolicy::insert(node* n)
	throw(std::invalid_argument)
{
	node* retVal = 0;
	//assert(insertedStartNode == 0 && insertedGoalNode == 0);
	if(n->getLabelL(kParent) == -1)
	{
		ClusterNode* target = dynamic_cast<ClusterNode*>(n);
		AbstractCluster* cluster = map->getCluster(
			target->getParentClusterId());

		if(getVerbose())
		{
			const int x = target->getLabelL(kFirstData);
			const int y = target->getLabelL(kFirstData+1);
			std::cout << "inserting node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;
		}

		cluster->addParent(target);
		graph* absg = map->getAbstractGraph(1);
		retVal = absg->getNode(target->getLabelL(kParent));
		addNode(retVal);
		//assert(retVal);
	}
	else
		retVal = n;

	return retVal;
}

void 
DefaultInsertionPolicy::remove(node* n) 
	throw(std::runtime_error)
{
	removeNode(n);
}

