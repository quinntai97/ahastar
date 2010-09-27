#ifndef EXPANSIONPOLICY_H
#define EXPANSIONPOLICY_H

// ExpansionPolicy.h
//
// Provides a generic interface for iterating over the neighbours
// of a node during expansion.
//
// @author: dharabor
// @created: 21/09/2010

class node;
class mapAbstraction;

class ExpansionPolicy
{
	public:
		ExpansionPolicy();
		virtual ~ExpansionPolicy();

		// initialises the policy with a target node
		virtual void expand(node* t);

		// return the first neighbour
		virtual node* first() = 0;

		// return the next neighbour
		virtual node* next() = 0;

		// return the current neighbour
		virtual node* n() = 0;

		// cost of the edge (target, n)
		virtual double cost_to_n() = 0;

		// returns true until all remaining neighbours are iterated over
		virtual bool hasNext() = 0;

		node* getTarget() const { return target;}

	protected:
		node* target;
};

#endif

