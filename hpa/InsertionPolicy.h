#ifndef INSERTIONPOLICY_H
#define INSERTIONPOLICY_H

// InsertionPolicy.h
//
// Defines a general class of insertion algorithms
// for use during hierarchical pathfinding.
//
// An insertion algorithm takes the start and goal nodes from a low-level graph
// (usually the grid, but not always) and inserts them into
// a higher level graph.
//
// As the insertion is usually temporary, this class also defines
// function stubs for a remove method -- which deletes inserted nodes.
//
// @author: dharabor
// @created: 08/03/2011
//

#include <stdexcept>
#include <vector>

class statCollection;
class node;
class InsertionPolicy
{
	public:
		InsertionPolicy();
		virtual ~InsertionPolicy();

		virtual node* insert(node* n) throw(std::invalid_argument) = 0;
		virtual void remove(node* n) throw(std::runtime_error) = 0;

		// metrics
		long getInsertNodesExpanded() { return insertNodesExpanded; }
		long getInsertNodesTouched() { return insertNodesTouched; }
		long getInsertNodesGenerated() { return insertNodesGenerated; }
		double getInsertSearchTime() { return insertSearchTime; }
		void resetMetrics();

	protected:
		void addNode(node* n);
		void removeNode(node* n);

	private:
		long insertNodesExpanded;
		long insertNodesTouched;
		long insertNodesGenerated;
		double insertSearchTime;

		std::vector<node*>* insertedNodes;

};

#endif

