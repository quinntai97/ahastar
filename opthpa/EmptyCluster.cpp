#include "EmptyCluster.h"

#include "ClusterNode.h"
#include "Entrance.h"
#include "graph.h"
#include "HPAClusterAbstraction.h"
#include "MacroEdge.h"
#include "MacroNode.h"
#include "map.h"

const int DUMMYDIMENSION = 10; // pass this to keep HPACluster constructor happy

EmptyCluster::EmptyCluster(const int x, const int y) throw(std::invalid_argument)
	: HPACluster(x, y, DUMMYDIMENSION, DUMMYDIMENSION)
{
	if(x < 0 || y <0)
		throw std::invalid_argument("EmptyCluster: x,y coordinates must be >= 0");

	macro = 0;
}

EmptyCluster::~EmptyCluster()
{
}

void EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap, int** clearance)
	throw(std::invalid_argument)
{
	if(aMap == NULL)
		throw std::invalid_argument("EmptyCluster::addNodesToCluster cluster"
			   	"abstraction parameter is null");
	extend(aMap, clearance);

	for(int x=getHOrigin(); x<getHOrigin()+getWidth(); x++)
	{
		for(int y=getVOrigin(); y<getVOrigin()+getHeight(); y++)
		{
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x, y));
			if(n)
			{
				addNode(n);
			}
			else
			{
				throw std::invalid_argument("EmptyCluster: tried to add to "
						"cluster a node of type other than ClusterNode");
			}
		}
	}

	//frameCluster(aMap);
	//if(aMap->getAllowDiagonals())
	//	addMacroEdges(aMap);
//	else
//		addCardinalMacroEdges(aMap);
}

void 
EmptyCluster::buildEntrances(HPAClusterAbstraction* hpamap) 
	throw(std::invalid_argument)
{
	if(getVerbose())
	{
		std::cout << "buildEntrances; ";
		print(std::cout);
		std::cout << std::endl;
	}
	HPACluster::buildEntrances(hpamap);

	if(hpamap->getAllowDiagonals())
	   addMacroEdges(hpamap);
	else
		addCardinalMacroEdges(hpamap);
}

void EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap)
	throw(std::invalid_argument)
{
	if(aMap == NULL)
		throw std::invalid_argument("EmptyCluster::addNodesToCluster cluster"
			   	"abstraction parameter is null");
	extend(aMap);

	if(getVerbose())
	{
		std::cout << "cheight: "<<getHeight()<<" cwidth: ";
		std::cout <<getWidth()<<std::endl;
	}

	for(int x=getHOrigin(); x<getHOrigin()+getWidth(); x++)
	{
		for(int y=getVOrigin(); y<getVOrigin()+getHeight(); y++)
		{
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x, y));
			if(n)
			{
				addNode(n);
			}
			else
			{
				throw std::invalid_argument("EmptyCluster: tried to add to "
						"cluster a node of type other than ClusterNode");
			}
		}
	}

	frameCluster(aMap);
	//addMacroEdges(aMap);
}


