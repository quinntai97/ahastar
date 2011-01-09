#include "EmptyCluster.h"

#include "ClusterNode.h"
#include "constants.h"
#include "EmptyClusterAbstraction.h"
#include "FlexibleAStar.h"
#include "fpUtil.h"
#include "IEdgeFactory.h"
#include "IncidentEdgesExpansionPolicy.h"
#include "MacroEdge.h"
#include "MacroNode.h"
#include "MacroNodeFactory.h"
#include "ManhattanHeuristic.h"
#include "OctileHeuristic.h"

EmptyCluster::EmptyCluster(int x, int y, EmptyClusterAbstraction* map, 
		bool pr, bool bfr) : AbstractCluster(x, y, map)
{
	this->perimeterReduction = pr;
	this->bfReduction = bfr;
}

EmptyCluster::~EmptyCluster()
{
	// secondary edges cleaned when ::~AbstractCluster calls
	// overridden function ::removeParent
}

// iterates over the nodes assigned to the cluster and adds into the abstract
// graph a parent for every node which is a transition point
// (i.e. is adjacent to a node from a neighbouring cluster).
void 
EmptyCluster::buildEntrances()
{
	if(verbose)
		std::cout << "EmptyCluster::buildEntrances\n";

	frameCluster();
		
	graph* absg = map->getAbstractGraph(1);
	for(HPAUtil::nodeTable::iterator it = nodes.begin(); 
			it != nodes.end(); 
			it++)
	{
		MacroNode* n = dynamic_cast<MacroNode*>((*it).second);
		assert(n);
		
		if(n->getLabelL(kParent) != -1)
		{
			node* parent = absg->getNode(n->getLabelL(kParent));
			assert(parent);
			addParent(parent);
		}
	}

	if(perimeterReduction)
		reducePerimeter();
}

//void 
//EmptyCluster::buildEntrances()
//{
//	for(HPAUtil::nodeTable::iterator it = nodes.begin(); it != nodes.end(); it++)
//	{
//		ClusterNode* n = dynamic_cast<ClusterNode*>((*it).second);
//		assert(n);
//
//		// perimeter nodes either have a smaller branching factor
//		// than expected (being next to obstacles) 
//		// or they're adjacent to nodes from an adjoining cluster
//		bool allowDiagonals = getMap()->getAllowDiagonals();
//		if((allowDiagonals && n->getNumEdges() < 8) ||
//				(!allowDiagonals && n->getNumEdges() < 4))
//		{
//				addParent(n);
//		}
//		else
//		{
//			neighbor_iterator iter = n->getNeighborIter();
//			for(int neighbourId = n->nodeNeighborNext(iter);
//					neighbourId != -1; 
//					neighbourId = n->nodeNeighborNext(iter))
//			{
//					ClusterNode* neighbour = dynamic_cast<ClusterNode*>(
//									map->getAbstractGraph(0)->getNode(neighbourId));
//					assert(neighbour);
//					if(neighbour->getParentClusterId() != this->getId())
//							addParent(n);   
//			}
//		}
//	}
//}


void
EmptyCluster::frameCluster()
{
	MacroNodeFactory* nf = dynamic_cast<MacroNodeFactory*>(
			map->getNodeFactory());
	graph* absg = map->getAbstractGraph(1);
	assert(nf);

	for(HPAUtil::nodeTable::iterator it = nodes.begin(); 
			it != nodes.end(); 
			it++)
	{
		MacroNode* n = dynamic_cast<MacroNode*>((*it).second);
		assert(n);
		assert(n->getLabelL(kParent) == -1);

		// nodes with branching factors < 8 (4 if diagonals are disallowed) are
		// next to some kind of obstacle and automatically added to the
		// perimeter set
		if(n->getNumEdges() < 4 ||
			(map->getAllowDiagonals() && n->getNumEdges() < 8))
		{
			MacroNode* parent = nf->newNode(n);
			assert(parent);
			parent->setLabelL(kAbstractionLevel, 1);
			absg->addNode(parent);
			n->setLabelL(kParent, parent->getNum());
		}
		else
		{
			// add nodes with neighbours in adjacent clusters to the 
			// perimeter nodes set
			neighbor_iterator iter = n->getNeighborIter();
			for(int neighborId = n->nodeNeighborNext(iter);
					neighborId != -1; 
					neighborId = n->nodeNeighborNext(iter))
			{
				ClusterNode* neighbor = dynamic_cast<ClusterNode*>(
						map->getAbstractGraph(0)->getNode(neighborId));
				assert(neighbor);

				if(neighbor->getParentClusterId() != n->getParentClusterId())
				{
					if(map->getAllowDiagonals() || 
						map->h(n, neighbor) == 1)
					{
						MacroNode* parent = nf->newNode(n);
						assert(parent);
						parent->setLabelL(kAbstractionLevel, 1);
						absg->addNode(parent);
						n->setLabelL(kParent, parent->getNum());
						break;
					}
				}
			}
		}
	}
}

