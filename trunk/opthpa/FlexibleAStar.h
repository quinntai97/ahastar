#ifndef FLEXIBLEASTAR_H
#define FLEXIBLEASTAR_H

// FlexibleAStar.h
//
// A standard implementation of the A* search algorithm which can be
// readily modified with alternative node expansion algorithms and different 
// heuristics.
//
// @author: dharabor
// @created: 26/09/2010


#include "searchAlgorithm.h"
#include <map>
#include <string>

class ExpansionPolicy;
class heap;
class Heuristic;
class FlexibleAStar : public searchAlgorithm
{
	public:
		FlexibleAStar(ExpansionPolicy*, Heuristic*);
		virtual ~FlexibleAStar();

		virtual const char *getName();
		virtual path *getPath(graphAbstraction *aMap, node *from, node *goal,
				reservationProvider *rp = 0);

		bool markForVis;	

	protected:
		ExpansionPolicy* policy;
		Heuristic* heuristic;
		graphAbstraction* map;
	
	private:
		path* search(graph* g, node* from, node* goal);
		void expand(node* current, node* goal, heap* openList,
				std::map<int, node*>* closedList);
		void closeNode(node* current, std::map<int, node*>& closedList);

		void relax(heap* openList, node* goal);
		path *extractBestPath(graph *g, unsigned int current);

		void debugClosedNode(node* current, node* neighbour, node* goal);
		bool checkParameters(graphAbstraction* aMap, node* from, node* to);
		void printNode(std::string msg, node* n, node* goal=0);
		void printPath(path* p);
};

#endif
