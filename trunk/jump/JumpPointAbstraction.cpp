#include "JumpPointAbstraction.h"

#include "IEdgeFactory.h"
#include "INodeFactory.h"
#include "graph.h"
#include "map.h"
#include "OctileHeuristic.h"

JumpPointAbstraction::JumpPointAbstraction(Map* _m, INodeFactory* _nf, 
		IEdgeFactory* _ef) : mapAbstraction(_m)
{
	nf = _nf;
	ef = _ef;
	makeJumpPointGraph();
}

JumpPointAbstraction::~JumpPointAbstraction()
{
	delete nf;
	delete ef;
}

mapAbstraction*
JumpPointAbstraction::clone(Map* _m)
{
	return new JumpPointAbstraction(_m, nf->clone(), ef->clone());
}

bool 
JumpPointAbstraction::pathable(node* n, node* m)
{
	return true;
}

void
JumpPointAbstraction::verifyHierarchy()
{
	graph* g = abstractions[0];

	if(g->getNumEdges() != 0)
	{
		repairAbstraction();
		return;
	}

	for(int i=0; i<g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		if(n->getNumIncomingEdges() != 0)
		{
			repairAbstraction();
			return;
		}
	}
}

void
JumpPointAbstraction::removeNode(node* n)
{
	graph* g = abstractions[0];

	edge_iterator it = n->getEdgeIter();
	for(edge* e = n->edgeIterNext(it); e != 0; e = n->edgeIterNext(it))
	{
		n->removeEdge(e);
	}
	g->removeNode(n);
}

void 
JumpPointAbstraction::removeEdge(edge *e, unsigned int absLevel)
{
	if(absLevel != 0)
		return;

	graph* g = abstractions[0];
	node* from = g->getNode(e->getFrom());	
	if(from == 0)
		return;

	from->removeEdge(e);
}

void 
JumpPointAbstraction::addNode(node *n)
{
	graph* g = abstractions[0];
	g->addNode(n);
}

void 
JumpPointAbstraction::addEdge(edge *e, unsigned int absLevel)
{
	if(absLevel != 0)
		return;

	graph* g = abstractions[0];
	node* from = g->getNode(e->getFrom());
	if(from == 0)
		return;

	from->addEdge(e);
}

void 
JumpPointAbstraction::repairAbstraction()
{
	graph* g = abstractions[0];
	edge_iterator it = g->getEdgeIter();
	for(edge *e = g->edgeIterNext(it); e != 0; e = g->edgeIterNext(it))
	{
		g->removeEdge(e);
		delete e;
	}
	
	for(int i=0; i < g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		edge_iterator it = n->getEdgeIter();
		for(edge* e = n->edgeIterNext(it); e != 0; e = n->edgeIterNext(it))
		{
			node* n1 = g->getNode(e->getFrom());
			node* n2 = g->getNode(e->getTo());
			n1->removeEdge(e);
			n2->removeEdge(e);
			delete e;
		}
	}

	delete g;
	abstractions.pop_back();

	makeJumpPointGraph();
}

void
JumpPointAbstraction::makeJumpPointGraph()
{
	// compute the set of nodes in the graph
	graph* g = makeMapNodes(this->getMap(), nf);
	abstractions.push_back(g);

	// compute jump point neighbours of each node 
	OctileHeuristic heuristic;
	for(int i=0; i < g->getNumNodes(); i++)
	{
		node* n = g->getNode(i);
		for(int j = 1; j<9; j++)
		{
			// connect n to a jump point in every direction
			int nx = n->getLabelL(kFirstData);
			int ny = n->getLabelL(kFirstData+1);

			node* neighbour = 0;
			switch(j)
			{
				case 1:
					neighbour = findJumpNode(Jump::N, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::N, nx, ny);
					break;
				case 2:
					neighbour = findJumpNode(Jump::NE, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::NE, nx, ny);
					break;
				case 3:
					neighbour = findJumpNode(Jump::E, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::E, nx, ny);
					break;
				case 4:
					neighbour = findJumpNode(Jump::SE, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::SE, nx, ny);
					break;
				case 5:
					neighbour = findJumpNode(Jump::S, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::S, nx, ny);
					break;
				case 6:
					neighbour = findJumpNode(Jump::SW, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::SW, nx, ny);
					break;
				case 7:
					neighbour = findJumpNode(Jump::W, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::W, nx, ny);
					break;
				case 8:
					neighbour = findJumpNode(Jump::NW, nx, ny);
					if(!neighbour)
						neighbour = findObstacleJumpNode(Jump::NW, nx, ny);
					break;
			}

			if(neighbour && neighbour->getNum() != n->getNum())
			{
					edge* e = new edge(n->getNum(), neighbour->getNum(),
							heuristic.h(n, neighbour));
					n->addEdge(e);
			}
		}
	}
}

