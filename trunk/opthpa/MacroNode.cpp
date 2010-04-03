#include "MacroNode.h"

MacroNode::MacroNode(const char* _name) : node(_name)
{
}

MacroNode::MacroNode(const MacroNode* n) : node(n)
{
	p = n->p;
}

MacroNode::~MacroNode()
{

}
