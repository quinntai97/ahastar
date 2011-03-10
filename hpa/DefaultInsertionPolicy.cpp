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
void
DefaultInsertionPolicy::insertStartAndGoalNodesIntoAbstractGraph(
		node* s, node* g)
	throw(std::invalid_argument)
{

	nodesExpanded = nodesTouched = nodesGenerated = 0;
	searchTime = 0;
	
	//assert(insertedStartNode == 0 && insertedGoalNode == 0);
	if(s->getLabelL(kParent) == -1)
	{
		ClusterNode* start = dynamic_cast<ClusterNode*>(s);
		AbstractCluster* cluster = this->getCluster(
			start->getParentClusterId());

		if(getVerbose())
		{
			const int x = start->getLabelL(kFirstData);
			const int y = start->getLabelL(kFirstData+1);
			std::cout << "inserting start node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;
		}

		cluster->addParent(start);
		graph* absg = this->getAbstractGraph(1);
		insertedStartNode = dynamic_cast<ClusterNode*>(
				absg->getNode(start->getLabelL(kParent)));
		//assert(insertedStartNode);
	}

	if(g->getLabelL(kParent) == -1)
	{
		ClusterNode* goal = dynamic_cast<ClusterNode*>(g);
		AbstractCluster* cluster = this->getCluster(
			goal->getParentClusterId());

		if(getVerbose())
		{
			const int x = goal->getLabelL(kFirstData);
			const int y = goal->getLabelL(kFirstData+1);
			std::cout << "inserting goal node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;
		}

		cluster->addParent(goal);
		graph* absg = this->getAbstractGraph(1);
		insertedGoalNode = dynamic_cast<ClusterNode*>(
				absg->getNode(goal->getLabelL(kParent)));
		//assert(insertedGoalNode);
	}
}

void 
DefaultInsertionPolicy::removeStartAndGoalNodesFromAbstractGraph() 
	throw(std::runtime_error)
{
	graph* absg = this->getAbstractGraph(1);
	if(insertedGoalNode)
	{
		AbstractCluster* cluster = this->getCluster(
				insertedGoalNode->getParentClusterId());

		if(getVerbose())
			std::cout << "removing inserted goal node"<<std::endl;

		cluster->removeParent(insertedGoalNode);
		//assert(insertedGoalNode->getNumEdges() == 0);
		absg->removeNode(insertedGoalNode->getNum());
		delete insertedGoalNode;
		insertedGoalNode = 0;
	}

	if(insertedStartNode)
	{
		AbstractCluster* cluster = this->getCluster(
				insertedStartNode->getParentClusterId());

		if(getVerbose())
			std::cout << "removing inserted start node"<<std::endl;

		cluster->removeParent(insertedStartNode);
		delete insertedStartNode;
		//assert(insertedStartNode->getNumEdges() == 0);
		absg->removeNode(insertedStartNode->getNum());
		insertedStartNode = 0;
	}
}

