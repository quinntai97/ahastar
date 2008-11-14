/*
 *  ClusterAStar.h
 *  hog
 *
	A simple variant on the standard A* implementation which supports limiting search to one or two clusters.
	Specifically, if useCorridor is true, the parentClusterId of the start and goal nodes are used to limit which nodes are added to the open list.
	NB: assumes the graph is composed of nodes of type ClusterNode.
		
	Also adds additional metrics for measuring search performance:
	 - Total time to search (in milliseconds).
	 - Peak memory (records max size of open list during the search).
	
	If the corridor is unspecified, a regular A* search is run on the map.
	
	TODO: 
		- HOG's AStar hierarchy is a mess. Lots of re-implementations, no shared code etc. Need an abstract parent implementing things like
			edge relaxation, path extraction and drawing paths(maybe).
		- Refactor to take a set of nodes as the "cluster" inside which search is limited & remove requirement that only graphs composed of 
			ClusterNodes may be used.
	
 *  Created by Daniel Harabor on 14/11/08.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERASTAR_H
#define CLUSTERASTAR_H

#include <ext/hash_map>
#include "heap.h"
#include "aStar3.h"
#include "graph.h"
#include "graphAbstraction.h"

namespace CAStarUtil {      
  typedef __gnu_cxx::hash_map<int,bool> NodeMap;
}

class statCollection;

class AbstractClusterAStar : public aStarOld
{
	public:	
		AbstractClusterAStar() : useCorridor(false) { corridorClusterId = -1;}
		virtual ~AbstractClusterAStar() {}
		virtual const char* getName() = 0;
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0) = 0;
		
		long getPeakMemory() { return peakmemory; }
		double getSearchTime() { return searchtime; }
		void limitSearchToClusterCorridor(bool value) { useCorridor=value; }
//		void setCorridorClusters(int cid1, int cid2) { cluster1 = cid1; cluster2 = cid2; }
		
	protected:
		bool isInCorridor(node* n);
		bool verbose;
		virtual bool evaluate(node* n, node* target) = 0; // check if a node is traversable
		long peakmemory;
		double searchtime;
		
		bool useCorridor;
		int corridorClusterId;

};

class ClusterAStar : public AbstractClusterAStar
{
	public:
		#ifdef UNITTEST
			friend class ClusterAStarTest; 
		#endif
		
		ClusterAStar() : AbstractClusterAStar() { e = NULL; }
		virtual ~ClusterAStar() {}
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0);
		virtual const char* getName() { return "ClusterAStar"; }
		virtual void logFinalStats(statCollection *stats);

	protected:
		virtual bool evaluate(node* n, node* target);
		edge* traversing() { return e; }
	private:
		edge* e;
};

#endif

