/*
 *  AnnotatedAStar.h
 *  hog
 *
	Search algorithm which supports different agent capabilities:
		- terrain traversal (agent differentiates between different types of terrain)
		- size (agents may occupy several tiles at a time)
		
	Intended for graphs produced by AnnotatedMapAbstraction and AnnotatedClusterAbstraction.
	
	TODO: HOG's AStar hierarchy is a mess. Lots of re-implementations, no shared code etc. Need an abstract parent implementing things like
	edge relaxation, path extraction and drawing paths(maybe).
	
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTAR_H
#define ANNOTATEDASTAR_H

#include <ext/hash_map>
#include "heap.h"
#include "aStar3.h"
//#include "searchAlgorithm.h"
#include "graph.h"
#include "graphAbstraction.h"

namespace AAStarUtil {      
  typedef __gnu_cxx::hash_map<int,bool> NodeMap;
}


// TODO: get rid of overridden getPath methods with capability & size params. these are set using accessor methods. if not, the search 
// returns null anyway (0 default values for both).
class AbstractAnnotatedAStar : public aStarOld
{
	public:	
		AbstractAnnotatedAStar() : useCorridor(false) { }
		virtual const char* getName() { return "AbstractAnnotatedAStar"; }
		virtual path* getPath(graphAbstraction *aMap, node* from, node* to) { return getPath(aMap, from, to, searchterrain, minclearance); }
		virtual path* getPath(graphAbstraction*, node*, node*, int, int) = 0;
		path* getPath(node* from, node* to, int terrain, int size) { return getPath(this->getGraphAbstraction(), from, to, terrain, size); }
		
		int getMinClearance() { return minclearance;}
		void setMinClearance(int minclearance) { this->minclearance = minclearance; }
		int getSearchTerrain() { return searchterrain; }
		void setSearchTerrain(int searchterrain) { this->searchterrain = searchterrain; }
		int getPeakMemory() { return peakmemory; }
		int getSearchTime() { return searchtime; }
		void limitSearchToClusterCorridor(bool value) { useCorridor=value; }
		void setCorridorClusters(int cid1, int cid2) { cluster1 = cid1; cluster2 = cid2; }
		
	protected:
		bool isInCorridor(node* n) { if(n->getParentCluster() != cluster1 && n->getParentCluster() != cluster2) return false; return true; }
		bool verbose;
		virtual bool evaluate(node* n, node* target) = 0; // check if a node is traversable
		int peakmemory;
		double searchtime;
		bool useCorridor;
		
	private:
		int searchterrain, minclearance; 
		int cluster1, cluster2;

};

class AnnotatedAStar : public AbstractAnnotatedAStar
{
	public:
		#ifdef UNITTEST
			friend class AnnotatedAStarTest; // need to make the test class a friend to enable private/protected method testing
		#endif
	
		virtual path* getPath(graphAbstraction*, node*, node*, int, int);
		virtual const char* getName() { return "AnnotatedAStar"; }
		static tDirection getDirection(node* current, node* target); // TODO: move this to a common AStar base class


	protected:
		virtual bool evaluate(node* n, node* target);
};

#endif