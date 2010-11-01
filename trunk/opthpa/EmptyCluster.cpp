#include "EmptyCluster.h"

EmptyCluster::EmptyCluster(graphAbstraction* map) 
	: Cluster(map)
{

}

EmptyCluster::~EmptyCluster()
{
}
void EmptyCluster::buildCluster()
{
	growCluster();
	buildEntrances();
}

void EmptyCluster::buildEntrances()
{
	// iterate over all nodes in cluster area
	// identify perimeter nodes.
	// add each one to the parents collection.	
}

void EmptyCluster::connectParent(node* n) 
	throw(std::invalid_argument)
{
	addAdjacentEdges(n);
	addMacroEdges(n);
}

// Takes a parent node and connects it with other parent nodes which are
// adjacent. 
//
// We figure out if two nodes are adjacent by looking to see whether the
// non-abstract nodes they represent are adjacent. In each case where this is
// true, we add a new edge between the respective parent nodes.
//
// NB: each pair of adjacent nodes must be assigned to clusters (either the same
// cluster or separate) before any new edges are created.
void EmptyCluster::addAdjacentEdges(node* parent)
{
	ClusterNode *n = map->getNodeFromMap(parent->getLabelL(kFirstData),
			parent->getLabelL(kFirstData+1));
	assert(n->getLabelL(kParent) == parent->getId());

	graph* g = map->getAbstractGraph(0);
	edge_iterator it = n->getEdgeIter();
	for(edge* e = n->edgeIterNext(it); e != 0; e = edgeIterNext(it));
	{
		ClusterNode* nb = static_cast<ClusterNode*>(g->getNode(
				e->getFrom()== n->getNum()?e->getTo():e->getFrom()));

		if(nb->getParentClusterId() != -1)
		{
			addTransition(n, nb, e->getWeight());
		}	
	}
}

void EmptyCluster::addMacroEdges(node* n)
{
	if(getAllowDiagonals())
	{
		addMacroEdgeSet(n, N, NW);
		addMacroEdgeSet(n, N, NE);
		addMacroEdgeSet(n, E, NE);
		addMacroEdgeSet(n, E, SE);
		addMacroEdgeSet(n, S, SE);
		addMacroEdgeSet(n, S, SW);
		addMacroEdgeSet(n, W, SW);
		addMacroEdgeSet(n, W, NW);
	}
	else
		addCardinalMacroEdges(n);
}


// Connects the argument node with nodes on the directly opposite side of the
// cluster. There are two cases to consider:
//  1. N is a perimeter node: this results in a single edge between n and the
// node on the directly opposite side of the cluster.
//  2. N is an interior node (which happens only during the insertion phase of a
// running search): results in 4 edges being created.
void EmptyCluster::addCardinalMacroEdges(node* n)
{
	ManhattanHeuristic heuristic;
	graph* g = map->getAbstractGraph(1);

	node* neighbour = findPerimeterNode(n, EmptyClusterNS::Direction::N);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, heuristic->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::Direction::S);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, heuristic->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::Direction::E);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, heuristic->h(n, neighbour), g, true);

	neighbour = findPerimeterNode(n, EmptyClusterNS::Direction::W);
	if(neighbour)
		addSingleMacroEdge(n, neighbour, heuristic->h(n, neighbour), g, true);
}