void 
EmptyCluster::connectParent(node* n) 
	throw(std::invalid_argument)
{
	addAdjacentEdges(n);
	addMacroEdges(n);
}

void 
EmptyCluster::removeParent(node* n_)
{
	MacroNode* n = dynamic_cast<MacroNode*>(n_);
	assert(n);

	AbstractCluster::removeParent(n);
	graph* absg = map->getAbstractGraph(1);
	for(unsigned int i=0; i<n->numSecondaryEdges(); i++)
	{
		edge* e = n->getSecondaryEdge(i);
		MacroNode* neighbour = dynamic_cast<MacroNode*>(
				absg->getNode(
					e->getFrom() == n->getNum() ? e->getTo() : e->getFrom() ));
		neighbour->removeSecondaryEdge(e->getEdgeNum());
		n->removeSecondaryEdge(e->getEdgeNum());
		secondaryEdges.erase(secondaryEdges.begin()+e->getEdgeNum());
		delete e;
	}
}

// Takes a parent node and connects it with other parent nodes which are
// adjacent. 
//
// We figure out if two parent nodes are adjacent by looking to see whether the
// non-abstract nodes they represent are adjacent. In each case where this is
// true, we add a new edge between the respective parent nodes.
//
// NB: This method only handles adding edges between immediatelty adjacent nodes
// in neighbouring clusters and between adjacent nodes on the perimeter of the 
// local cluster.
void 
EmptyCluster::addAdjacentEdges(node* parent)
{
	ClusterNode *n = static_cast<ClusterNode*>(
			map->getNodeFromMap(
				parent->getLabelL(kFirstData),
				parent->getLabelL(kFirstData+1)));
	assert(n->getLabelL(kParent) == parent->getNum());

	graph* g = map->getAbstractGraph(0);
	graph* absg = map->getAbstractGraph(1);

	edge_iterator it = n->getEdgeIter();
	for(edge* e = n->edgeIterNext(it); e != 0; e = n->edgeIterNext(it))
	{
		double edgeweight = e->getWeight();
		ClusterNode* nb = dynamic_cast<ClusterNode*>(
				g->getNode(
					e->getFrom()== n->getNum()?e->getTo():e->getFrom()));
		assert(nb);

		if(nb->getParentClusterId() != -1 && nb->getLabelL(kParent) != -1)
		{
			ClusterNode* parent2 = dynamic_cast<ClusterNode*>(
				absg->getNode(nb->getLabelL(kParent)));	
			assert(parent2);
				
			edge* absedge = absg->findEdge(parent->getNum(), parent2->getNum());
			if(!absedge)
			{
				absedge = map->getEdgeFactory()->newEdge(parent->getNum(),
						parent2->getNum(), edgeweight);
				absg->addEdge(absedge);

				if(getVerbose())
				{
					std::cout << "EmptyCluster::addAdjacentEdges connecting: ";
					parent->Print(std::cout);
					parent2->Print(std::cout);
					std::cout << " cost: "<<edgeweight<<std::endl;
				}
			}
		}	
	}
}

