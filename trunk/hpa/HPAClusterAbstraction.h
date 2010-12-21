#ifndef HPACLUSTERABSTRACTION_H
#define HPACLUSTERABSTRACTION_H

//  HPAClusterAbstraction.h
//
// 	An alternative implementation of HPA*.
// 	More flexible / extensible than the version found in HOG.
//
//	@author: dharabor
//	@created: 20/11/2008
//
//

#include "GenericClusterAbstraction.h"
#include "HPAUtil.h"
#include <iostream>
#include <stdexcept>

class HPACluster;
class IClusterAStarFactory;
class HPAClusterFactory;
class path;

class HPAClusterAbstraction : public GenericClusterAbstraction
{
	#ifdef UNITTEST
		friend class HPAClusterAbstractionTest;
	#endif

	public:
		HPAClusterAbstraction(Map* m, HPAClusterFactory* cf, INodeFactory* nf, 
				IEdgeFactory* ef, bool allowDiagonals=true);
		virtual ~HPAClusterAbstraction();
		
		// need to make GenericClusterAbstraction concrete
		virtual void buildClusters();
		virtual void insertStartAndGoalNodesIntoAbstractGraph(node*, node*) 
			throw(std::invalid_argument);
		virtual void removeStartAndGoalNodesFromAbstractGraph() 
			throw(std::runtime_error);

		// measure insertion effort 
		long getNodesExpanded() { return nodesExpanded; }
		void setNodesExpanded(long num) { nodesExpanded = num; } 
		long getNodesTouched() { return nodesTouched; }
		void setNodesTouched(long num) { nodesTouched = num; } 
		long getNodesGenerated() { return nodesGenerated; }
		void setNodesGenerated(long num) { nodesGenerated = num; } 
		double getSearchTime() { return searchTime; }
		void setSearchTime(double num) { searchTime = num; }

		virtual double h(node *a, node *b);
		virtual void print(std::ostream& out);
		void verifyClusters();

		int getClusterSize() { return clustersize; } 
		void setClusterSize(unsigned int csz) { clustersize = csz; }

	private:
		unsigned int clustersize;

		long nodesExpanded; 
		long nodesTouched; 
		double searchTime;
		long nodesGenerated;
};

#endif