// Every node framing the cluster is added to the abstract graph. Also, 
// every edge between two adjacent framed nodes will appear in the graph.
// NB: there is a special case where the cluster has size 1x1.
// In this case the single node will be added to the graph when 
// buildHorizontalEntrances and buildVerticalEntrances are called
// rather than by this method
void EmptyCluster::frameCluster(HPAClusterAbstraction* aMap)
{
        if(getVerbose())
                std::cout << "frameCluster "<<this->getId()<<std::endl;

        // add nodes along left border
        ClusterNode* first = 0;
        ClusterNode* last = 0;
        int x = this->getHOrigin();
        int y = this->getVOrigin();
        for( ; y<this->getVOrigin()+this->getHeight(); y++)
        {
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			assert(n);
			if(isIncidentWithInterEdge(n, aMap))
			{
				addParent(n);
				if(last)
				{
					if(n->getUniqueID() != last->getUniqueID() &&
							this->isIncidentWithInterEdge(n, aMap))
					{
							addTransitionPoint(n, last, aMap, aMap->h(n, last));
							last = n;
					}
				}
				else
				{
					first = n;
					last = n;
				}
			}
        }

        // add nodes along bottom border
        y = this->getVOrigin()+this->getHeight()-1;
        for(x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
        {
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			assert(n);
			if(isIncidentWithInterEdge(n, aMap))
			{
				addParent(n);
				if(last)
				{
					if(n->getUniqueID() != last->getUniqueID() &&
							this->isIncidentWithInterEdge(n, aMap))
					{
							addTransitionPoint(n, last, aMap, aMap->h(n, last));
							last = n;
					}
				}
				else
				{
					first = n;
					last = n;
				}
			}
        }

        // don't try to add edges twice if the cluster has dimension = 1
        if(getWidth() > 1 && getHeight() > 1)
        {
			// add nodes along right border
			x = this->getHOrigin()+this->getWidth()-1;
			for(y=this->getVOrigin()+this->getHeight()-1; y>=this->getVOrigin(); y--)
			{
				ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
				assert(n);
				if(isIncidentWithInterEdge(n, aMap))
				{
					addParent(n);
					if(last)
					{
						if(n->getUniqueID() != last->getUniqueID() &&
								this->isIncidentWithInterEdge(n, aMap))
						{
								addTransitionPoint(n, last, aMap, aMap->h(n, last));
								last = n;
						}
					}
					else
					{
						first = n;
						last = n;
					}
				}
			}

			// add nodes along top border
			y = this->getVOrigin();
			for(x=this->getHOrigin()+this->getWidth()-1; x>=this->getHOrigin(); x--)
			{
				ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
				assert(n);
				if(isIncidentWithInterEdge(n, aMap))
				{
					addParent(n);
					if(last)
					{
						if(n->getUniqueID() != last->getUniqueID() &&
								this->isIncidentWithInterEdge(n, aMap))
						{
								addTransitionPoint(n, last, aMap, aMap->h(n, last));
								last = n;
						}
					}
					else
					{
						first = n;
						last = n;
					}
				}
			}

		   //  TODO: implement addParent to also add abstract nodes
		   //  if passed a node that isn't in the abstract graph
		   //
		   //
			// connect the first and last nodes to be considered
			if(first && last && first->getUniqueID() != last->getUniqueID())
			{
				addTransitionPoint(first, last, aMap, aMap->h(first, last));
			}
        }

        // frame single-node clusters
        if(getWidth() == 1 && getHeight() == 1)
        {
                ClusterNode* n = dynamic_cast<ClusterNode*>(
                                aMap->getNodeFromMap(getHOrigin(), getVOrigin()));
                ClusterNode* absn = dynamic_cast<ClusterNode*>(n->clone());
                absn->setLabelL(kAbstractionLevel, 1);
                graph* g = aMap->getAbstractGraph(1);
                g->addNode(absn);
                addParent(absn, aMap); // also creates intra-edges
                n->setLabelL(kParent, absn->getNum());
        }
}

// use this when working with 4-connected grid maps
// (see [Harabor & Botea, 2010])
void EmptyCluster::addCardinalMacroEdges(HPAClusterAbstraction *aMap)
{
//	this->print(std::cout);
//	std::cout << std::endl;
	if(getVerbose())
	{
		std::cout << "adding cardinal macro edges for cluster "<<getId()<<" origin ";
		std::cout <<"("<< getHOrigin()<<", "<<getVOrigin()<<")";
		std::cout <<" diagonal edges allowed? "<<getAllowDiagonals()<< " "<<std::endl;
	}
	graph* absg = aMap->getAbstractGraph(1);
	macro = 0;

	// connect nodes on directly opposite sides of the perimeter
	if(this->getWidth() > 1)
	{
		int lx = this->getHOrigin();
		int rx = lx + this->getWidth()-1;
		for(int y=this->getVOrigin(); y<this->getVOrigin()+this->getHeight(); y++)
		{
			node *left = absg->getNode(
					aMap->getNodeFromMap(lx, y)->getLabelL(kParent));
			node *right = absg->getNode(
					aMap->getNodeFromMap(rx, y)->getLabelL(kParent));
			if(left && right)
				addSingleMacroEdge(left, right, aMap->h(left, right), absg);
		}
	}

	if(this->getHeight() > 1)
	{
		int ty = this->getVOrigin();
		int by = this->getVOrigin()+this->getHeight()-1;
		for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
		{
			node *top = absg->getNode(
					aMap->getNodeFromMap(x, ty)->getLabelL(kParent));
			node *bottom = absg->getNode(
					aMap->getNodeFromMap(x, by)->getLabelL(kParent));
			if(top && bottom)
				addSingleMacroEdge(top, bottom, aMap->h(top, bottom), absg);

		}
	}

	// connect entrances on directly opposite sides
	addMacroEdgesBetweenTopAndBottomEntrances(aMap);
	addMacroEdgesBetweenLeftRightEntrances(aMap);

	if(getVerbose())
		std::cout << macro << " macro edges added for cluster "<<getId()<<std::endl;
}


void
EmptyCluster::addDiagonalMacroEdges(HPAClusterAbstraction* aMap)
{
	graph* absg = aMap->getAbstractGraph(1);
	macro = 0;

	// first, add diagonal edges between nodes on orthogonal sides of the 
	// cluster
	int max = this->getHeight() > this->getWidth()?
		this->getWidth():this->getHeight();
	for(int offset=1; offset<max; offset++)
	{
		// connect left and bottom sides of cluster
		int fx = this->getHOrigin()+offset;
		int fy = this->getVOrigin();
		int sx = this->getHOrigin();
		int sy = this->getVOrigin()+offset;
		node *first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));
		node *second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));
		if(first && second)
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		else if(!first && second)
		{
			first = nextNodeInRow(fx, fy, aMap, true);
			if(first)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
		else if(!second && first)
		{
			second = nextNodeInColumn(sx, sy, aMap, true);
			if(second)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}

		// connect top and right sides of cluster
		fx = this->getHOrigin()+this->getWidth()-1-offset;
		sx = this->getHOrigin()+this->getWidth()-1;
		first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));
		second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));
		if(first && second)
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		else if(!first && second)
		{
			first = nextNodeInRow(fx, fy, aMap, false);
			if(first)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
		else if(first && !second)
		{
			second = nextNodeInColumn(sx, sy, aMap, true);
			if(second)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}

		// connect left and bottom sides of cluster
		fx = this->getHOrigin()+offset;
		fy = this->getVOrigin()+getHeight()-1;
		sx = this->getHOrigin();
		sy = this->getVOrigin()+this->getHeight()-1-offset;
		first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));
		second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));
		if(first && second)
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		else if(!first && second)
		{
			first = nextNodeInRow(fx, fy, aMap, true);
			if(first)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
		else if(first && !second)
		{
			second = nextNodeInColumn(sx, sy, aMap, false);
			if(second)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}

		// connect bottom and right sides sides of cluster
		fx = this->getHOrigin()+this->getWidth()-1;
		fy = this->getVOrigin()+getHeight()-1-offset;
		sx = this->getHOrigin()+this->getWidth()-1-offset;
		sy = this->getVOrigin()+this->getHeight()-1;
		first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));
		second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));
		if(first && second) 
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		else if(!first && second)
		{
			first = nextNodeInColumn(fx, fy, aMap, false);
			if(first)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
		else if(first && !second)
		{
			second = nextNodeInRow(sx, sy, aMap, false);
			if(second)
				addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
	}

}