// Connects the argument node with a set of nodes from the orthogonal or
// opposite sides of the cluster. Two Direction arguments specify which nodes 
// are in the target set. The first direction is cardinal. i.e. one of 
// {N, S, E, W} while the second direction is diagonal but adjacent to the 
// cardinal direction. i.e. one of {NE, NW, SE, SW}. 
// 
// Example: if cd = N, dd is restricted to one of {NE, NW}
//
// OUTLINE:
// We travel from node n, in direction dd, until a perimeter node is 
// reached. This requires some maximum number of diagonal steps.
// We connect n to this node and also every other node that we can reach
// by taking between 0 < x < max_diagonal_steps.
// After x diagonal steps, we travel only in direction 'cd' 
// (until a perimeter node is reached).
//
// The aim is to connect each node on the perimeter with a set of neighbours
// s.t. an optimal traveral from one side of the cluster to the other is always
// possible without stepping inside the cluster.
// 
void EmptyCluster::addMacroEdgeSet(node* n, Direction cd, Direction dd)
{
	assert( cd == N || cd == S || cd == E || cd == W);
	assert( dd = NE || dd = NW || dd = SE || dd == SW);

	node* first = findPerimeterNode(n, d1);
	node* second = findPerimeterNode(n, d2);

	if(!first || !second)
		return;


	OctileHeuristic heuristic;
	int dsteps_max = DiagonalStepsBetween(first, second);
	for(int dsteps = 0; dsteps <= dsteps_max; dsteps++)
	{	
		int x = n->getLabelL(kFirstData);
		int y = n->getLabelL(kFirstData+1);
		if(dd == NW)
		{
			x -= dsteps;
			y -= dsteps;
		}
		else if(dd == NE)
		{
			x += dsteps;
			y -= dsteps;
		}
		else if(dd == SE)
		{
			x += dsteps;
			y += dsteps;
		}
		else if(dd == SW)
		{
			x -= dsteps;
			y += dsteps;
		}

		node* neighbour = findPerimeterNode(map->getNodeFromMap(x, y), cd);
		assert(neighbour);
		if(!adjacent(n, neighbour))
		{
			if(dsteps == dsteps_max)
				addSingleMacroEdge(n, neighbour, 
						heuristic->(n, neighbour), g, false);
			else
				addSingleMacroEdge(n, neighbour, 
						heuristic->(n, neighbour), g, true);
		}
	}
}

