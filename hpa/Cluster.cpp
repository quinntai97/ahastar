#include "Cluster.h"
#include "ClusterNode.h"

unsigned Cluster::uniqueClusterIdCnt = 0;
Cluster::Cluster(graphAbstraction* map)
{
	this->map = map;
	this->clusterId = ++uniqueClusterIdCnt;
}

Cluster::~Cluster()
{
	for(nodeTable::iterator it = nodes.begin();
			it != nodes.end(); it = nodes.begin())
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>((*it).second);
		n->setParentClusterId(-1);
		nodes.erase(it);
	}

	for(nodeTable::iterator it = parents.begin();
			it != parents.end(); it = parents.begin())
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>((*it).second);
		n->setParentClusterId(-1);
		parents.erase(it);
	}
}

// addParent is responsible for adding new abstract nodes to the cluster and
// creating intra-edges that connect the new node to existing abstract nodes. 
void 
Cluster::addParent(node* _p) throw(std::invalid_argument)
{
	ClusterNode* p = dynamic_cast<ClusterNode*>(_p);

	if(p == NULL)
		throw std::invalid_argument("Cluster::addParent: arg not of type ClusterNode");

	if(parents.find(p->getUniqueID()) != parents.end())
		return; // node already in parents collection
	
	if(map->getAbstractGraph(1)->getNode(p->getNum()) == NULL)
	{
		if(p->getLabelL(kAbstractionLevel) != 0)
			throw std::invalid_argument(
					"Cluster::addParent: arg not in graph at level 0 or 1");
	
		ClusterNode* p2 = dynamic_cast<ClusterNode*>(from->clone());
		p2->setLabelL(kAbstractionLevel, 1);
		map->getAbstractGraph(1)->addNode(p2);
		p->setLabelL(kParent, p2->getNum());
		p = p2;
	}
	
	if(getVerbose())
	{
		std::cout << "addParent ";
		p->Print(std::cout);
		std::cout << std::endl;
	}

	p->setParentClusterId(this->getId());
	connectParent(p);
	parents[p->getUniqueID()] = p;
}

void 
Cluster::removeParent(int nodeid)
{
	nodeTable::iterator it = parents.find(nodeid);
	if(it != parents.end())
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>((*it).second);
		n->setParentClusterId(-1);
		unsigned int numParents= parents.size();
		parents.erase(it);
		assert(parents.size() == --numParents);
	}	
}

// argNode is responsible for assigning nodes to the cluster
void 
Cluster::addNode(node* n) throw(std::invalid_argument)
{
	if(n == NULL)
		throw std::invalid_argument("Cluster::addNode arg is null");

	if(nodes[n->getUniqueID()] != NULL) // already added
		return;

	n->setParentClusterId(this->getId());
	nodes[n->getUniqueID()] = n;
}

// a transition point connects two nodes on the map. usually the nodes are on the
// border of two adjacent clusters (in which case the transition results in an
// inter-edge being created) but this is not necessary.
void 
Cluster::addTransition(node* from, node* to, double edgeweight)
	throw(std::invalid_argument)
{
	if(edgeweight < 0)
		throw std::invalid_argument(
			"Cluster::addTransition edge weight cannot be < 0");

	graph *g = hpamap->getAbstractGraph(1);
	ClusterNode* absfrom = 
		dynamic_cast<ClusterNode*>(g->getNode(from->getLabelL(kParent)));
	ClusterNode* absto = 
		dynamic_cast<ClusterNode*>(g->getNode(to->getLabelL(kParent)));
	
	// Try to reuse existing nodes from the abstract graph, else create new.
	if(absfrom == NULL)
	{
		Cluster* parentCluster = 
			map->getCluster(from->getParentClusterId());
		parentCluster->addParent(from, hpamap); // also creates intra-edges
	}
	if(absto == NULL)
	{	
		Cluster* parentCluster = 
			map->getCluster(to->getParentClusterId());
		parentCluster->addParent(to, hpamap); // also creates intra-edges
	}

	edge* e = g->findEdge(absfrom->getNum(), absto->getNum());
	if(!e)
	{
		edge* e = map->getEdgeFactory()->newEdge(absfrom->getNum(), 
				absto->getNum(), edgeweight);
		g->addEdge(e);

		if(getVerbose())
		{
			std::cout << "addTransitionPoint: ";
			from->Print(std::cout);
			to->Print(std::cout);
			std::cout << " cost: "<<edgeweight<<std::endl;
		}
	}
	else
	{
		if(getVerbose())
		{
			std::cout << "Cluster::addTransition edge already exists: ";
			from->Print(std::cout);
			to->Print(std::cout);
			std::cout << " cost: "<<e->getWeight()<<std::endl;
		}
	}
}

void
Cluster::print(std::ostream& out)
{
	out << "Cluster "<<getId();
	out << " #nodes: "<<nodes.size();
	out << " #parents: "<<parents.size();
}

void 
Cluster::printParents()
{
	nodeTable::iterator it;
	it = parents.begin();
	while(it != parents.end())
	{
		node* n = (*it).second;
		std::cout << "parent node: " <<"addr: "<<&(*n);
		std::cout <<" num: "<<n->getUniqueID() <<" ("<<n->getLabelL(kFirstData);
		std::cout <<","<<n->getLabelL(kFirstData+1)<<")"<<std::endl;
		it++;
	}
}
