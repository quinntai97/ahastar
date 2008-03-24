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
class node;
class AnnotatedClusterAbstractionIsNullException : public std::exception
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

class EntranceNodeIsNullException : public std::exception
{
	virtual const char* what() const throw() { return "tried to create an entrance but one of the parameters is a null node"; }
};

class EntranceNodesAreIdenticalException : public std::exception
{
	virtual const char* what() const throw() { return "tried to create an entrance but the parameter nodes both point to same node object"; }
};

class CannotBuildEntranceFromAbstractNodeException : public std::exception
{
		virtual const char* what() const throw() { return "tried to build an entrance using a parameter node which is already part of an abstract graph"; }
};


class EntranceNodeIsHardObstacleException : public std::exception
{
	virtual const char* what() const throw() { return "tried to build an entrance using two nodes from the same cluster"; }
};

class EntranceNodesAreNotAdjacentException : public std::exception
{
	virtual const char* what() const throw() { return "tried to build an entrance using two non-adjacent nodes"; }	
};

class EntranceException : public std::exception
{
	public:
		EntranceException(node* n1, node* n2) : endpoint1(n1), endpoint2(n2) { message=0; }
		virtual ~EntranceException() throw() { if(message)  delete message; } 
		virtual const char* what() const throw() = 0;

	protected:
		std::string* message;
		node* endpoint1, *endpoint2;
};

class CannotBuildEntranceToSelfException : public EntranceException
{
	public:
		CannotBuildEntranceToSelfException(node*, node*);
		virtual const char* what() const throw();
};

class InvalidCapabilityParameterException : public std::exception
{	
	virtual const char* what() const throw() { return "no agent exists with specified capability"; }	
};

class AnnotatedClusterAbstraction;
class AnnotatedCluster : public Cluster
{

	#ifdef UNITTEST
		friend class AnnotatedClusterTest;
		friend class exceptionThrownHelper;
	#endif

	public:
		AnnotatedCluster(int, int, int, int) throw(InvalidClusterDimensionsException, InvalidClusterOriginCoordinatesException);
		~AnnotatedCluster() { }
		virtual bool addNode(node *) throw(NodeIsAlreadyAssignedToClusterException, ClusterFullException, NodeIsNullException); 
		virtual void addParent(node *);
		virtual void addNodesToCluster(AnnotatedClusterAbstraction*);
		
	protected:
		virtual void addInterEdge(node*, node*, AnnotatedClusterAbstraction*) 
			throw(EntranceNodeIsNullException, EntranceNodesAreIdenticalException, CannotBuildEntranceFromAbstractNodeException, 
				CannotBuildEntranceToSelfException, EntranceNodeIsHardObstacleException, EntranceNodesAreNotAdjacentException);
		virtual void buildVerticalEntrances(int, AnnotatedClusterAbstraction*);
		
	private:
		void addEdgeToAbstractGraph(node*, node*, int, int, double, AnnotatedClusterAbstraction*);
		static unsigned int uniqueClusterIdCnt;
		
};


#endif