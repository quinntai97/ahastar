#ifndef REFINEMENTPOLICY_H
#define REFINEMENTPOLICY_H

// RefinementPolicy.h
// 
// Defines a general class of path refinement approaches for use during
// hierarchical pathfinding.
//
// Refinement is often required after an abstract path is computed. This is 
// because not all nodes on an abstract path are necessarily next to one 
// another.
//
// @author: dharabor
// @created: 08/03/2011
//

class path;
class RefinementPolicy
{
	public:
		virtual path* refine(path* abspath) = 0;
};

#endif

