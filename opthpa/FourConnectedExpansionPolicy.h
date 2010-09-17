#ifndef FOURCONNECTEDEXPANSIONPOLICY_H
#define FOURCONNECTEDEXPANSIONPOLICY_H

#include "ExpansionPolicy.h"

class FourConnectedExpansionPolicy : public ExpansionPolicy
{
	public:
		FourConnectedExpansionPolicy(graphAbstraction* map_, node* target_);
		virtual ~FourConnectedExpansionPolicy();

		virtual node* n() const; 
		virtual void next();

	private:
		int which;
};

#endif

