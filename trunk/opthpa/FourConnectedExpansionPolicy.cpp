#include "FourConnectedExpansionPolicy.h"

FourConnectedExpansionPolicy::FourConnectedExpansionPolicy(
		graphAbstraction* map_, node* target_) : ExpansionPolicy(map_, target_)
{
	which = 0;
}

FourConnectedExpansionPolicy::~FourConnectedExpansionPolicy()
{
}

node* FourConnectedExpansionPolicy::n() const
{
	node* n = 0;
	switch(which)
	{
		case '0':
		{
			
		}
			
	}

}

void next()
{
	if(0 <= which < 4)
	{
		which++;
	}
	else
		which = -1;
}
