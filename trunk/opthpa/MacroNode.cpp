#include "MacroNode.h"

MacroNode::MacroNode(const char* _name) : ClusterNode(_name)
{
}

MacroNode::MacroNode(const MacroNode* n) : ClusterNode(n)
{
	p = n->p;
}

MacroNode::~MacroNode()
{

}
