#ifndef EXPANSIONPOLICY_H
#define EXPANSIONPOLICY_H

// ExpansionPolicy.h
//
// Provides a generic interface for iterating over the neighbours
// of a node during expansion.

class node;
class graphAbstraction;

class ExpansionPolicy
{
	public:
		ExpansionPolicy(graphAbstraction* map_, node* t);
		virtual ~ExpansionPolicy();

		node* target() const { return target;}
		virtual node* next() const = 0;
		virtual node* n() const = 0;

	protected:
		node* target; // node being expanded
		graphAbstraction* map;
};

#endif