void
EmptyCluster::addDiagonalFanMacroEdges(HPAClusterAbstraction* aMap)
{
	graph* absg = aMap->getAbstractGraph(1);
	macro = 0;

	// add edges connecting nodes on the top side to nodes on the bottom 
	// side of the cluster
	int max = this->getHeight() > this->getWidth()?
		this->getWidth():this->getHeight();
	for(int fx=this->getHOrigin(); 
			fx<this->getHOrigin()+this->getWidth(); 
			fx++)
	{
		int fy = this->getVOrigin();
		node* first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));

		if(!first) continue;

		int minx = (fx-(max-1))<this->getHOrigin()?
			(this->getHOrigin()):(fx-(max-1)); 
		int maxx = (fx+max)>(this->getHOrigin()+this->getWidth())?
			(this->getHOrigin()+this->getWidth()):(fx+max);
		for(int sx = minx; sx < maxx; sx++)
		{
			int sy = this->getVOrigin()+this->getHeight()-1;
			node* second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));

			if(!second) continue;
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
	}

	// add edges connecting nodes on the left side to nodes on the right 
	// side of the cluster
	for(int fy=this->getVOrigin();
			fy<this->getVOrigin()+this->getHeight();
			fy++)
	{
		int fx = this->getHOrigin();
		node* first = absg->getNode(
				aMap->getNodeFromMap(fx, fy)->getLabelL(kParent));

		if(!first) continue;

		int miny = (fy-(max-1))<this->getVOrigin()?
			(this->getVOrigin()):(fy-(max-1)); 
		int maxy = (fy+max)>(this->getVOrigin()+this->getHeight())?
			(this->getVOrigin()+this->getHeight()):(fy+max);
		for(int sy = miny; sy < maxy; sy++) 
		{
			int sx = this->getHOrigin()+this->getWidth()-1;
			node* second = absg->getNode(
				aMap->getNodeFromMap(sx, sy)->getLabelL(kParent));

			if(!second) continue;
			addSingleMacroEdge(first, second, aMap->h(first, second), absg);
		}
	}
}

