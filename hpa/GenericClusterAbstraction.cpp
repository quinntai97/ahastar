#include "GenericClusterAbstraction.h"

#include "ClusterNode.h"
#include "IClusterFactory.h"
#include "IEdgeFactory.h"
#include "INodeFactory.h"
#include "ManhattanHeuristic.h"
#include "OctileHeuristic.h"

#include "graph.h"

GenericClusterAbstraction::GenericClusterAbstraction(Map* m, IClusterFactory* cf, 
		INodeFactory* nf, IEdgeFactory* ef, bool allowDiagonals_) throw(std::invalid_argument)
	: mapAbstraction(m), allowDiagonals(allowDiagonals_)
{
	nodesExpanded = nodesTouched = nodesGenerated = 0;
	searchTime = 0;

	node* n = nf->newNode("test");
	if(!dynamic_cast<ClusterNode*>(n))
	{
		delete n;
		throw std::invalid_argument("ClusterAbstraction requires a node factory"
				"that produces ClusterNode objects");
	}
	delete n;

	this->cf = cf;
	this->nf = nf;
	this->ef = ef;
		
	abstractions.push_back(getMapGraph(this->getMap(), nf, ef, allowDiagonals)); 
	abstractions.push_back(new graph());	
	insertedGoalNode = insertedStartNode = 0;
	drawClusters=true;
	verbose = false;

	if(allowDiagonals)
		heuristic = new OctileHeuristic();
	else
		heuristic = new ManhattanHeuristic();
}

GenericClusterAbstraction::~GenericClusterAbstraction()
{
	for(HPAUtil::pathTable::iterator it = pathCache.begin(); it != pathCache.end(); it++)
		delete (*it).second;
	pathCache.erase(pathCache.begin(), pathCache.end());

	for(HPAUtil::clusterTable::iterator it = clusters.begin(); it != clusters.end(); it++)
		delete (*it).second;
	clusters.erase(clusters.begin(), clusters.end());

	delete ef;
	delete cf;
	delete nf;
	delete heuristic;
}

double 
GenericClusterAbstraction::h(node *a, node *b)
{
	return heuristic->h(a, b);
}

AbstractCluster* 
GenericClusterAbstraction::getCluster(int cid)
{		
	HPAUtil::clusterTable::iterator it = clusters.find(cid);
	if(it != clusters.end())
		return (*it).second;
	
	return 0;
}

void 
GenericClusterAbstraction::buildEntrances()
{
	cluster_iterator it = this->getClusterIter();
	AbstractCluster* cluster = static_cast<AbstractCluster*>(
			this->clusterIterNext(it));
	while(cluster)
	{
		cluster->setVerbose(getVerbose());
		cluster->buildEntrances();
		cluster = static_cast<AbstractCluster*>(
				this->clusterIterNext(it));
	}
}


void 
GenericClusterAbstraction::addCluster(AbstractCluster* cluster) 
{ 
	clusters[cluster->getId()] = cluster;
} 

/* paths are cached in the direction of the edge (from, to) */
void
GenericClusterAbstraction::addPathToCache(edge* e, path* p)
{
	if(e == NULL || p == NULL)
		return;
	
	pathCache[e->getUniqueID()] = p;
}

// insert start and goal nodes into the abstract graph before running
// any search.
void
GenericClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(
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
GenericClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph() 
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


/* Cache for known paths. Stores one path for each abstract edge. */
path* 
GenericClusterAbstraction::getPathFromCache(edge* e)
{
	if(e == NULL)
		return 0;

	HPAUtil::pathTable::iterator it = pathCache.find(e->getUniqueID());
	if(it != pathCache.end())
		return (*it).second;
	
	return 0;
}

void GenericClusterAbstraction::deletePathFromCache(edge* e)
{
	if(e == NULL)
		return;

	HPAUtil::pathTable::iterator it = pathCache.find(e->getUniqueID());
	if(it != pathCache.end())
	{
		delete (*it).second;
		pathCache.erase(it);
	}
}


AbstractCluster* 
GenericClusterAbstraction::clusterIterNext(cluster_iterator &iter) const
{
	if (iter != clusters.end())
	{
		AbstractCluster* c = (*iter).second;
		iter++;
		return c;
	}
  return 0;
}

void 
GenericClusterAbstraction::openGLDraw()
{
	mapAbstraction::openGLDraw();	

	cluster_iterator it = getClusterIter();
	AbstractCluster *cluster = clusterIterNext(it);
	while(cluster)
	{
		cluster->openGLDraw();
		cluster = clusterIterNext(it);
	}
}

// debugging method
void 
GenericClusterAbstraction::printUniqueIdsOfAllNodesInGraph(graph *g)
{
	node_iterator it = g->getNodeIter();
	node* n = (*it);
	while(n)
	{
		std::cout << "addr: "<<&(*n) << "uid: "<< n->getUniqueID() <<" ("
			<< n->getLabelL(kFirstData)<<","<< n->getLabelL(kFirstData+1)
			<<")"<<std::endl;
		n = g->nodeIterNext(it);
	}
}

void 
GenericClusterAbstraction::print(std::ostream& out)
{
	out << "ClusterAbstraction. "<<getNumClusters()<<" clusters.";
	cluster_iterator it = getClusterIter();
	AbstractCluster* cluster = clusterIterNext(it);
	while(cluster)
	{
		cluster->print(out);
		out<<std::endl;
		cluster = clusterIterNext(it);
	}
}


