#include "MacroEdge.h"

MacroEdge::MacroEdge(unsigned int from, unsigned int to, double weight)
	: edge(from, to, weight)
{
}

MacroEdge::MacroEdge(MacroEdge& e) : edge(e)
{
}

MacroEdge::~MacroEdge()
{
}