void 
EmptyCluster::addMacroEdgesBetweenLeftRightEntrances(
		HPAClusterAbstraction* hpamap)
{
	// connect entrances along the left of the cluster to entrances along
	// the right.
	graph* absg = hpamap->getAbstractGraph(1);
	int ly = this->getVOrigin();
	while(ly < (this->getVOrigin()+this->getHeight()))
	{
		int lx = this->getHOrigin();
		int rx = this->getHOrigin()+this->getWidth()-1;
		int length = findVerticalLength(lx, ly, hpamap);
		//std::cout << "@ :"<<lx<<", "<<ly<< " found vent of length "<<length;
		//std::cout << std::endl;

		if(length == 0)
		{
			ly++;
			continue;
		}

		int rymin = (ly - (this->getWidth()-1));
		if(rymin >= this->getVOrigin())
		{
			//std::cout << "connecting top... lx: "<<lx<<"ly: "<<ly;
			//std::cout << " rymin: "<<rymin<<" width-1: "<<this->getWidth()-1;
			//std::cout << " wtf: "<<ly - this->getWidth()-1;
			//std::cout << std::endl;

			for(int ry = rymin; ry >= this->getVOrigin(); ry--)
			{
			//	std::cout << "ry: "<<ry<<" rx: "<<rx<<std::endl;
				node* absNeighbour = 0;
				assert(hpamap->getNodeFromMap(rx, ry));
				absNeighbour = absg->getNode(hpamap->getNodeFromMap(rx, 
							ry)->getLabelL(kParent));

				if(absNeighbour)
				{
					assert(hpamap->getNodeFromMap(rx, ry)->getLabelL(kParent) 
							!= -1);
					assert((int)absNeighbour->getNum() < absg->getNumNodes());
					node* absNode = absg->getNode(
							hpamap->getNodeFromMap(lx, ly)->getLabelL(kParent));
					addSingleMacroEdge(absNode, absNeighbour, 
							hpamap->h(absNode, absNeighbour), absg);
					break;
				}
			}
		}

		rymin = (ly + length-1)+(this->getWidth()-1);
		if(rymin < (this->getVOrigin()+this->getHeight()))
		{
			//std::cout << "connecting bottom..."<<std::endl;
			//std::cout << " rymin: "<<rymin<<" width-1: "<<this->getWidth()-1;
			//std::cout << std::endl;
			for(int ry = rymin; 
					ry < (this->getVOrigin()+this->getHeight());
					ry++)
			{
			//	std::cout << "ry: "<<ry<<" rx: "<<rx<<std::endl;
				node* absNeighbour = 0;
				assert(hpamap->getNodeFromMap(rx, ry));
				absNeighbour = absg->getNode(hpamap->getNodeFromMap(rx, 
							ry)->getLabelL(kParent));

				if(absNeighbour)
				{
					assert(hpamap->getNodeFromMap(rx, ry)->getLabelL(kParent) != 
							-1);
					assert((int)absNeighbour->getNum() < absg->getNumNodes());
					node* absNode = absg->getNode(hpamap->getNodeFromMap(lx, 
								(ly+length-1))->getLabelL(kParent));
					addSingleMacroEdge(absNode, absNeighbour, 
							hpamap->h(absNode, absNeighbour), absg);
					break;
				}

			}
		}

		ly += length;
	}
}

