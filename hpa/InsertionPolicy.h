#ifndef INSERTIONPOLICY_H
#define INSERTIONPOLICY_H

// InsertionPolicy.h
//
// Defines a general class of insertion algorithms
// for use during hierarchical pathfinding.
//
// An insertion algorithm takes a node from a low-level graph
// (usually the grid, but not always) and inserts it into
// a higher level graph.
//
// As the insertion is usually temporary, this class also defines
// function stubs for a ::remove method -- which deletes inserted nodes.
//
// @author: dharabor
// @created: 08/03/2011
//

class statCollection;
class node;
class InsertionPolicy
{
	public:
		InsertionPolicy();
		virtual ~InsertionPolicy();

		// param: initial node; return: inserted node
		virtual node* insert(node* n) = 0;

		// param: node to be removed.
		virtual void remove(node* n) = 0;

		// metrics
		long getInsertNodesExpanded() { return insertNodesExpanded; }
		long getInsertNodesTouched() { return insertNodesTouched; }
		long getInsertNodesGenerated() { return insertNodesGenerated; }
		double getInsertSearchTime() { return insertSearchTime; }
		void resetMetrics();

	private:
		long insertNodesExpanded;
		long insertNodesTouched;
		long insertNodesGenerated;
		double insertSearchTime;

};

#endif

