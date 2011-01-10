#ifndef JUMPPOINTSEXPANSIONPOLICY_H
#define JUMPPOINTSEXPANSIONPOLICY_H

// JumpPointsExpansionPolicy.h
//
// This expansion policy reduces the branching factor
// of a node n during search by ignoring any neighbours which
// could be reached by an equivalent (or shorter) path that visits
// the parent of n but not n itself.
//
// An extension of this idea is to generate jump nodes located in the
// same direction as the remaining neighbours. 
//
// TODO: Jump nodes need to be better explained. Perhaps a reference
// to a draft paper.
//
// Based on an idea suggsted by Alban Grastien and Adi Botea.
//
// @author: dharabor
// @created: 06/01/2010

#include "ExpansionPolicy.h"
#include <vector>
#include <stdexcept>

namespace JPEP
{
	typedef enum 
	{N, S, E, W, NE, NW, SE, SW} 
	Direction;
}

class JumpPointsExpansionPolicy : public ExpansionPolicy
{

	public:
		JumpPointsExpansionPolicy();
		virtual ~JumpPointsExpansionPolicy();

		virtual void expand(node* t) throw(std::logic_error);
		virtual node* first();
		virtual node* next();
		virtual node* n();
		virtual double cost_to_n();
		virtual bool hasNext();

		int jumplimit; 

	private:
		JPEP::Direction directionToParent(node* n);
		void computeNeighbourSet();
		node* findJumpNode(JPEP::Direction d, int x, int y);

		std::vector<node*> neighbours;
		unsigned int neighbourIndex; 
};

#endif

