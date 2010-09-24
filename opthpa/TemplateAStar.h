#ifndef TEMPLATEASTAR_H
#define TEMPLATEASTAR_H


#include "ExpansionPolicy.h"
#include "Heuristic.h"

#include "graph.h"
#include "mapAbstraction.h"
#include "path.h"
#include "reservationProvider.h"
#include "searchAlgorithm.h"
#include "unitSimulation.h"

template <typename TP, typename TH>
class TemplateAStar : public searchAlgorithm
{
	public:

		TemplateAStar();
		virtual ~TemplateAStar();

		virtual const char *getName();
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, 
				reservationProvider *rp = 0);
	

	protected:
		virtual path* search(graph* g, node* from, node* goal);
		void expand(node* current, node* goal, heap* openList,
				std::map* closedList);
		void relax(heap* openList, node* goal);


	private:

		void ValidateExpansionPolicy(ExpansionPolicy &p) const {}
		void ValidateHeuristic(Heuristic &h) const {}
		void debugClosedNode(node* current, node* neighbour, node* goal);

		TP* policy;
		TH* heuristic;

};


TemplateAStar::TemplateAStar()
{
	policy = new TP();
	heuristic = new TH();

	ValidatePolicy(*policy);
	ValidateHeuristic(*heuristic);
}

TemplateAStar::~TemplateAStar()
{
	delete policy;
	delete heuristic;
}

path* TemplateAStar::getPath(graphAbstraction *aMap, node *from, node *to, 
		reservationProvider *rp)
{
	nodesExpanded=0;
	nodesTouched=0;
	peakmemory = 0;
	searchTime =0;
	nodesGenerated = 0;

	if(verbose) 
	{
		std::cout << "getPath() mapLevel: ";
		std::cout <<from->getLabelL(kAbstractionLevel)<<std::endl;
	}

	if(!checkParameters(aMap, from, to))
		return NULL;

	this->setGraphAbstraction(aMap);
	graph *g = aMap->getAbstractGraph(from->getLabelL(kAbstractionLevel));

	from->setLabelF(kTemporaryLabel, heuristic->h(from, goal));
	from->backpointer = 0;
	
	heap* openList = new heap(30);
	std::map<int, node*> closedList;
	
	openList->add(from);
	path *p = NULL;
	
	Timer t;
	t.startTimer();
	while(1) 
	{
		node* current = ((node*)openList->remove()); 

		// check if the current node is the goal (early termination)
		if(current == goal)
		{
			p = extractBestPath(g, current->getNum());
			if(verbose)
				printNode(string("goal found! "), current);
			break;
		}
		
		// expand current node
		expand(current, goal, openList, closedList);
		closeNode(current, closedList);
				
		// terminate when the open list is empty
		if(openList->empty())
		{
			if(verbose) std::cout << "search failed. ";
			break;
		}
	}
	searchTime = t.endTimer();
	delete openList; 
	closedList.clear();

	if(verbose)
	{
		std::cout << "\n";
		printPath(p);
	}

	return p;	
}

void TemplateAStar::closeNode(node* current, 
		std::map<int, node*>& closedList)
{
	if(markForVis)
		current->drawColor = 2; // visualise expanded

	if(verbose)
	{	
		printNode(string("closing... "), current);
		std::cout << " f: "<<current->getLabelF(kTemporaryLabel) <<std::endl;
	}
	closedList[current->getUniqueID()] = current;	

}

void TemplateAStar::expand(node* current, node* goal, heap* openList,
		std::map* closedList)
{
	// expand the current node
	if(verbose) printNode(string("expanding... "), current, goal);
	nodesExpanded++;

	policy.expand(current, aMap);
	for(node* neighbour = policy.first(); n != 0; n = policy.next())
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
			debugClosedNode(current, neighbour, to, neighbours);

		if(verbose)
			std::cout << std::endl;
	}
}

// a node is correctly closed only if its fCost is smaller than the cost of
// of any path through an alternative parent.
void TemplateAStar::debugClosedNode(node* current, node* neighbour, node* to)
{
	if(verbose) 
		printNode("\t\tclosed! ", neighbour);

	double fclosed = neighbour->getLabelF(kTemporaryLabel);
	double gclosed =  fclosed - heuristic->h(neighbour, to);

	// alternate fcost
	double alth = h(neighbour, to);
	double altg = current->getLabelF(kTemporaryLabel) - heuristic->h(current, to);

	if((altg + e->getWeight() + alth) < fclosed)
	{
		std::cout << "node "<<neighbour->getName()<<" expanded out of order! ";
		std::cout << " fClosed = "<<fclosed;
		std::cout << " fActual: "<<altg + e->getWeight() + alth;
		std::cout << " gClosed = "<<gclosed;
		std::cout << "; alternative: "<<altg+e->getWeight();
		printNode("\nfaulty node: ", neighbour, to); 
		std::cout << std::endl;
		printNode(" alt parent: ", current, to);
		std::cout << std::endl;
	}
}

void TemplateAStar::relax(heap* openList, node* goal)
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

#endif
