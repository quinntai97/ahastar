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
class AnnotatedCluster;

class ACAException : public std::exception
{
	public:
		ACAException(node* problemNode, AnnotatedCluster* ac) { this->problemNode = problemNode; this->ac = ac;}
		virtual const char* what() const throw();
		
	protected:
		virtual const char* getExceptionErrorMessage() const = 0;
		node* getNode() { return problemNode; }
		AnnotatedCluster* getClusterTerrain() { return ac; }
		
	private: 
		node *problemNode; 
		AnnotatedCluster* ac;
};

class NodeIsHardObstacleException : public ACAException
{
	public: 
		NodeIsHardObstacleException(node* problemNode, AnnotatedCluster* ac) : ACAException(problemNode, ac) {  }

	protected:
		virtual const char* getExceptionErrorMessage() const;

};

class NodeIsAlreadyAssignedToClusterException  : public ACAException
{
	public: 
		NodeIsAlreadyAssignedToClusterException(node* problemNode, AnnotatedCluster* ac) : ACAException(problemNode, ac) {  }
		
	protected:
		virtual const char* getExceptionErrorMessage() const;
};

class ClusterFullException : public ACAException
{
	public:
		ClusterFullException(node* problemNode, AnnotatedCluster* ac) : ACAException(problemNode, ac) {  }
	
	protected:
		virtual const char* getExceptionErrorMessage() const;		
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
		AnnotatedCluster(int, int, int, int, int);
		~AnnotatedCluster() { }
		virtual bool addNode(node *) throw(NodeIsHardObstacleException, NodeIsAlreadyAssignedToClusterException, ClusterFullException); 
		virtual void addParent(node *);
		void getClusterTerrain(); 
		void getMaxClearance(); 
		void addNodesToCluster(AbstractAnnotatedMapAbstraction*);
		
	private:
		int clusterterrain, maxclearance;
		
		static unsigned int uniqueClusterIdCnt;
};

#endif