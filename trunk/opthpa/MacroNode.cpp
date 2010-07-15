#include "MacroNode.h"

MacroNode::MacroNode(const char* _name) : ClusterNode(_name)
{
	p = 0;
}

MacroNode::MacroNode(const MacroNode* n) : ClusterNode(n)
{
	p = n->p;
}

MacroNode::~MacroNode()
{

}

void MacroNode::reset()
{
	ClusterNode::reset();
	setMacroParent(0);
}

void MacroNode::removePrimaryNeighbourId(int nodeId)
{
	std::vector<int>::iterator it = primary.begin();
	while(it != primary.end())
	{
		if(*it == nodeId)
		{
			primary.erase(it);
			break;
		}
		it++;
	}
}

void MacroNode::removeSecondaryNeighbourId(int nodeId)
{
	std::vector<int>::iterator it = secondary.begin();
	while(it != secondary.end())
	{
		if(*it == nodeId)
		{
			secondary.erase(it);
			break;
		}
		it++;
	}
}
