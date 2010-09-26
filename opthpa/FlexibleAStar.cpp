#include "FlexibleAStar.h"

#include "ExpansionPolicy.h"
#include "graph.h"
#include "heap.h"
#include "Heuristic.h"
#include "mapAbstraction.h"
#include "path.h"
#include "reservationProvider.h"
#include "timer.h"
#include "unitSimulation.h"

FlexibleAStar::FlexibleAStar(ExpansionPolicy* policy, Heuristic* heuristic)
	: searchAlgorithm()
{
	this->policy = policy;
	this->heuristic = heuristic;
}

FlexibleAStar::~FlexibleAStar()
{
	delete policy;
	delete heuristic;
}

const char* FlexibleAStar::getName()
{
	return "FlexibleAStar";
}

path* FlexibleAStar::getPath(graphAbstraction *aMap, node *from, node *goal,
		reservationProvider *rp)
{
	nodesExpanded=0;
	nodesTouched=0;
	searchTime =0;
	nodesGenerated = 0;

	if(verbose) 
	{
		std::cout << "getPath() mapLevel: ";
		std::cout <<from->getLabelL(kAbstractionLevel)<<std::endl;
	}

	if(!checkParameters(aMap, from, goal))
		return NULL;

	this->map = aMap;
	graph *g = aMap->getAbstractGraph(from->getLabelL(kAbstractionLevel));

	from->setLabelF(kTemporaryLabel, heuristic->h(from, goal));
	from->backpointer = 0;
	
	heap openList(30);
	std::map<int, node*> closedList;
	
	openList.add(from);
	path *p = NULL;
	
	Timer t;
	t.startTimer();
	while(1) 
	{
		node* current = ((node*)openList.remove()); 

		// check if the current node is the goal (early termination)
		if(current == goal)
		{
			p = extractBestPath(g, current->getNum());
			if(verbose)
				printNode(std::string("goal found! "), current);
			break;
		}
		
		// expand current node
		expand(current, goal, &openList, &closedList);
		closeNode(current, closedList);
				
		// terminate when the open list is empty
		if(openList.empty())
		{
			if(verbose) std::cout << "search failed. ";
			break;
		}
	}
	searchTime = t.endTimer();
	closedList.clear();

	if(verbose)
	{
		std::cout << "\n";
		printPath(p);
	}

	return p;	
}

void FlexibleAStar::closeNode(node* current, std::map<int, node*>& closedList)
{
	if(markForVis)
		current->drawColor = 2; // visualise expanded

	if(verbose)
	{	
		printNode(std::string("closing... "), current);
		std::cout << " f: "<<current->getLabelF(kTemporaryLabel) <<std::endl;
	}
	closedList[current->getUniqueID()] = current;	

}

void FlexibleAStar::expand(node* current, node* goal, heap* openList,
		std::map<int, node*>* closedList)
{
	// expand the current node
	if(verbose) printNode(std::string("expanding... "), current, goal);
	nodesExpanded++;

	policy->expand(current, map);
	for(node* neighbour = policy->first(); neighbour != 0; 
			neighbour = policy->next())
	{
		nodesTouched++;			
		if(closedList.find(neighbour->getUniqueID()) == closedList.end()) 
		{
			if(openList->isIn(neighbour)) 
			{	
				if(verbose) 
				{
					printNode("\t\trelaxing...", neighbour);
					std::cout << " f: "<<neighbour->getLabelF(kTemporaryLabel);
				}

				relax(openList); 
			}
			else
			{
				if(verbose) 
					printNode("\t\tgenerating...", neighbour);

				neighbour->setLabelF(kTemporaryLabel, MAXINT); // initial fCost 
				neighbour->setKeyLabel(kTemporaryLabel); // store priority here 
				neighbour->reset();  // reset any marked edges 
				openList->add(neighbour);
				relax(openList, goal); 
				nodesGenerated++;
			}
			if(markForVis)
				neighbour->drawColor = 1; // visualise touched
		}
		else
			debugClosedNode(current, neighbour, goal, neighbours);

		if(verbose)
			std::cout << std::endl;
	}
}

