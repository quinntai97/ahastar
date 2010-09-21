#ifndef FOURCONNECTEDEXPANSIONPOLICY_H
#define FOURCONNECTEDEXPANSIONPOLICY_H

#include "ExpansionPolicy.h"

class FourConnectedExpansionPolicy : public ExpansionPolicy
{
	public:
		FourConnectedExpansionPolicy(mapAbstraction* map_, node* target_);
		virtual ~FourConnectedExpansionPolicy();

		virtual node* n();
		virtual node* first();
		virtual node* next();
		virtual bool hasNext();

	private:
		int which;
};

#endif

