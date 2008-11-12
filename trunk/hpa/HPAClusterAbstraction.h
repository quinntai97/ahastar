/*
 *  HPAClusterAbstraction.h
 *  hog
 
	Alternative implementation of HPA*.
	Based on code from HAA*.
 *
 *  Created by dharabor on 10/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef HPACLUSTERABSTRACTION_H
#define HPACLUSTERABSTRACTION_H

#include "mapAbstraction.h"
#include "HPAUtil.h"
#include <stdexcept>

class IHPAClusterFactory;
class INodeFactory;
class IEdgeFactory;
class HPACluster;
class Map;
class searchAlgorithm;
class path;

typedef HPAUtil::clusterTable::const_iterator cluster_iterator;
class HPAClusterAbstraction : public mapAbstraction
{
	#ifdef UNITTEST
		friend class HPAClusterAbstractionTest;
	#endif

	public:
		HPAClusterAbstraction(Map* m, searchAlgorithm* alg, IHPAClusterFactory* cf, INodeFactory* nf, IEdgeFactory* ef, unsigned int clustersize);
		virtual ~HPAClusterAbstraction();
		
		virtual void buildEntrances();
		virtual void buildClusters();
		
		/* insert and remove start/goal */
		virtual void insertStartAndGoalNodesIntoAbstractGraph(node*, node*) throw(std::invalid_argument);
		void removeStartAndGoalNodesFromAbstractGraph() throw(std::runtime_error);
		
		/* cluster getters and iterator functions */
		cluster_iterator getClusterIter() const { return clusters.begin(); }
		HPACluster* clusterIterNext(cluster_iterator&) const;
		HPACluster* getCluster(int cid);
		int getClusterSize() { return clustersize; } 
		int getNumClusters() { return clusters.size(); } 

		/* measure insertion effort */
		int getNodesExpanded() { return nodesExpanded; }
		void setNodesExpanded(int num) {  nodesExpanded = num; }
		int getNodesTouched() { return nodesTouched; }
		void setNodesTouched(int num) {  nodesTouched = num; }
		int getPeakMemory() { return peakMemory; }
		void setPeakMemory(int num) {  peakMemory = num; }
		double getSearchTime() { return searchTime; }
		void setSearchTime(double newtime) { searchTime = newtime; }

		/* path caching for quick refinement */
		void addPathToCache(edge* e, path* p);
		path* getPathFromCache(edge* e);
		int getPathCacheSize() { return pathCache.size(); }

		/* drawing and overlay methods */
		virtual void openGLDraw(); 
		void setDrawClusters(bool draw) { drawClusters = draw; }
		bool getDrawClusters() { return drawClusters; }
		
		/* all the pure-virtual stuff we need to implement for a concrete mapAbstraction */
		virtual bool pathable(node*, node*) { return false; }
		virtual void verifyHierarchy() {}
		virtual void removeNode(node*) {} 
		virtual void removeEdge(edge*, unsigned int) {}
		virtual void addNode(node*) {}
		virtual void addEdge(edge*, unsigned int) {}
		virtual void repairAbstraction() {}
		virtual mapAbstraction* clone(Map *) { return NULL; }
	
	protected:
		void addCluster(HPACluster* cluster);
		int getNumberOfAbstractionLevels() { return abstractions.size(); }
	
	private:
		IHPAClusterFactory* cf;
		INodeFactory* nf;
		IEdgeFactory* ef;
		searchAlgorithm* alg;

		HPAUtil::clusterTable clusters;
		HPAUtil::pathTable pathCache;

		unsigned int clustersize;
		int startid, goalid; // for use during insertion
		bool drawClusters; 

		int nodesExpanded; 
		int nodesTouched; 
		double searchTime;
		int peakMemory;
};

#endif
