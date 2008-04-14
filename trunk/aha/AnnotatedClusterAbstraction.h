/*
 *  AnnotatedClusterAbstraction.h
 *  hog
 *
 *  Created by Daniel Harabor on 22/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERABSTRACTION_H
#define ANNOTATEDCLUSTERABSTRACTION_H

#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"
#include "AnnotatedClusterFactory.h"
#include "AnnotatedCluster.h"
#include <ext/hash_map>

class Map;
//class AnnotatedCluster;
class AnotatedClusterFactory;
//class NodeIsNullException;

namespace ACAUtil
{
	typedef __gnu_cxx::hash_map<int, path*> pathTable;
	
	enum GraphQualityParameter
	{
		kHighQualityAbstraction, // abstract graph includes all optimal paths for every capability between endpoints in each cluster
		kMediumQualityAbstraction, // try to re-use optimal paths found using single-terrain-capability for multi-terrain-capable agents
		kLowQualityAbstraction // build large single capability corridors first and re-use as many as possible 
	};
}

class NodeHasNonZeroAbstractionLevelException : public std::exception
{
	const char* what() const throw() { return "found a node with kAbstractionlLevel label > 0 when it should be == 0"; }
};

class AnnotatedClusterAbstraction : public AnnotatedMapAbstraction
{
	
	#ifdef UNITTEST
		friend class AnnotatedClusterAbstractionTest;
	#endif
	
	public: 
		AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize, ACAUtil::GraphQualityParameter qual=ACAUtil::kHighQualityAbstraction);
		virtual ~AnnotatedClusterAbstraction();

		virtual void buildClusters(IAnnotatedClusterFactory*);
		virtual void buildEntrances();
		virtual void insertStartAndGoalNodesIntoAbstractGraph(node*, node*) throw(NodeIsNullException, NodeHasNonZeroAbstractionLevelException);
		virtual void removeStartAndGoalNodesFromAbstractGraph();

		virtual int getClusterSize() { return clustersize; } 
		virtual int getNumClusters() { return clusters.size(); } 
		virtual AnnotatedCluster* getCluster(int cid);
		virtual double distance(path* p); 
		
		int getNodesExpanded() { return nodesExpanded; }
		void setNodesExpanded(int num) {  nodesExpanded = num; }
		int getNodesTouched() { return nodesTouched; }
		void setNodesTouched(int num) {  nodesTouched = num; }
		int getPeakMemory() { return peakMemory; }
		void setPeakMemory(int num) {  peakMemory = num; }
		double getSearchTime() { return searchTime; }
		void setSearchTime(double newtime) { searchTime = newtime; }

		virtual void addPathToCache(edge* e, path* p);
		virtual path* getPathFromCache(edge*); 
		int getPathCacheSize() { return pathCache.size(); };
		ACAUtil::GraphQualityParameter getQualityParam() { return quality; }
		void setGraphQualityParameter(ACAUtil::GraphQualityParameter newqual) { quality = newqual; }
		
		virtual void openGLDraw(); 
		
	protected: 
		virtual void addCluster(AnnotatedCluster* ac);
		virtual int getNumberOfAbstractionLevels() { return abstractions.size(); }

	private:
		int nodesExpanded; 
		int nodesTouched; 
		double searchTime;
		int peakMemory; 
		
		int startid, goalid;
		int clustersize;
		std::vector<AnnotatedCluster *> clusters;
		ACAUtil::pathTable pathCache;
		ACAUtil::GraphQualityParameter quality;
};


#endif