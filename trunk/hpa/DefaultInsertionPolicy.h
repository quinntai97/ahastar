#ifndef DEFAULTINSERTIONPOLICY_H
#define DEFAULTINSERTIONPOLICY_H

// DefaultInsertionPolicy.h
//
// The default insertion method for cluster-based abstractions.
// This class inserts start and goal nodes into the abstract graph 
// by running a search between each node to be inserted and every abstract
// node in its parent cluster.
//
// @author: dharabor
// @created: 10/03/2011
//

#include "InsertionPolicy.h"
#include <stdexcept>

class GenericClusterAbstraction;
class DefaultInsertionPolicy : public InsertionPolicy
{
	public:
		DefaultInsertionPolicy(GenericClusterAbstraction* _map);
		virtual ~DefaultInsertionPolicy();

		virtual void insertStartAndGoalNodesIntoAbstractGraph(node* n) 
			throw(std::invalid_argument);
		virtual void removeStartAndGoalNodesFromAbstractGraph()
			throw(std::runtime_error);

		bool getVerbose() { return verbose; }
		void setVerbose(bool _verbose) { verbose = _verbose; }

	private:
		GenericClusterAbstraction* map;
		bool verbose;

};

#endif

