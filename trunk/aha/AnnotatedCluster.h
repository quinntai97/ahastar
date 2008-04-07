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
		NodeIsNullException() : AnnotatedClusterException(NULL, NULL) { }
		NodeIsNullException(node* problemNode, AnnotatedCluster* ac) : AnnotatedClusterException(problemNode, ac) {  }

	protected:
		virtual const char* getExceptionErrorMessage() const;
};

class CannotBuildEntranceFromAbstractNodeException : public std::exception
{
		virtual const char* what() const throw() { return "tried to build an entrance using a parameter node which is already part of an abstract graph"; }
};


class EntranceNodeIsHardObstacleException : public std::exception
{
	virtual const char* what() const throw() { return "tried to build an entrance using two nodes from the same cluster"; }
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

class InvalidClearanceParameterException : public std::exception
{	
	virtual const char* what() const throw() { return "clearance parameter <=0 or otherwise inconsistent with entrance endpoints"; }	
};


class EntranceNodeIsNotTraversable : public std::exception
{	
	virtual const char* what() const throw() { "entrance's capability clearance parameter does not match endpoint node clearance"; }	
};

class ValidateMapAbstractionException : public std::exception
{
	virtual const char* what() const throw() { "Map abstraction parameter cannot be null"; }
};

class ValidateTransitionEndpointsException : public std::exception
{
	public:
		ValidateTransitionEndpointsException(const char* error) : errmsg(error) { };
		virtual const char* what() const throw() { return errmsg; }	

	private:
		const char* errmsg;
};

class EntranceNodesAreNotAdjacentException : public std::exception
{
		virtual const char* what() const throw() { return "tried to build an entrance using two non-adjacent nodes"; }	
};

class InvalidTransitionWeightException : public std::exception
{
		virtual const char* what() const throw() { return "tried to build an entrance with weight <= 0"; }	
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
		virtual void addParent(node *, AnnotatedClusterAbstraction*);
		virtual void addNodesToCluster(AnnotatedClusterAbstraction*);
		virtual void buildEntrances(AnnotatedClusterAbstraction*) throw (AnnotatedClusterAbstractionIsNullException);
		
	protected:
		virtual bool addNode(node *) throw(NodeIsAlreadyAssignedToClusterException, ClusterFullException, NodeIsNullException); 
		virtual void addEntrance(node*, node*, int, int, AnnotatedClusterAbstraction*)
			throw(InvalidClearanceParameterException, EntranceNodeIsNotTraversable);
					
		virtual void buildVerticalEntrances(int, AnnotatedClusterAbstraction*);
		virtual void buildHorizontalEntrances(int, AnnotatedClusterAbstraction*);
		
		
	private:		
		void validateMapAbstraction(AnnotatedClusterAbstraction*) throw(ValidateMapAbstractionException);
		void validateTransitionEndpoints(node*, node*) throw(ValidateTransitionEndpointsException);
		void addEndpointsToAbstractGraph(node*, node*, AnnotatedClusterAbstraction*) 
			throw(EntranceNodesAreNotAdjacentException, CannotBuildEntranceToSelfException, CannotBuildEntranceFromAbstractNodeException);
		void addTransitionToAbstractGraph(node* from, node* to, int capability, int clearance, double weight, AnnotatedClusterAbstraction* aca) throw (InvalidTransitionWeightException);
		void connectEntranceEndpoints(node* n1, node* n2, int capability, AnnotatedClusterAbstraction* aca);
		void getPathClearance(path *p, int& capability, int& clearance);

		static unsigned int uniqueClusterIdCnt;
		
};


#endif