void 
EmptyCluster::addMacroEdgesBetweenTopAndBottomEntrances(
		HPAClusterAbstraction* hpamap)
{
	// connect entrances along the top of the cluster to entrances along
	// the bottom.
	graph* absg = hpamap->getAbstractGraph(1);
	int tx = this->getHOrigin();
	while(tx < (this->getHOrigin()+this->getWidth()))
	{
		int ty = this->getVOrigin();
		int by = this->getVOrigin()+this->getHeight()-1;
		int length = findHorizontalLength(tx, ty, hpamap);
		//std::cout << "@ :"<<tx<<", "<<ty<< " found hent of length "<<length;
		//std::cout << std::endl;

		if(length == 0)
		{
			tx++;
			continue;
		}

		int bxmin = tx - (this->getHeight()-1);
		if(bxmin > this->getHOrigin())
		{
			//std::cout << "connecting left... tx: "<<tx<<"ty: "<<ty;
			//std::cout << " bxmin: "<<bxmin<<" height-1: "<<this->getHeight()-1;
			//std::cout << std::endl;

			for(int bx = bxmin; bx >= this->getHOrigin(); bx--)
			{
				//std::cout << "by: "<<by<<" bx: "<<bx;
				node* absNeighbour = 0;
				assert(hpamap->getNodeFromMap(bx, by));
				absNeighbour = absg->getNode(hpamap->getNodeFromMap(bx, 
							by)->getLabelL(kParent));

				if(absNeighbour)
				{
					//std::cout << " exists!"<<std::endl;
					assert(hpamap->getNodeFromMap(bx, by)->getLabelL(kParent) 
							!= -1);
					assert((int)absNeighbour->getNum() < absg->getNumNodes());
					node* absNode = absg->getNode(
							hpamap->getNodeFromMap(tx, ty)->getLabelL(kParent));
					addSingleMacroEdge(absNode, absNeighbour, 
							hpamap->h(absNode, absNeighbour), absg);
					break;
				}
				//std::cout << std::endl;
			}
		}

		bxmin = (tx + length-1)+(this->getHeight()-1);
		if(bxmin < (this->getHOrigin()+this->getWidth()))
		{
			//std::cout << "connecting right... tx: "<<tx<<"ty: "<<ty;
			//std::cout << " bxmin: "<<bxmin<<" height-1: "<<this->getHeight()-1;
			//std::cout << std::endl;
			for(int bx = bxmin; 
					bx < (this->getHOrigin()+this->getWidth());
					bx++)
			{
				//std::cout << "by: "<<by<<" bx: "<<bx;
				node* absNeighbour = 0;
				assert(hpamap->getNodeFromMap(bx, by));
				absNeighbour = absg->getNode(hpamap->getNodeFromMap(bx, 
							by)->getLabelL(kParent));

				if(absNeighbour)
				{
					//std::cout << " exists!"<<std::endl;
					assert(hpamap->getNodeFromMap(bx, by)->getLabelL(kParent) 
							!= -1);
					assert((int)absNeighbour->getNum() < absg->getNumNodes());
					node* absNode = absg->getNode(hpamap->getNodeFromMap(
								(tx+length-1), ty)->getLabelL(kParent));
					addSingleMacroEdge(absNode, absNeighbour, 
							hpamap->h(absNode, absNeighbour), absg);
					break;
				}
				//std::cout << std::endl;

			}
		}

		tx += length;
	}
}

// use this when working with 8-connected grid maps
void EmptyCluster::addMacroEdges(HPAClusterAbstraction *aMap)
{
	if(getVerbose())
	{
		std::cout << "adding macro edges for cluster "<<getId()<<" origin ";
		std::cout <<"("<< getHOrigin()<<", "<<getVOrigin()<<")";
		std::cout <<" diagonal edges allowed? "<<getAllowDiagonals()<< " "<<std::endl;
	}

	if(this->getHeight() > 1 && this->getWidth() > 1)
	{
		addDiagonalMacroEdges(aMap);
		addDiagonalFanMacroEdges(aMap);
		addMacroEdgesBetweenTopAndBottomEntrances(aMap);
		addMacroEdgesBetweenLeftRightEntrances(aMap);
	}

	if(getVerbose())
		std::cout << macro << " macro edges added for cluster "<<getId()<<std::endl;
}

