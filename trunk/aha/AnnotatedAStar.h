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

class statCollection;

class AbstractAnnotatedAStar : public aStarOld
{
	public:	
		AbstractAnnotatedAStar(int _capability, int _clearance) : useCorridor(false), capability(_capability), clearance(_clearance) { capability=0; clearance=0; }
		virtual const char* getName() = 0;
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0) = 0;
		
		int getClearance() { return clearance;}
		void setClearance(int clearance) { this->clearance = clearance; }
		int getCapability() { return capability; }
		void setCapability(int capability) { this->capability = capability; }
		long getPeakMemory() { return peakmemory; }
		double getSearchTime() { return searchtime; }
		void limitSearchToClusterCorridor(bool value) { useCorridor=value; }
		void setCorridorClusters(int cid1, int cid2) { cluster1 = cid1; cluster2 = cid2; }
		
	protected:
		bool isInCorridor(node* n) { if(n->getParentCluster() != cluster1 && n->getParentCluster() != cluster2) return false; return true; }
		bool verbose;
		virtual bool evaluate(node* n, node* target) = 0; // check if a node is traversable
		long peakmemory;
		double searchtime;
		bool useCorridor;
		
	private:
		int capability, clearance; 
		int cluster1, cluster2;

};

class AnnotatedAStar : public AbstractAnnotatedAStar
{
	public:
		#ifdef UNITTEST
			friend class AnnotatedAStarTest; // TODO: replace these stupid friends with an inheritance-based solution
			friend class AnnotatedHierarchicalAStarTest;
		#endif
		AnnotatedAStar(int _capability=0, int _clearance=0) : AbstractAnnotatedAStar(_capability, _clearance) { e = NULL; }
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0);
		virtual const char* getName() { return "AAStar"; }
		static tDirection getDirection(node* current, node* target); // TODO: move this to a common AStar base class
		virtual void logFinalStats(statCollection *stats);

	protected:
		virtual bool evaluate(node* n, node* target);
		edge* traversing() { return e; }
	private:
		edge* e;
};

#endif

