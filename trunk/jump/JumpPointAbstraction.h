#ifndef JUMPPOINTABSTRACTION_H
#define JUMPPOINTABSTRACTION_H

// JumpPointAbstraction.h
//
// Creates a graph in which every neighbour of each node
// is a jump point.
//
// @author: dharabor
// @created: 15/03/2011
//

#include "mapAbstraction.h"

class IEdgeFactory;
class INodeFactory;
class Map;
class node;
class edge;
class graph;

class JumpPointAbstraction : public mapAbstraction
{
	public:
		JumpPointAbstraction(Map*, INodeFactory*, IEdgeFactory*);
		virtual ~JumpPointAbstraction();
		virtual mapAbstraction *clone(Map *);

		virtual bool pathable(node*, node*);
		virtual void verifyHierarchy();
		virtual void removeNode(node *n);
		virtual void removeEdge(edge *e, unsigned int absLevel);
		virtual void addNode(node *n);
		virtual void addEdge(edge *e, unsigned int absLevel);
		virtual void repairAbstraction();

	private:
		INodeFactory* nf;
		IEdgeFactory* ef;

		graph* getJumpPointGraph();		
};

#endif

