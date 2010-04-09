// MacroNode.h
// 
// MacroNode is a specialisation of HOG's node class.
// It adds a "macro parent" attribute so that a node may be related 
// to another node without necessarily having an edge connecting them.
// 
// Macro parents are useful for implementing OHA* [1]
// 
// [1] D. Harabor & A. Botea, Fast and Optimal Hierarchical Pathfinding, 2010
// 
//	@author: dharabor
//	@created: 03-04-2010
// 

#ifndef MACRONODE_H
#define MACRONODE_H

#include "ClusterNode.h"

class MacroNode : public ClusterNode
{
	public:
		MacroNode(const char* _n);
		MacroNode(const MacroNode* n);		
		virtual ~MacroNode();
		virtual graph_object* clone() const { return new MacroNode(this); }

		inline void	setMacroParent(MacroNode* _p) { p = _p; }
		inline MacroNode* getMacroParent() { return p; }
		inline double getDistanceToMacroParent() { return pdist; }
		inline void setDistanceToMacroParent(double _dist) { pdist = _dist; }

	private:
		MacroNode* p;
		double pdist;
};

#endif

