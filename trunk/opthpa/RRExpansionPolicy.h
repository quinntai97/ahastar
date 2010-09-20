#ifndef RREXPANSIONPOLICY_H
#define RREXPANSIONPOLICY_H

#include "IncidentEdgesExpansionPolicy.h"

class RRExpansionPolicy : public IncidentEdgesExpansionPolicy
{
	public:
		RRExpansionPolicy();
		virtual RRExpansionPolicy();

		virtual node* first();
		virtual node* n();
		virtual void next();

	protected:
		int which_macro;
		int max_macro;

};

#endif
