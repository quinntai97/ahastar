#ifndef FCRREXPANSIONPOLICY_H
#define FCRREXPANSIONPOLICY_H

// FCRRExpansionPolicy.h
//
// Four Connected Rectangular Room expansion policy.
// Iterates over all edges incident with the target node plus
// one neighbour on the directly opposite side of the current empty
// rectangular room.

#include "ExpansionPolicy.h"

class FCRRExpansionPolicy : public ExpansionPolicy
{
	public:
		FCRRExpansionPolicy(mapAbstraction*, node*);
		virtual ~FCRRExpansionPolicy();

		virtual node* n();
		virtual node* first(); 
		virtual node* next();
		virtual bool hasNext();

	private:
		int which_macro;
};

#endif
