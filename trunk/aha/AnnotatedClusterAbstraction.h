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

class Map;
//class AnnotatedCluster;
class AnotatedClusterFactory;
//class NodeIsNullException;

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
		AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize);
		~AnnotatedClusterAbstraction() { clusters.clear(); };

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
	
};


#endif