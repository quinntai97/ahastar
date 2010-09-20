#ifndef EXPANSIONPOLICY_H
#define EXPANSIONPOLICY_H

// ExpansionPolicy.h
//
// Provides a generic interface for iterating over the neighbours
// of a node during expansion.

class node;
class mapAbstraction;

class ExpansionPolicy
{
	public:
		ExpansionPolicy(mapAbstraction* map_, node* t);
		virtual ~ExpansionPolicy();

		node* getTarget() const { return target;}
		virtual void next() = 0;
		virtual node* first() = 0;
		virtual node* n() const = 0;

	protected:
		node* target; // node being expanded
		mapAbstraction* map;
};

#endif