node* 
JumpPointAbstraction::findJumpNode(Jump::Direction d, int x, int y)
{
	const int jumplimit = INT_MAX;

	node* n = this->getNodeFromMap(x, y);

	switch(d)
	{
		case Jump::N:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y-steps;
				n = this->getNodeFromMap(x, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				// n is a jump node if we cannot prove a shorter path to 
				// a diagonal neighbour exists
				if(!this->getNodeFromMap(x-1, ny) && 
						this->getNodeFromMap(x-1, ny-1))
				{
					break;
				}

				if(!this->getNodeFromMap(x+1, ny) &&
						this->getNodeFromMap(x+1, ny-1))
				{
					break;
				}
				
			}
			break;
		}

		case Jump::S:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y+steps;
				n = this->getNodeFromMap(x, ny);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				if(!this->getNodeFromMap(x-1, ny) && 
						this->getNodeFromMap(x-1, ny+1))
				{
					break;
				}

				if(!this->getNodeFromMap(x+1, ny) &&
						this->getNodeFromMap(x+1, ny+1))
				{
					break;
				}
			}
			break;
		}

		case Jump::E:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				n = this->getNodeFromMap(nx, y);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				if(!this->getNodeFromMap(nx, y-1) && 
						this->getNodeFromMap(nx+1, y-1))
				{
					break;
				}

				if(!this->getNodeFromMap(nx, y+1) &&
						this->getNodeFromMap(nx+1, y+1))
				{
					break;
				}
			}
			break;
		}

		case Jump::W:
		{

			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				n = this->getNodeFromMap(nx, y);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
					break;
				
				if(!this->getNodeFromMap(nx, y-1) && 
						this->getNodeFromMap(nx-1, y-1))
				{
					break;
				}

				if(!this->getNodeFromMap(nx, y+1) &&
						this->getNodeFromMap(nx-1, y+1))
				{
					break;
				}
			}
			break;
		}

		case Jump::NE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y-steps;
				n = this->getNodeFromMap(nx, ny);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				// n is a jump node if a SE neighbour exists which cannot be
				// reached by a shorter path than one involving n.
				if(!this->getNodeFromMap(nx, ny+1) && 
						this->getNodeFromMap(nx+1, ny+1))
				{
					break;
				}

				// n is a jump node if a NW neighbour exists which cannot be
				// reached by a shorter path than one involving n.
				if(!this->getNodeFromMap(nx-1, ny) && 
						this->getNodeFromMap(nx-1, ny-1))
				{
					break;
				}
			
				// n is a jump node if we can reach other jump nodes by
				// travelling vertically or horizontally 
				if(findJumpNode(Jump::N, nx, ny) || 
						findJumpNode(Jump::E, nx, ny))
					break;
			}
			break;
		}

		case Jump::SE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y+steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				if(!this->getNodeFromMap(nx, ny-1) && 
						this->getNodeFromMap(nx+1, ny-1))
				{
					break;
				}

				if(!this->getNodeFromMap(nx-1, ny) && 
						this->getNodeFromMap(nx-1, ny+1))
				{
					break;
				}

				if(findJumpNode(Jump::S, nx, ny) || 
						findJumpNode(Jump::E, nx, ny))
					break;
			}
			break;
		}

		case Jump::NW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps; 
				int ny = y-steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				if(!this->getNodeFromMap(nx, ny+1) && 
						this->getNodeFromMap(nx-1, ny+1))
				{
					break;
				}

				if(!this->getNodeFromMap(nx+1, ny) && 
						this->getNodeFromMap(nx+1, ny-1))
				{
					break;
				}

				if(findJumpNode(Jump::N, nx, ny) || 
						findJumpNode(Jump::W, nx, ny))
					break;
			}
			break;
		}

		case Jump::SW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				int ny = y+steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
					break;

				if(!this->getNodeFromMap(nx, ny-1) && 
						this->getNodeFromMap(nx-1, ny-1))
				{
					break;
				}

				if(!this->getNodeFromMap(nx+1, ny) && 
						this->getNodeFromMap(nx+1, ny+1))
				{
					break;
				}

				if(findJumpNode(Jump::S, nx, ny) || 
						findJumpNode(Jump::W, nx, ny))
					break;
			}
			break;
		}
	}

	return n;
}


// Sometimes ::findJumpNode fails to return a jump node because it reaches
// an obstacle. This method returns the node immediately prior to the
// obstacle and designates it a jump point.
node*
JumpPointAbstraction::findObstacleJumpNode(Jump::Direction d, int x, int y)
{
	const int jumplimit = INT_MAX;
	node* n = 0; 

	switch(d)
	{
		case Jump::N:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y-steps;
				n = this->getNodeFromMap(x, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x, y-(steps-1));
					break;
					
				}
			}
			break;
		}

		case Jump::S:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y+steps;
				n = this->getNodeFromMap(x, ny);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x, y+(steps-1));
					break;
					
				}
			}
			break;
		}

		case Jump::E:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				n = this->getNodeFromMap(nx, y);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x+(steps-1), y);
					break;
				}
			}
			break;
		}

		case Jump::W:
		{

			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				n = this->getNodeFromMap(nx, y);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x-(steps-1), y);
					break;
				}
			}
			break;
		}

		case Jump::NE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y-steps;
				n = this->getNodeFromMap(nx, ny);
				
				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x+(steps-1), y-(steps-1));
					break;
					
				}
			}
			break;
		}

		case Jump::SE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y+steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x+(steps-1), y+(steps-1));
					break;
					
				}
			}
			break;
		}

		case Jump::NW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps; 
				int ny = y-steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x-(steps-1), y-(steps-1));
					break;
				}
			}
			break;
		}

		case Jump::SW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				int ny = y+steps;
				n = this->getNodeFromMap(nx, ny);

				// the node just before an obstacle is a jump point
				if(n == 0)
				{
					n = this->getNodeFromMap(x-(steps-1), y+(steps-1));
					break;
				}
			}
			break;
		}
	}

	return n;
}

