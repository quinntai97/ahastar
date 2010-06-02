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
