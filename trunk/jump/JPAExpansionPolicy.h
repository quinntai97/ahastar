#ifndef JPAEXPANSIONPOLICY_H
#define JPAEXPANSIONPOLICY_H

// JPAExpansionPolicy.h
//
// An expansion policy specifically for maps using JumpPointAbstraction.
// Similar to IncidentEdgesExpansionPolicy but some neighbours
// are replaced with intermediate nodes when the edge
// (n, neighbour) crosses the row or column that the goal is located
// on.
//
// See [Harabor & Grastien, 2011b]
//
// @author: dharabor
// @created: 17/03/2011
//

#include "ExpansionPolicy.h"

class node;
class JPAExpansionPolicy : public ExpansionPolicy
{
	public:
		JPAExpansionPolicy();
		virtual ~JPAExpansionPolicy();

		virtual node* first();
		virtual node* next();
		virtual node* n();
		virtual double cost_to_n();
		virtual bool hasNext();
};

#endif

