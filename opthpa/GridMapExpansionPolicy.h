#ifndef GRIDMAPEXPANSIONPOLICY_H
#define GRIDMAPEXPANSIONPOLICY_H

// GridMapExpansionPolicy.h
//
// A base class for ExpansionPolicy objects for regular grids.
// Each GridMapExpansionPolicy object has a constant number
// of neighbours. The intention is to support all kinds of grids
// including tiles, octiles, hexes etc.
//
// @author: dharabor
// @created: 28/10/2010

#include "ExpansionPolicy.h"

class Heuristic;
class mapAbstraction;
class GridMapExpansionPolicy : public ExpansionPolicy
{
	public:
		GridMapExpansionPolicy(unsigned int maxNeighbours);
		virtual ~GridMapExpansionPolicy();
	
		virtual node* first();
		virtual node* next();
		virtual node* n() = 0;
		virtual bool hasNext();
		virtual double cost_to_n();

	protected:
		int which;
		int max; // maximum degree of a grid node

	private:
		double cost; // heuristic cost estimate; d(target, n()) 
};

#endif