// returns true if node n_ has a neighbour with a different parent cluster. 
bool EmptyCluster::isIncidentWithInterEdge(node* n_, HPAClusterAbstraction* hpamap)
{
	bool retVal = false;
	MacroNode* n = dynamic_cast<MacroNode*>(n_);
	assert(n);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);
	for(int nbx = nx-1; nbx < nx+2; nbx++)
		for(int nby = ny-1; nby < ny+2; nby++)
		{
			MacroNode* nb = dynamic_cast<MacroNode*>(
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

void EmptyCluster::addSingleMacroEdge(node* from_, node* to_, double weight, graph* absg)
{
	assert(from_ && to_);
	MacroNode* from = dynamic_cast<MacroNode*>(from_);
	MacroNode* to = dynamic_cast<MacroNode*>(to_);


	assert(from && to);
	assert(from->getParentClusterId() == to->getParentClusterId());

	edge* e = absg->findEdge(from->getNum(), to->getNum());
	if(e == 0 && from->getParentClusterId() == to->getParentClusterId())
	{
		e = new MacroEdge(from->getNum(), to->getNum(), weight);
		absg->addEdge(e);
		macro++;
		if(getVerbose())
		{
			std::cout << "added macro edge: [("<<from->getLabelL(kFirstData)<<", ";
			std::cout <<from->getLabelL(kFirstData+1)<<") <-> (";
			std::cout << to->getLabelL(kFirstData) << ", ";
			std::cout << to->getLabelL(kFirstData+1);
			std::cout <<") wt: "<<weight<< " ] "<<std::endl;
		}
	}

}

void EmptyCluster::extend(HPAClusterAbstraction* aMap)
{
	setWidth(1);
	setHeight(1);
	Map* themap = aMap->getMap();
	int hsize=0;
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<themap->getMapWidth(); x++)
	{
		ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
		if(!n || n->getParentClusterId() != -1)
			break;
		hsize++;
	}
	
	this->setWidth(hsize);
	int vsize=0;
	for( ; y<themap->getMapHeight(); y++)
	{
		bool rowok=true;
		for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
		{
			ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			if(getVerbose())
			{
				if(n)
					std::cout << "("<<x<<", "<<y<<") is ok!"<<std::endl;
			}
			if(!n || n->getParentClusterId() != -1)
			{
				rowok=false;
			}
		}
		if(!rowok)
			break;
		vsize++;
	}
	this->setHeight(vsize);

	initOpenGLCoordinates(aMap);
}

void EmptyCluster::extend(HPAClusterAbstraction* aMap, int** clearance)
{
	ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(getHOrigin(), getVOrigin()));
	if(!n || n->getParentClusterId() != -1)
	{
		setHeight(0);
		setWidth(0);
		std::cout << "WARNING!! aborting cluster creation; origin is invalid or already assigned!!"<<std::endl;
		return;
	}

	setWidth(1);
	setHeight(1);
	//std::cout << "cluster: "<<getHOrigin()<<", "<<getVOrigin()<<" ht: "<<getHeight()<<" wt: "<<getWidth()<<std::endl;
	while(canExtendClearanceSquare(aMap))
	{
		setHeight(getHeight() + 1);
		setWidth(getWidth() + 1);
	}
	//std::cout << "csq done; height: "
	//	<<getHeight()<<" width: "<<getWidth()<<std::endl;


	while(canExtendHorizontally(aMap))
	{
		setWidth(getWidth() + 1);
	//	std::cout << " extending horizontally!"<<std::endl;
	}

	while(canExtendVertically(aMap))
	{
		setHeight(getHeight() + 1);
	//	std::cout << " extending vertically!"<<std::endl;
	}

	initOpenGLCoordinates(aMap);
}

void EmptyCluster::initOpenGLCoordinates(HPAClusterAbstraction* aMap)
{
	Map* map = aMap->getMap();
	GLdouble xx, yy, zz,rr;

	map->getOpenGLCoord(this->getHOrigin(), this->getVOrigin(), xx, yy, zz, rr);
	glx = xx;
	gly = yy;
	glz = zz-rr*0.5;

	map->getOpenGLCoord(this->getHOrigin()+this->getWidth()-1, 
			this->getVOrigin(), xx, yy, zz, rr);
	glWidth = xx - glx;

	map->getOpenGLCoord(this->getHOrigin(), this->getVOrigin()+this->getHeight()-1, 
			xx, yy, zz, rr);
	glHeight = yy - gly;

	glx-=rr;
	gly-=rr;
	glHeight+=2*rr;
	glWidth+=2*rr;
}

void EmptyCluster::openGLDraw()
{
//	glColor3f (0.6F, 0.9F, 0.4F);
	glColor3f (0.0F, 0.6F, 0.0F);
	//glColor3f(0.1, 0.1, 0.7);
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(glx, gly, glz);
	glVertex3f(glx+glWidth, gly, glz);
	glVertex3f(glx+glWidth, gly+glHeight, glz);
	glVertex3f(glx, gly+glHeight, glz);
	glVertex3f(glx, gly, glz);
	glEnd();
	glLineWidth(1.0f);
}

void EmptyCluster::connectParent(node*, HPAClusterAbstraction*)
{
}

