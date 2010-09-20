#ifndef FCRREXPANSIONPOLICY_H
#define FCRREXPANSIONPOLICY_H

// FCRRExpansionPolicy.h
//
// Four Connected Rectangular Room expansion policy.
// Iterates over all edges incident with the target node plus
// one neighbour on the directly opposite side of the current empty
// rectangular room.

#include "IncidentEdgesExpansionPolicy.h"

class FCRRExpansionPolicy : public IncidentEdgesExpansionPolicy
{
	public:
		FCRRExpansionPolicy(mapAbstraction*, node*);
		virtual ~FCRRExpansionPolicy();

		virtual node* n() const;
		virtual node* first(); 
		virtual void next();

	private:
		int which_macro;
};

#endif
