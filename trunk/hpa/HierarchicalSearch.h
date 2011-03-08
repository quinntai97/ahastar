#ifndef HIERARCHICALSEARCH_H
#define HIERARCHICALSEARCH_H

// HierarchicalSearch.h
//
// Defines a framework for hierarchical pathfinding algorithms.
// Each such algorithm consists of 3 components:
// 	1. An insertion policy
// 	2. A search strategy
// 	3. A path refinement policy
//
// 	

#include "searchAlgorithm.h"

class InsertionPolicy;
class RefinementPolicy;
class statsCollection;
class HierarchicalSearch : public searchAlgorithm
{
	public:
		HierarchicalSearch(InsertionPolicy*, searchAlgorithm*, 
				RefinementPolicy*);
		virtual ~HierarchicalSearch();
		virtual const char* getName() { return alg->getName(); }

		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, 
				reservationProvider *rp = 0);	

		long getInsertNodesExpanded() { return insertNodesExpanded; }
		long getInsertNodesTouched() { return insertNodesTouched; }
		long getInsertNodesGenerated() { return insertNodesGenerated; }
		double getInsertSearchTime() { return insertSearchTime; }
		virtual void logFinalStats(statCollection* sc);

	private:
		void updateMetrics();  
		bool checkParameters(node* from, node* to);
		void resetMetrics();

		searchAlgorithm* alg;
		InsertionPolicy* insertPolicy;
		RefinementPolicy* refinePolicy;

		long insertNodesExpanded;
		long insertNodesTouched;
		long insertNodesGenerated;
		double insertSearchTime;
};

#endif