void
EmptyCluster::reducePerimeter()
{
	std::vector<ClusterNode*> pruneSet;
	graph* absg = map->getAbstractGraph(1);

	unsigned int numParents = getNumParents();
	unsigned int numAbsNodes = absg->getNumNodes();
	std::cout << " before prune; absnodes: "<<numAbsNodes;
	std::cout << " parents: "<<numParents<<std::endl;

	// add to pruneSet any nodes not adjacent to a parent in another cluster
	for(HPAUtil::nodeTable::iterator it = parents.begin(); 
		it != parents.end(); 
		it++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>((*it).second);
		assert(n);
		std::cout << "reducePerimeter: evaluating parent ";
		n->Print(std::cout);
		std::cout << std::endl;
		bool prune = true;
		if(n->getLabelL(kFirstData == 11) && n->getLabelL(kFirstData+1) == 7)
			std::cout << "stop plz";

		neighbor_iterator iter = n->getNeighborIter();
		int neighbourId = n->nodeNeighborNext(iter);
		while(neighbourId != -1)
		{
			ClusterNode* neighbour = dynamic_cast<ClusterNode*>(
					absg->getNode(neighbourId));
			assert(neighbour);
			if(neighbour->getParentClusterId() != this->getId())
			{
				n->Print(std::cout);
				std::cout << " from cluster "<<n->getParentClusterId() <<
				" has neighbour in cluster "<<neighbour->getParentClusterId();
				neighbour->Print(std::cout);
				prune = false;
				break;
			}
			neighbourId = n->nodeNeighborNext(iter);
		}
		if(prune)
			pruneSet.push_back(n);
	}

	std::cout << " nodes to prune: "<<pruneSet.size()<<std::endl;

	// connect to each other all neighbours of each node to be pruned.
	// then, delete the node from the parents collection
	while(pruneSet.size() > 0)
	{
		ClusterNode* n = pruneSet.at(0);

		neighbor_iterator firstIter = n->getNeighborIter();
		for(int firstId = n->nodeNeighborNext(firstIter); 
				firstId != -1; 
				firstId = n->nodeNeighborNext(firstIter))
		{
			ClusterNode* first = dynamic_cast<ClusterNode*>(
					absg->getNode(firstId));

			neighbor_iterator secondIter = firstIter;
			for(int secondId = n->nodeNeighborNext(secondIter); 
					secondId != -1; 
					secondId = n->nodeNeighborNext(secondIter))
			{
				ClusterNode* second = dynamic_cast<ClusterNode*>(
						absg->getNode(secondId));
				
				if(absg->findEdge(n->getNum(), first->getNum()) &&
						absg->findEdge(n->getNum(), second->getNum()))
				{
					edge* e = map->getEdgeFactory()->newEdge(first->getNum(),
							second->getNum(), map->h(first, second));
					absg->addEdge(e);
				}
				else if(findSecondaryEdge(first->getNum(), second->getNum()))
				{
					addSingleMacroEdge(first, second, map->h(first, second),
							absg, true);
				}
				else
					throw std::logic_error("while reducing perimeter: "
						"found an edge that's neither primary nor seconday!");
			}
		}

		numParents = parents.size();
		numAbsNodes = absg->getNumNodes();
		node* ll = map->getNodeFromMap(n->getLabelL(kFirstData),
				n->getLabelL(kFirstData+1));
		std::cout << "pruning node ";
		n->Print(std::cout);
		std::cout << std::endl;

		this->removeParent(n);
		absg->removeNode(n->getNum());
		pruneSet.erase(pruneSet.begin());

		assert(parents.size() == numParents -1);
		assert((unsigned int)absg->getNumNodes() == numAbsNodes -1);
		assert(ll->getLabelL(kParent) == -1);
		delete n;
	}

	std::cout << "ok, done prunning; numParents: "<<parents.size() <<
		" absg nodes: "<<absg->getNumNodes()<<std::endl;
}

void 
EmptyCluster::addMacroEdges(node* n)
{
	if(getMap()->getAllowDiagonals())
	{
		addShortcutMacroEdges(n);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::N, EmptyClusterNS::NE);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::E, EmptyClusterNS::NE);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::E, EmptyClusterNS::SE);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::S, EmptyClusterNS::SE);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::S, EmptyClusterNS::SW);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::W, EmptyClusterNS::SW);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::W, EmptyClusterNS::NW);
		addDiagonalFanMacroEdgeSet(n, EmptyClusterNS::N, EmptyClusterNS::NW);
	}
	else
	{
		addCardinalMacroEdgeSet(n);
	}
}


// Connects the argument node with nodes on the directly opposite side of the
// cluster. There are two cases to consider:
//  1. N is a perimeter node: this results in a single edge between n and the
// node on the directly opposite side of the cluster.
//  2. N is an interior node (which happens only during the insertion phase of a
// running search): results in 4 edges being created.
void 
EmptyCluster::addCardinalMacroEdgeSet(node* n)
{
	graph* g = map->getAbstractGraph(1);

	node* neighbour = findPerimeterNode(n, EmptyClusterNS::N);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::S);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::E);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::W);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), g, true);
}

