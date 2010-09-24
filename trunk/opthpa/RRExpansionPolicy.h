#ifndef RREXPANSIONPOLICY_H
#define RREXPANSIONPOLICY_H

#include "ExpansionPolicy.h"

class IncidentEdgesExpansionPolicy;
class RRExpansionPolicy : public ExpansionPolicy
{
	public:
		RRExpansionPolicy();
		virtual ~RRExpansionPolicy();

		virtual node* first();
		virtual node* n();
		virtual node* next();
		virtual bool hasNext();

	private:
		int which;
		int max;
		IncidentEdgesExpansionPolicy* edgesPolicy;
};

#endif
