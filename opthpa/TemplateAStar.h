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


	private:

		void ValidateExpansionPolicy(ExpansionPolicy &p) const {}
		void ValidateHeuristic(Heuristic &h) const {}

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

		// check if the current node is the goal
		if(current == goal)
		{
			p = extractBestPath(g, current->getNum());
			if(verbose)
				printNode(string("goal found! "), current);
			break;
		}
		
		expand(current, goal, openList, closedList);

		// expand the current node
		if(verbose) printNode(string("expanding... "), current, goal);
		nodesExpanded++;

		TP neighbours(current, aMap);
		for(node* n = neighbours.first(); n != 0; n = neighbours.next())
		{
			
		}
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

void AbstractClusterAStar::closeNode(node* current, 
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
}

#endif