void EmptyCluster::addSingleMacroEdge(node* from_, node* to_, double weight, 
		graph* absg, bool secondaryEdge)
{
	assert(from_ && to_);
	assert(from_->getUniqueID() != to_->getUniqueID());
	assert(from->getLabelL(kAbstractionLevel) 
			== to->getLabelL(kAbstractionLevel));

	MacroNode* from = static_cast<MacroNode*>(from_);
	MacroNode* to = static_cast<MacroNode*>(to_);

	assert(from && to);
	assert(from->getParentClusterId() == to->getParentClusterId());

	MacroEdge* e = static_cast<MacroEdge*>(
			absg->findEdge(from->getNum(), to->getNum()));
	if(e == 0 && from->getParentClusterId() == to->getParentClusterId())
	{
		e = static_cast<MacroEdge*>(findSecondaryEdge(from->getNum(), to->getNum()));
		if(e == 0 && secondaryEdge && bfReduction)
		{
			e = new MacroEdge(from->getNum(), to->getNum(), weight);
			from->addSecondaryEdge(e);
			to->addSecondaryEdge(e);
			secondaryEdges.push_back(e);
		}
		else
		{
			e = new MacroEdge(from->getNum(), to->getNum(), weight);
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
	else
	{
		e->setSecondary(secondaryEdge);
	}
}


// returns true if node n_ has a neighbour with a different parent cluster. 
bool EmptyCluster::isIncidentWithInterEdge(node* n_)
{
	bool retVal = false;
	MacroNode* n = static_cast<MacroNode*>(n_);
	assert(n);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);
	for(int nbx = nx-1; nbx < nx+2; nbx++)
		for(int nby = ny-1; nby < ny+2; nby++)
		{
			MacroNode* nb = static_cast<MacroNode*>(
					hpamap->getNodeFromMap(nbx, nby));
			if(!getAllowDiagonals() && nbx != nx && nby != ny)
				continue;
			else if(nb && 
					nb->getParentClusterId() != n->getParentClusterId())
			{
				retVal = true;
			}
		}
	
//	if(getVerbose())
//	{
//		std::cout << "checking incidence w/ inter edge for node: ";
//		n->print(std::cout);
//		std::cout << retVal <<std::endl;
//	}

	return retVal;
}

edge* EmptyCluster::findSecondaryEdge(unsigned int fromId, unsigned int toId)
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

// calculate which side of the room's perimeter this node belongs to
EmptyCluster::RoomSide EmptyCluster::whichSide(node* n_)
{
	MacroNode* n = static_cast<MacroNode*>(n_);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	if(ny == this->getVOrigin() && nx >= this->getHOrigin())
		return TOP;
	if(ny == (this->getVOrigin()+this->getHeight()-1) && nx >= this->getHOrigin())
		return BOTTOM;
	if(nx == this->getHOrigin() && ny >= this->getVOrigin())
		return LEFT;
	if(nx == (this->getHOrigin()+this->getWidth()-1) && ny >= this->getVOrigin())
		return RIGHT;	
	
	return NONE;
}

void RectangularRoom::setBestExpandedNode(node *n)
{
	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	if(ny == this->getVOrigin() && nx >= this->getHOrigin())
	{
		bestTop = n;
	}
	if(ny == (this->getVOrigin()+this->getHeight()-1) && nx >= this->getHOrigin())
	{
		bestBottom = n;
	}
	if(nx == this->getHOrigin() && ny >= this->getVOrigin())
	{
		bestLeft = n;
	}
	if(nx == (this->getHOrigin()+this->getWidth()-1) && ny >= this->getVOrigin())
	{
		bestRight = n;
	}
}

node* RectangularRoom::getBestExpandedNode(RoomSide side)
{
	switch(side)
	{
		case TOP:
			return bestTop;
		case BOTTOM:
			return bestBottom;
		case LEFT:
			return bestLeft;
		case RIGHT: 
			return bestRight;
		default:
			return 0;
	}
}


// Finds another perimeter node by traveling in one of the eight
// main compass directions away from arg node 'n'
//
// TODO:
// find another perimeter node using  a limited # of diagonal moves in some
// direction maybe connect to nodes between two nodes?
//
node* EmptyCluster::findPerimeterNode(node* n, Direction d)
{
	int x = n->getLabelL(kFirstData)
	int y = n->getLabelL(kFirstData+1);
	node* retVal = 0;

	while(true)
	{
		switch(d)
		{
			case N:
				y--;
				break
			case S:
				y++;
				break;
			case W:
				x--;
				break;
			case E:
				x++;
				break;
			case NW:
				y--;
				x--;
				break;
			case NE:
				y--;
				x++;
				break;
			case SW:
				y++;
				x--;
				break;
			case SE:
				y--;
				x++;
				break;
		}

		ClusterNode* neighbour = 
			dynamic_cast<ClusterNode*>(map->getNodeFromMap(x, y));
		if(neighbour && 
		   neighbour->getParentClusterId() == this->getId())
		{
			int parentId = neighbour->getLabelL(kParent);
			if(parentId != -1)
			{
				graph* g = map->getAbstractGraph(1);
				retVal = g->getNode(parentId);
				assert(retVal);
				break;
			}
		}
		else
			break;
	}
	return retVal;
}

int EmptyCluster::DiagonalStepsBetween(node* n1, node* n2)
{
	int deltax = n->getLabelL(kFirstData) - dn->getLabelL(kFirstData);
	int deltay = n->getLabelL(kFirstData+1) - dn->getLabelL(kFirstData+1);
	if(deltax < 0)
		deltax *= -1;
	if(deltay < 0)
		deltay *= -1;

	int maxdiagonal = deltax<deltay?deltax:deltay;
	return maxdiagonal;
}

bool EmptyCluster::adjacent(node* n1, node* n2)
{
	bool retVal = false;

	neighbour_iterator iter = n1->getNeighborIter();
	for(int nId = n1->nodeNeighborNext(iter); 
			nId != -1;
		   	nId = n1->nodeNeighborNext(iter))
	{
		if(nId == n2->getId())
		{
			retval = true;
			break;
		}
	}

	return retVal;
}
