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



class InvalidClusterOriginCoordinatesException : public std::exception
{
	public:
		InvalidClusterOriginCoordinatesException(int xo, int yo) { xorigin=xo; yorigin=yo; }
			
		virtual const char* what() const throw();
		
	private: 
		int xorigin, yorigin;
};


class InvalidClusterDimensionsException : public std::exception
{
	public:
		InvalidClusterDimensionsException(int w, int h, int xo, int yo) { width=w; height=h; xorigin=xo; yorigin=yo; }
			
		virtual const char* what() const throw();
		
	private: 
		int width, height, xorigin, yorigin;
};

class AnnotatedMapAbstractionIsNullException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

class AnnotatedClusterException : public std::exception
{
	public:
		AnnotatedClusterException(node* problemNode, AnnotatedCluster* ac)  { this->problemNode = problemNode; this->ac = ac;}
		virtual const char* what() const throw();
		
	protected:
		virtual const char* getExceptionErrorMessage() const = 0;
		node* getNode() { return problemNode; }
		AnnotatedCluster* getClusterTerrain() { return ac; }
		
	private: 
		node *problemNode; 
		AnnotatedCluster* ac;
};

class NodeIsHardObstacleException : public AnnotatedClusterException
{
	public: 
		NodeIsHardObstacleException(node* problemNode, AnnotatedCluster* ac) : AnnotatedClusterException(problemNode, ac) {  }

	protected:
		virtual const char* getExceptionErrorMessage() const;

};

class NodeIsAlreadyAssignedToClusterException  : public AnnotatedClusterException
{
	public: 
		NodeIsAlreadyAssignedToClusterException(node* problemNode, AnnotatedCluster* ac) : AnnotatedClusterException(problemNode, ac) {  }
		
	protected:
		virtual const char* getExceptionErrorMessage() const;
};

class ClusterFullException : public AnnotatedClusterException
{
	public:
		ClusterFullException(node* problemNode, AnnotatedCluster* ac) : AnnotatedClusterException(problemNode, ac) {  }
	
	protected:
		virtual const char* getExceptionErrorMessage() const;		
};

class NodeIsNullException : public AnnotatedClusterException
{
	public: 
		NodeIsNullException(node* problemNode, AnnotatedCluster* ac) : AnnotatedClusterException(problemNode, ac) {  }

	protected:
		virtual const char* getExceptionErrorMessage() const;
};


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
		AnnotatedCluster(int, int, int, int) throw(InvalidClusterDimensionsException, InvalidClusterOriginCoordinatesException);
		~AnnotatedCluster() { }
		virtual bool addNode(node *) throw(NodeIsAlreadyAssignedToClusterException, ClusterFullException, NodeIsNullException); 
		virtual void addParent(node *);
		virtual void addNodesToCluster(AbstractAnnotatedMapAbstraction*);
		
	private:
		static unsigned int uniqueClusterIdCnt;
};

#endif