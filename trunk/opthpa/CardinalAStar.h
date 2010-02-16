/*
 * CardinalAStar.h
 *
 * An A* variant which only expands nodes connected to neighbours
 * in one of the four cardinal directions (N, S, E, W).
 * This is equivalent to searching on a 4-connected tile map.
 *
 * When applied to searching over a graph generated by
 * EmptyClusterAbstraction, this search algorithm will perform an optimal
 * hierarchical search.
 *
 * See [Harabor & Botea 2010] for more details.
 *
 * @author: dharabor
 * @created: 16/02/2010
 *
 */

#ifndef CARDINALASTAR_H
#define CARDINALASTAR_H

#include "ClusterAStar.h"
#include <map>

class EmptyClusterAbstraction;

class CardinalAStar : public ClusterAStar
{
	public:
		CardinalAStar() { }
		~CardinalAStar() { }

		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0);

	protected:
		virtual bool evaluate(node* current, node* target, edge* e);

	private:
		path* refine(path* p);

		EmptyClusterAbstraction* aMap;
};

#endif