// identifies entrances between horizontally adjacent clusters.
// an abstract node is added for each node in the entrance area
// and an abstract edge is added for each (straight) edge in the entrance area 
// connecting the two clusters.
//
// if 'getAllowDiagonals()' is set we also add an abstract edge for each diagonal
// edge in the entrance area between the two clusters
void EmptyCluster::processHorizontalEntrance(
		HPAClusterAbstraction* hpamap, int x, int y, int length)
{
//	if(getVerbose())
//	{
//		std::cout << "processing hEnts for ";
//		print(std::cout);
//		std::cout << std::endl;
//	}

	for(int xprime=x; xprime <x+length; xprime++)
	{	
		node* endpoint1 = hpamap->getNodeFromMap(xprime, y); 
		node* endpoint2 = hpamap->getNodeFromMap(xprime, y-1);
		this->addTransitionPoint(endpoint1, endpoint2, hpamap);
	}

	if(getAllowDiagonals())
	{
		// first set of diagonals
		for(int xprime=x+1; xprime <x+length; xprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(xprime, y); 
			node* endpoint2 = hpamap->getNodeFromMap(xprime-1, y-1);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<xprime<<","<<y<<") <-> ("
				"" << xprime-1<<", "<<y-1<<")"<<std::endl;
			}
		}

		// second set of diagonals
		for(int xprime=x+1; xprime <x+length; xprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(xprime, y-1); 
			node* endpoint2 = hpamap->getNodeFromMap(xprime-1, y);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<xprime<<","<<y-1<<") <-> ("
				"" << xprime-1<<", "<<y<<")"<<std::endl;
			}
		}
	}

}

void EmptyCluster::buildHorizontalEntrances(HPAClusterAbstraction* hpamap)
{
	int mapheight = hpamap->getMap()->getMapHeight();
	int y = this->getVOrigin()+this->getHeight();
	if(y == mapheight)
		return; 

	// scan for vertical entrances along the eastern border 
	int x = this->getHOrigin();
	while(x < this->getHOrigin()+this->getWidth())
	{
		int length = findHorizontalEntranceLength(x,y, hpamap);
	
		if(length == 0)
			x++;
		else
		{
			// add inter-edges to connect adjacent clusters 
			processHorizontalEntrance(hpamap, x, y, length);
			x += length;
		}
	}
}

// each node in the veritcal entrance area is represented by a node in 
// the abstract graph. 
// further, each (straight) edge in the entrance area, connecting the two
// adjacent clusters, is represented by a node in the abstract graph.
//
// if 'getAllowDiagonals()' is true, every diagonal edge connecting the two
// adjacent clusters is represented by an edge in the abstract graph.
void EmptyCluster::processVerticalEntrance(
		HPAClusterAbstraction* hpamap, int x, int y, int length)
{
//	if(getVerbose())
//	{
//		std::cout << "processing vEnts for ";
//		print(std::cout);
//		std::cout << std::endl;
//	}

	for(int yprime=y; yprime <y+length; yprime++)
	{
		node* endpoint1 = hpamap->getNodeFromMap(x, yprime); 
		node* endpoint2 = hpamap->getNodeFromMap(x-1, yprime);
		this->addTransitionPoint(endpoint1, endpoint2, hpamap);
	}

	if(getAllowDiagonals())
	{
		// first set of diagonals
		for(int yprime=y+1; yprime <y+length; yprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(x, yprime); 
			node* endpoint2 = hpamap->getNodeFromMap(x-1, yprime-1);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<x<<","<<yprime<<") <-> ("
				"" << x-1<<","<<yprime-1<<")"<<std::endl;
			}
		}

		// second set of diagonals
		for(int yprime=y+1; yprime <y+length; yprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(x-1, yprime); 
			node* endpoint2 = hpamap->getNodeFromMap(x, yprime-1);
			assert(endpoint1 && endpoint2);
				
			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<x-1<<","<<yprime<<") <-> ("
				"" << x<<","<<yprime-1<<")"<<std::endl;
			}
		}
	}
	
}

void EmptyCluster::buildVerticalEntrances(HPAClusterAbstraction* hpamap)
{
	int mapwidth = hpamap->getMap()->getMapWidth();
	int x = this->getHOrigin()+this->getWidth();
	if(x == mapwidth)
		return; 

	// scan for vertical entrances along the eastern border 
	int y = this->getVOrigin();
	while(y < this->getVOrigin()+this->getHeight())
	{
		int length = findVerticalEntranceLength(x,y, hpamap);
	
		if(length == 0)
			y++;
		else
		{
			processVerticalEntrance(hpamap, x, y, length);
			y += length;
		}
	}
}