// a node is correctly closed only if its fCost is smaller than the cost of
// of any path through an alternative parent.
void FlexibleAStar::debugClosedNode(node* current, node* neighbour, node* goal)
{
	if(verbose) 
		printNode("\t\tclosed! ", neighbour);

	double fclosed = neighbour->getLabelF(kTemporaryLabel);
	double gclosed =  fclosed - heuristic->h(neighbour, goal);

	// alternate fcost
	double alth = h(neighbour, goal);
	double altg = current->getLabelF(kTemporaryLabel) - heuristic->h(current, goal);

	if((altg + e->getWeight() + alth) < fclosed)
	{
		std::cout << "node "<<neighbour->getName()<<" expanded out of order! ";
		std::cout << " fClosed = "<<fclosed;
		std::cout << " fActual: "<<altg + e->getWeight() + alth;
		std::cout << " gClosed = "<<gclosed;
		std::cout << "; alternative: "<<altg+e->getWeight();
		printNode("\nfaulty node: ", neighbour, goal); 
		std::cout << std::endl;
		printNode(" alt parent: ", current, goal);
		std::cout << std::endl;
	}
}

bool FlexibleAStar::checkParameters(graphAbstraction* aMap, node* from, node* to)
{
	if(aMap == NULL)
		return false;
				
	if(!from || !to)
		return false;

	if(from->getUniqueID() == to->getUniqueID())
		return false;
		
	if(from->getLabelL(kFirstData) == to->getLabelL(kFirstData) 
			&& from->getLabelL(kFirstData+1) == to->getLabelL(kFirstData+1))
		return false;

	if(from->getLabelL(kAbstractionLevel) != to->getLabelL(kAbstractionLevel))
		return false;

	return true;
	
}

void FlexibleAStar::relax(heap* openList, node* goal)
{
	node* from = policy.getTarget();
	node* to = policy.n();

	double g_from = from->getLabelF(kTemporaryLabel) - heuristic->h(from, goal);
	double f_to = g_from + policy.cost_to_n() + heuristic->h(to, goal);
	
	if(f_to < to->getLabelF(kTemporaryLabel))
	{
		to->setLabelF(kTemporaryLabel, f_to);
		to->backpointer = from;
		openList->decreaseKey(to);
	}
}

path* FlexibleAStar::extractBestPath(graph *g, unsigned int current)
{
	return 0;
}

void FlexibleAStar::printNode(std::string msg, node* n, node* goal)
{	
	std::cout << msg <<"addr: "<<&(*n)<<" num: "<<n->getUniqueID();
	std::cout <<" ("<<n->getLabelL(kFirstData)<<","<<n->getLabelL(kFirstData+1)<<") ";

	if(static_cast<MacroNode*>(n))
	{
		MacroNode* mp = static_cast<MacroNode*>(n)->getParent();
		if(mp)
		{
			std::cout << " mp: "<<static_cast<MacroNode*>(n)->getParent()->getName()<<" ";
		}
			if(n->getMarkedEdge())
			{
				graph* g =  getGraphAbstraction()->getAbstractGraph(n->getLabelL(kAbstractionLevel));
				edge* e = n->getMarkedEdge();
				int parentId = e->getTo() == n->getNum()?e->getFrom():e->getTo();
				node* parent = g->getNode(parentId);
				std::cout << " p: ("<<parent->getLabelL(kFirstData)<<", "<<parent->getLabelL(kFirstData+1)<<") ";
			}
	}

	if(goal)
	{
		double hcost = h(n, goal);
		//double hcost = 0; 
		double gcost = n->getLabelF(kTemporaryLabel) - hcost;
		std::cout << " f: "<<gcost+hcost<<" g: "<<gcost<<" h: "<<hcost<<std::endl;
	}
}

void FlexibleAStar::printPath(path* p)
{
	if(!p)
	{
		std::cout << "search failed"<<std::endl;
		return;
	}

	graphAbstraction* aMap = this->getGraphAbstraction();
	node* goal = p->tail()->n;
	node* last = 0;
	double g=0;
	while(p)
	{
		node* n = p->n;
		
		double h = aMap->h(n, goal);
		if(last)
			g += aMap->h(n, last);
		std::cout << "id: "<<n->getUniqueID()<<" node @ "<<n->getName();
		std::cout << " g: "<<g<<" f: "<<g+h<<std::endl;
		last = n;
		p = p->next;
	}		
}