// Connects a node n with a set of seconday neighbours that lie 
// on the perimeter of the cluster.
// Two Direction arguments (fd and sd) specify which nodes are in the 
// target set. 
//
// If fd = {NE, SE, SW, NW}, sd cannot be any direction more than 45 degrees
// from fd. This is increased to 90 degrees if fd is cardinal.
//
// e.g. if fd = NE, sd must be one of {N, E}
// however if fd = E, sd can be any of {N, NE, SE, S}
//
// OUTLINE:
// From node n, we cross the cluster in direction fd, one step at a time.
// After each step we look for a perimeter node in direction sd. 
// If such a node is found, we add a secondary edge between node n and it.
// The process is repeated until we can take no more steps in direction fd.
//
// The aim is to connect each node on the perimeter with a set of neighbours
// s.t. an optimal traveral from one side of the cluster to the other is always
// possible without stepping inside the cluster.
void 
EmptyCluster::addDiagonalFanMacroEdgeSet(node* n, EmptyClusterNS::Direction sd, 
		EmptyClusterNS::Direction fd)
{
	Heuristic* h = 0;
	if(map->getAllowDiagonals())
		h = new OctileHeuristic();
	else
		h = new ManhattanHeuristic();
	FlexibleAStar astar(new IncidentEdgesExpansionPolicy(map), h); 

	int x = n->getLabelL(kFirstData);
	int y = n->getLabelL(kFirstData+1);
	while(true)
	{
		ClusterNode* fdn = dynamic_cast<ClusterNode*>(
				map->getNodeFromMap(x, y));

		// stop if we step outside the cluster 
		if(!fdn || fdn->getParentClusterId() != this->getId())
			break;

		// try to find a perimeter node one by stepping
		// only in direction sd.
		ClusterNode* sdn = findPerimeterNode(fdn, sd); 
		if(sdn)
		{
			assert(sdn->getLabelL(kAbstractionLevel) == 1);

			// add (only non-dominated) secondary macro edges
			double dist = map->h(n, sdn);
			path* p = astar.getPath(map, n, sdn);
			double plen = map->distance(p);
			if(fless(dist, plen))
			{
				addSingleMacroEdge(n, sdn, map->h(n, sdn), 
						map->getAbstractGraph(1), true);
			}
			delete p;
		}

		// take one step in direction fd and repeat
		switch(fd)
		{
			case EmptyClusterNS::N:
				y--;
				break;
			case EmptyClusterNS::S:
				y++;
				break;
			case EmptyClusterNS::W:
				x--;
				break;
			case EmptyClusterNS::E:
				x++;
				break;
			case EmptyClusterNS::NW:
				y--;
				x--;
				break;
			case EmptyClusterNS::NE:
				y--;
				x++;
				break;
			case EmptyClusterNS::SW:
				y++;
				x--;
				break;
			case EmptyClusterNS::SE:
				y++;
				x++;
				break;
		}
	}
}

// Connects each node to a set of primary neighbours that are reachable by
// only travelling in a diagonal direction (NE, SE, SW, NW)
void
EmptyCluster::addShortcutMacroEdges(node *n)
{
	node* neighbour = 0;
	neighbour = findPerimeterNode(n, EmptyClusterNS::NE);
	if(neighbour)
	{
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), 
				map->getAbstractGraph(1), false);
	}

	neighbour = findPerimeterNode(n, EmptyClusterNS::SE);
	if(neighbour)
	{
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), 
				map->getAbstractGraph(1), false);
	}

	neighbour = findPerimeterNode(n, EmptyClusterNS::SW);
	if(neighbour)
	{
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), 
				map->getAbstractGraph(1), false);
	}

	neighbour = findPerimeterNode(n, EmptyClusterNS::NW);
	if(neighbour)
	{
		addSingleMacroEdge(n, neighbour, map->h(n, neighbour), 
				map->getAbstractGraph(1), false);
	}
}

void 
EmptyCluster::addSingleMacroEdge(node* from_, node* to_, double weight, 
		graph* absg, bool secondaryEdge)
{
	assert(from_ && to_);
	assert(from_->getUniqueID() != to_->getUniqueID());
	assert(from_->getLabelL(kAbstractionLevel) 
			== to_->getLabelL(kAbstractionLevel));

	MacroNode* from = static_cast<MacroNode*>(from_);
	MacroNode* to = static_cast<MacroNode*>(to_);

	assert(from && to);
	assert(from->getParentClusterId() == to->getParentClusterId());

	edge* e = absg->findEdge(from->getNum(), to->getNum());
	if(e == 0 && from->getParentClusterId() == to->getParentClusterId())
	{
		e = findSecondaryEdge(from->getNum(), to->getNum());
		if(e == 0 && secondaryEdge && bfReduction)
		{
			e = map->getEdgeFactory()->newEdge(from->getNum(), 
					to->getNum(), weight);
			assert(e);
			from->addSecondaryEdge(e);
			to->addSecondaryEdge(e);
			secondaryEdges.push_back(e);
			e->edgeNum = secondaryEdges.size() - 1;
		}
		else
		{
			e = map->getEdgeFactory()->newEdge(from->getNum(), 
					to->getNum(), weight);
			assert(e);
			absg->addEdge(e);
		}

		macro++;
		if(getVerbose())
		{
			std::cout << "added";
			if(secondaryEdge)
				std::cout << " secondary ";
			std::cout << " macro edge: [("<<from->getLabelL(kFirstData)<<", ";
			std::cout <<from->getLabelL(kFirstData+1)<<") <-> (";
			std::cout << to->getLabelL(kFirstData) << ", ";
			std::cout << to->getLabelL(kFirstData+1);
			std::cout <<") wt: "<<weight<< " ] "<<std::endl;
		}
	}
}