bool EmptyCluster::canExtendClearanceSquare(HPAClusterAbstraction* hpamap)
{
	int x = getHOrigin() + getWidth();
	int y = getVOrigin();
	for( ; y <= getVOrigin() + getHeight(); y++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(hpamap->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}

	x = getHOrigin();
	y = getVOrigin() + getHeight();
	for( ; x <= getHOrigin() + getWidth(); x++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(hpamap->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}

	return true;
}

bool EmptyCluster::canExtendHorizontally(HPAClusterAbstraction* hpamap)
{
	int x = getHOrigin() + getWidth();
	for(int y = getVOrigin(); y<getVOrigin() + getHeight(); y++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(hpamap->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}
	return true;
}

bool EmptyCluster::canExtendVertically(HPAClusterAbstraction* hpamap)
{
	int y = getVOrigin() + getHeight();
	for(int x = getHOrigin(); x<getHOrigin() + getWidth(); x++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(hpamap->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}
	return true;

}

// returns the length of a contiguous horizontal entrance area starting at
// coordinate (x, y)
int EmptyCluster::findHorizontalLength(int x, int y, 
		HPAClusterAbstraction* hpamap)
{
	int length = 0;
	for(int xprime = x;
		   	xprime < (this->getHOrigin()+this->getWidth());
		   	xprime++)
	{
		node* n = hpamap->getNodeFromMap(xprime, y);
		if(isIncidentWithInterEdge(n, hpamap))
		{
			if(n->getLabelL(kParent) == -1)
				std::cout << " \nnode has no abs parent: "<<n->getLabelL(kFirstData)<<", "<<n->getLabelL(kFirstData+1)<<std::endl;
			assert(n->getLabelL(kParent) != -1);
			length++;
		}
		else
			break;	
	}
	return length;
}

// returns the length of a contiguous vertical entrance area starting at
// coordinate (x, y)
int EmptyCluster::findVerticalLength(int x, int y, 
		HPAClusterAbstraction* hpamap)
{
	int length = 0;
	for(int yprime = y; 
			yprime < (this->getVOrigin()+this->getHeight());
		   	yprime++)
	{
		node* n = hpamap->getNodeFromMap(x, yprime);
		if(isIncidentWithInterEdge(n, hpamap))
		{
			assert(n->getLabelL(kParent) != -1);
			length++;
		}
		else
			break;	
	}
	return length;
}

node*
EmptyCluster::nextNodeInRow(int x, int y, HPAClusterAbstraction* hpamap,
		bool leftToRight)
{
	node* retVal = 0;
	graph* absg = hpamap->getAbstractGraph(1);
	if(leftToRight)
	{
		for( ; x < (this->getHOrigin() + this->getWidth()) ; x++)
		{
			int nodeId = hpamap->getNodeFromMap(x, y)->getLabelL(kParent);
			if(nodeId != -1)
			{
				retVal = absg->getNode(nodeId);
				break;
			}
		}
	}
	else
	{
		for( ; x >= this->getHOrigin(); x--)
		{
			int nodeId = hpamap->getNodeFromMap(x, y)->getLabelL(kParent);
			if(nodeId != -1)
			{
				retVal = absg->getNode(nodeId);
				break;
			}
		}
	}

	return retVal;
}

node*
EmptyCluster::nextNodeInColumn(int x, int y, HPAClusterAbstraction* hpamap,
		bool topToBottom)
{
	node* retVal = 0;
	graph* absg = hpamap->getAbstractGraph(1);

	if(topToBottom)
	{
		for( ; y < (this->getVOrigin() + this->getHeight()); y++)
		{
			int nodeId = hpamap->getNodeFromMap(x, y)->getLabelL(kParent);
			if(nodeId != -1)
			{
				retVal = absg->getNode(nodeId);
				break;
			}
		}
	}
	else
	{
		for( ; y >= this->getVOrigin(); y--)
		{
			int nodeId = hpamap->getNodeFromMap(x, y)->getLabelL(kParent);
			if(nodeId != -1)
			{
				retVal = absg->getNode(nodeId);
				break;
			}
		}
	}

	return retVal;
}
