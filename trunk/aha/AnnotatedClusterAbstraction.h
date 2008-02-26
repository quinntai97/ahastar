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
#include "clusterAbstraction.h"
#include <exception>

class Map;

class NodeTerrainDoesNotMatchClusterOriginException : public std::exception
{
	public: 
		virtual const char* what() const throw();
};

class NodeClearanceGreaterThanClusterOriginException : public std::exception
{
	public: 
		virtual const char* what() const throw();
};

class NodeIsHardObstacleException : public std::exception
{
	public: 
		virtual const char* what() const throw();
};

class AnnotatedCluster;
class AnnotatedClusterAbstraction : public AnnotatedMapAbstraction
{
	
	#ifdef UNITTEST
		friend class AnnotatedClusterAbstractionTest;
	#endif
	
	public: 
		AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg);
		~AnnotatedClusterAbstraction();
		int getNumClusters() { return clusters.size(); } 
		
	private: 
		AnnotatedCluster* buildCluster(int, int);
		std::vector<Cluster *> clusters;

};

class AnnotatedCluster : public Cluster
{
	public:
		AnnotatedCluster(int, int, AnnotatedClusterAbstraction*) throw(NodeIsHardObstacleException, NodeTerrainDoesNotMatchClusterOriginException, NodeClearanceGreaterThanClusterOriginException);
		~AnnotatedCluster() { }
		void addNode(node *); 
		void addParent(node *);
		void getClusterTerrain(); 
		void getMaxClearance(); 
		
	private:
		void addNodesToCluster(AnnotatedClusterAbstraction*);
		void calculateClusterHeight();
		void calculateClusterWidth();
		int clusterterrain, maxclearance;
};

#endif