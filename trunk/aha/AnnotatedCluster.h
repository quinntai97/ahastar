/*
 *  AnnotatedCluster.h
 *  hog
 *
 *  Created by Daniel Harabor on 6/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTER_H
#define ANNOTATEDCLUSTER_H

#include "clusterAbstraction.h"
#include <exception>

class AnnotatedCluster;
class AnnotatedMapAbstractionIsNullException : public std::exception
{
	public:
		virtual const char* what() const throw();
};

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

class AbstractAnnotatedMapAbstraction;
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