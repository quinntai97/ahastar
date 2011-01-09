#include "JumpPointsExpansionPolicy.h"
#include "Heuristic.h"
#include "mapAbstraction.h"
#include "ProblemInstance.h"

JumpPointsExpansionPolicy::JumpPointsExpansionPolicy()
	: ExpansionPolicy()
{
	jumplimit = INT_MAX;
}

JumpPointsExpansionPolicy::~JumpPointsExpansionPolicy()
{
	neighbours.clear();
}


void JumpPointsExpansionPolicy::expand(node* t) throw(std::logic_error)
{
	ExpansionPolicy::expand(t);

	x = t->getLabelL(kFirstData);
	y = t->getLabelL(kFirstData+1);
	goalx = problem->getGoalNode()->getLabelL(kFirstData);
	goaly = problem->getGoalNode()->getLabelL(kFirstData+1);

	neighbours.clear();
	computeNeighbourSet();

	neighbourIndex = 0;
}

void JumpPointsExpansionPolicy::computeNeighbourSet()
{
	mapAbstraction* map = problem->getMap();

	if(target->backpointer != 0)
	{
		JPEP::Direction which = directionToParent(target->backpointer);
		switch(which)
		{
			case JPEP::N:
			{
				addJumpNodes(JPEP::S);

				// check if we also need to add diagonal neighbours
				if(!map->getNodeFromMap(x+1, y))
				{
					node* n = map->getNodeFromMap(x+1, y+1);
					if(n)
						neighbours.push_back(n);
				}
				if(!map->getNodeFromMap(x-1, y))
				{
					node* n = map->getNodeFromMap(x-1, y+1);
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::NE:
			{
				addJumpNodes(JPEP::SW);

				// add NW neighbour only if N neighbour is null
				if(!map->getNodeFromMap(x, y-1))
				{
					node* n = map->getNodeFromMap(x-1, y-1);
					if(n)
						neighbours.push_back(n);
				}

				// add SE neighbour only if E neighbour is null
				if(!map->getNodeFromMap(x+1, y))
				{
					node* n = map->getNodeFromMap(x+1, y+1); 
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::E:
			{
				addJumpNodes(JPEP::W);

				// check if we also need to add diagonal neighbours
				if(!map->getNodeFromMap(x, y-1))
				{
					node* n = map->getNodeFromMap(x-1, y-1);
					if(n)
						neighbours.push_back(n);
				}
				if(!map->getNodeFromMap(x, y+1))
				{
					node* n = map->getNodeFromMap(x-1, y+1);
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::SE:
			{
				addJumpNodes(JPEP::NW); 

				// add SW neighbour only if S neighbour is null
				if(!map->getNodeFromMap(x, y+1))
				{
					node* n = map->getNodeFromMap(x-1, y+1);
					if(n)
						neighbours.push_back(n);
				}

				// add NE neighbour only if E neighbour is null
				if(!map->getNodeFromMap(x+1, y))
				{
					node* n = map->getNodeFromMap(x+1, y-1); 
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::S:
			{
				addJumpNodes(JPEP::N);

				// check if we also need to add diagonal neighbours
				if(!map->getNodeFromMap(x+1, y))
				{
					node* n = map->getNodeFromMap(x+1, y-1);
					if(n)
						neighbours.push_back(n);
				}
				if(!map->getNodeFromMap(x-1, y))
				{
					node* n = map->getNodeFromMap(x-1, y-1);
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::SW:
			{
				addJumpNodes(JPEP::NE); 

				// add SE neighbour only if S neighbour is null
				if(!map->getNodeFromMap(x, y+1))
				{
					node* n = map->getNodeFromMap(x+1, y+1);
					if(n)
						neighbours.push_back(n);
				}

				// add NW neighbour only if W neighbour is null
				if(!map->getNodeFromMap(x-1, y))
				{
					node* n = map->getNodeFromMap(x-1, y-1); 
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::W:
			{
				addJumpNodes(JPEP::E);

				// check if we also need to add diagonal neighbours
				if(!map->getNodeFromMap(x, y-1))
				{
					node* n = map->getNodeFromMap(x+1, y-1);
					if(n)
						neighbours.push_back(n);
				}
				if(!map->getNodeFromMap(x, y+1))
				{
					node* n = map->getNodeFromMap(x+1, y+1);
					if(n)
						neighbours.push_back(n);
				}
				break;
			}

			case JPEP::NW:
			{
				addJumpNodes(JPEP::SE);

				// add NE neighbour only if N neighbour is null
				if(!map->getNodeFromMap(x, y-1))
				{
					node* n = map->getNodeFromMap(x+1, y-1);
					if(n)
						neighbours.push_back(n);
				}

				// add SW neighbour only if W neighbour is null
				if(!map->getNodeFromMap(x-1, y))
				{
					node* n = map->getNodeFromMap(x-1, y+1); 
					if(n)
						neighbours.push_back(n);
				}
				break;
			}
		}
	}
}

node* JumpPointsExpansionPolicy::first()
{
	if(neighbours.size() > 0)
		return neighbours.at(0);
	return 0;
}

node* JumpPointsExpansionPolicy::next()
{
	node* nextnode = 0;

	if(hasNext())
	{
		neighbourIndex++;
		nextnode = n();
	}

	return nextnode;
}

node* JumpPointsExpansionPolicy::n()
{
	node* retVal = 0;
	unsigned int numNeighbours = neighbours.size();
	if(numNeighbours > 0 && neighbourIndex < numNeighbours)
	{
		retVal = neighbours.at(neighbourIndex);
	}
	return retVal;
}

double JumpPointsExpansionPolicy::cost_to_n()
{
	node* current = n();
	return problem->getHeuristic()->h(target, current);
}

bool JumpPointsExpansionPolicy::hasNext()
{
	if(neighbourIndex+1 < neighbours.size())
		return true;
	return false;
}


JPEP::Direction JumpPointsExpansionPolicy::directionToParent(node* n)
{
	node* parent = target->backpointer;	
	int px = parent->getLabelL(kFirstData);
	int py = parent->getLabelL(kFirstData+1);
	
	if(py == y)
	{
		if(px > x)
			return JPEP::E;
		else
			return JPEP::W;
	}

	if(py < y)
	{
		if(px < x)
			return JPEP::NW;
		else if(px > x)
			return JPEP::NE;
		else
			return JPEP::N;

	}

	if(py > y)
	{
		if(px < x)
			return JPEP::SW;
		else if(px > x)
			return JPEP::SE;
		else
			return JPEP::S;
	}

	throw std::logic_error("JumpPointsExpansionPolicy::directionToParent"
			" failed to determine direction to parent!");
}

// Populates the set of neighbours associated with the target node with
// reachable jump points in the Direction d.
//
// @return: true if a jump node was found, otherwise false
bool
JumpPointsExpansionPolicy::addJumpNodes(JPEP::Direction d)
{
	node* n = 0; // jump node in Direction d
	mapAbstraction* map = problem->getMap();
	switch(d)
	{
		case JPEP::N:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y-steps;
				n = map->getNodeFromMap(x, ny);
				if(n == 0)
					break;

				// (ny == goaly) implies n is a jump node 
				if(ny == goaly)
					break;

				// n is a jump node if we cannot prove a shorter path to 
				// a diagonal neighbour exists
				if(!map->getNodeFromMap(x-1, ny) && 
						map->getNodeFromMap(x-1, ny-1))
				{
					break;
				}

				if(!map->getNodeFromMap(x+1, ny) &&
						map->getNodeFromMap(x+1, ny-1))
				{
					break;
				}
			}
			break;
		}

		case JPEP::S:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int ny = y+steps;
				n = map->getNodeFromMap(x, ny);
				if(n == 0)
					break;

				// (ny == goaly) implies n is a jump node 
				if(ny == goaly)
					break;

				// n is a jump node if we cannot prove a shorter path to 
				// a diagonal neighbour exists
				if(!map->getNodeFromMap(x-1, ny) && 
						map->getNodeFromMap(x-1, ny+1))
				{
					break;
				}

				if(!map->getNodeFromMap(x+1, ny) &&
						map->getNodeFromMap(x+1, ny+1))
				{
					break;
				}
			}
			break;
		}

		case JPEP::E:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				n = map->getNodeFromMap(nx, y);
				if(n == 0)
					break;

				// (nx == goalx) implies n is a jump node 
				if(nx == goalx)
					break;

				// n is a jump node if we cannot prove a shorter path to 
				// a diagonal neighbour exists
				if(!map->getNodeFromMap(nx, y-1) && 
						map->getNodeFromMap(nx+1, y-1))
				{
					break;
				}

				if(!map->getNodeFromMap(nx, y+1) &&
						map->getNodeFromMap(nx+1, y+1))
				{
					break;
				}
			}
			break;
		}

		case JPEP::W:
		{

			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				n = map->getNodeFromMap(nx, y);
				if(n == 0)
					break;

				// (nx == goalx) implies n is a jump node 
				if(nx == goalx)
					break;

				// n is a jump node if we cannot prove a shorter path to 
				// a diagonal neighbour exists
				if(!map->getNodeFromMap(nx, y-1) && 
						map->getNodeFromMap(nx-1, y-1))
				{
					break;
				}

				if(!map->getNodeFromMap(nx, y+1) &&
						map->getNodeFromMap(nx-1, y+1))
				{
					break;
				}
			}
			break;
		}

		case JPEP::NE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y-steps;
				n = map->getNodeFromMap(nx, ny);
				if(n == 0)
					break;

				// continue diagonally only if there are no horizontal
				// or vertical jump points reachable from n
				if(addJumpNodes(JPEP::N))
				{
					addJumpNodes(JPEP::E);
					break;
				}

				if(addJumpNodes(JPEP::E))
					break;

				if(nx == goalx || ny == goaly)
					break;

			}
			break;
		}

		case JPEP::SE:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x+steps;
				int ny = y+steps;
				n = map->getNodeFromMap(nx, ny);
				if(n == 0)
					break;

				if(addJumpNodes(JPEP::S))
				{
					addJumpNodes(JPEP::E);
					break;
				}

				if(addJumpNodes(JPEP::E))
					break;

				if(nx == goalx || ny == goaly)
					break;
			}
			break;
		}

		case JPEP::NW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps; 
				int ny = y-steps;
				n = map->getNodeFromMap(nx, ny);
				if(n == 0)
					break;

				if(addJumpNodes(JPEP::N))
				{
					addJumpNodes(JPEP::W);
					break;
				}

				if(addJumpNodes(JPEP::W))
					break;

				if(nx == goalx || ny == goaly)
					break;
			}
			break;
		}

		case JPEP::SW:
		{
			for(int steps=1; steps <= jumplimit ; steps++)
			{
				int nx = x-steps;
				int ny = y+steps;
				n = map->getNodeFromMap(nx, ny);
				if(n == 0)
					break;

				if(addJumpNodes(JPEP::S))
				{
					addJumpNodes(JPEP::W);
					break;
				}

				if(addJumpNodes(JPEP::W))
					break;

				if(nx == goalx || ny == goaly)
					break;
			}
			break;
		}
	}

	if(n)
	{
		neighbours.push_back(n);
		return true;
	}

	return false;
}
