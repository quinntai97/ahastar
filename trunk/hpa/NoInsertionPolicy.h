#ifndef NOINSERTIONPOLICY_H
#define NOINSERTIONPOLICY_H

// NoInsertionPolicy.h
//
// The insertion policy for when you want to run a 
// hierarchical search that doesn't use insertion.
//
// @author: dharabor
// @created: 08/03/2011
//

#include "InsertionPolicy.h"

class NoInsertionPolicy : public InsertionPolicy
{
	public:
		NoInsertionPolicy();
		virtual ~NoInsertionPolicy();

		virtual void insertStartAndGoalNodesIntoAbstractGraph(
			node* s, node* g) throw(std::invalid_argument);
		virtual void removeStartAndGoalNodesFromAbstractGraph()
			throw(std::runtime_error);
};

#endif