edge* 
EmptyCluster::findSecondaryEdge(unsigned int fromId, unsigned int toId)
{
	edge* retVal = 0;
	for(unsigned int i=0; i<secondaryEdges.size(); i++)
	{
		edge* e = secondaryEdges.at(i);
		if((e->getFrom() == fromId && e->getTo() == toId) ||
			(e->getTo() == fromId && e->getFrom() == toId))
		{
			e = retVal;
			break;
		}
	}
	return retVal;
}

// Finds another perimeter node by traveling in one of the eight
// main compass directions away from arg node 'n'.
// The function terminates when the maximum number of allowable steps
// have been taken or when the search extends beyond the current cluster. 
ClusterNode* 
EmptyCluster::findPerimeterNode(node* n, EmptyClusterNS::Direction d, 
		unsigned int maxsteps)
{
	int x = n->getLabelL(kFirstData);
	int y = n->getLabelL(kFirstData+1);
	ClusterNode* retVal = 0;

	for(unsigned int numsteps = 0; numsteps < maxsteps; numsteps++)
	{
		switch(d)
		{
			case EmptyClusterNS::N:
				y--;
				break;
			case EmptyClusterNS::S:
				y++;
				break;
			case EmptyClusterNS::W:
				x--;
				break;
			case EmptyClusterNS::E:
				x++;
				break;
			case EmptyClusterNS::NW:
				y--;
				x--;
				break;
			case EmptyClusterNS::NE:
				y--;
				x++;
				break;
			case EmptyClusterNS::SW:
				y++;
				x--;
				break;
			case EmptyClusterNS::SE:
				y++;
				x++;
				break;
		}

		ClusterNode* neighbour = dynamic_cast<ClusterNode*>(
				map->getNodeFromMap(x, y));
		if(!neighbour || neighbour->getParentClusterId() != this->getId())
			break;

		int parentId = neighbour->getLabelL(kParent);
		if(parentId != -1)
		{
			graph* g = map->getAbstractGraph(1);
			retVal = dynamic_cast<ClusterNode*>(g->getNode(parentId));
			assert(retVal);
			break;
		}
	}
	return retVal;
}

void 
EmptyCluster::openGLDraw()
{
	Map* themap = map->getMap();
	double depthmod = 0.4;
	for(HPAUtil::nodeTable::iterator it = nodes.begin();
			it != nodes.end(); it++)
	{
		node* cur = (*it).second;
		if(isPerimeterNode(cur) && cur->getLabelL(kParent) != -1)
		{
			glBegin(GL_QUADS);
			GLdouble xx, yy, zz, rr;
			int x = cur->getLabelL(kFirstData);
			int y = cur->getLabelL(kFirstData+1);
			themap->getOpenGLCoord(x, y, xx, yy, zz, rr);
			rr*=0.5;
			zz = zz-rr*depthmod;

			glColor3f(0.2, 0.6, 0.2);
			glVertex3f(xx-rr, yy-rr, zz);
			glVertex3f(xx-rr, yy+rr, zz);
			glVertex3f(xx+rr, yy+rr, zz);
			glVertex3f(xx+rr, yy-rr, zz);
			glEnd();
		}
	}
}

bool
EmptyCluster::isPerimeterNode(node* n_)
{
	bool retVal = false;
	ClusterNode* n = static_cast<ClusterNode*>(n_);
	assert(n);

	if(map->getAllowDiagonals() && n->getNumEdges() < 8)
		retVal = true;
	else if(!map->getAllowDiagonals() && n->getNumEdges() < 4)
		retVal = true;
	else
	{
		neighbor_iterator it = n->getNeighborIter();
		for( int nodeId = n->nodeNeighborNext(it);
				nodeId != -1; 
				nodeId = n->nodeNeighborNext(it) )
		{
			ClusterNode* nb = static_cast<ClusterNode*>(
					map->getAbstractGraph(0)->getNode(nodeId));
			if(nb->getParentClusterId() != n->getParentClusterId())
			{
				retVal = true;
				break;
			}
		}
	}

	return retVal;
}
