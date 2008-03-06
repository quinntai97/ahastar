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

class Map;
class AnnotatedCluster;

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

class CannotBuildEntranceToSelfException : public std::exception
{
	virtual const char* what() const throw() { return "tried to build an entrance using two nodes from the same cluster"; }
};

class EntranceNodeIsHardObstacleException : public std::exception
{
	virtual const char* what() const throw() { return "tried to build an entrance using two nodes from the same cluster"; }
};


class AnnotatedClusterAbstraction : public AnnotatedMapAbstraction
{
	
	#ifdef UNITTEST
		friend class AnnotatedClusterAbstractionTest;
	#endif
	
	public: 
		AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize);
		~AnnotatedClusterAbstraction();
		int getNumClusters() { return clusters.size(); } 
		int getClusterSize() { return clustersize; }
		int getNumberOfAbstractionLevels() { return abstractions.size(); }
		
	private: 
		void addEntranceToGraph(node*, node*) 
			throw(EntranceNodeIsNullException, EntranceNodesAreIdenticalException, CannotBuildEntranceFromAbstractNodeException, 
				CannotBuildEntranceToSelfException, EntranceNodeIsHardObstacleException);
		void buildClusters();
		void buildAbstractGraph();
		
		std::vector<AnnotatedCluster *> clusters;
		int clustersize;

};


#endif