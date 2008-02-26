/*
 *  AnnotatedClusterAbstraction.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 22/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstraction.h"
#include "clusterAbstraction.h"

const char* NodeTerrainDoesNotMatchClusterOriginException::what() const throw()
{
		return "Tried to assign a node with terrain different from cluster origin";
}

const char* NodeClearanceGreaterThanClusterOriginException::what() const throw()
{
		return "Tried to assign a node with terrain different from cluster origin";
}

const char* NodeIsHardObstacleException::what() const throw()
{
		return "Tried to assign a node which is hard obstacle to a cluster";
}


AnnotatedCluster::AnnotatedCluster(int startx, int starty, AnnotatedClusterAbstraction* aMap) 
	throw(NodeIsHardObstacleException, NodeTerrainDoesNotMatchClusterOriginException, NodeClearanceGreaterThanClusterOriginException) 
		:  Cluster(0,0,0,0,0,0,0)
{
	node* origin = aMap->getNodeFromMap(startx, starty);
	clusterterrain = origin->getTerrainType();
	maxclearance = origin->getClearance(clusterterrain);
	
	if(maxclearance <= 0)
		throw NodeIsHardObstacleException();
		
	setClusterId(aMap->getNumClusters());
	setVOrig(starty);
	setHOrig(startx);
	calculateClusterHeight();
	calculateClusterWidth();
	addNodesToCluster(aMap);
}

void AnnotatedCluster::calculateClusterHeight()
{ 
	return;
}

void AnnotatedCluster::calculateClusterWidth()
{
	return;
}

void AnnotatedCluster::addNode(node* mynode)
{
	if(mynode->getClearance(mynode->getTerrainType()) <= 0)
		return;
	
}

void AnnotatedCluster::addParent(node* parentnode)
{
	return;
}

void AnnotatedCluster::addNodesToCluster(AnnotatedClusterAbstraction* aMap)
{
	node* toAdd = aMap->getNodeFromMap(this->getHOrig(), this->getVOrig());
	Cluster::addNode(toAdd->getNum());
}

AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg) : AnnotatedMapAbstraction(m, searchalg)
{
	return;
}

AnnotatedClusterAbstraction::~AnnotatedClusterAbstraction()
{
	clusters.clear();
}

AnnotatedCluster* AnnotatedClusterAbstraction::buildCluster(int startx, int starty)
{
	node* startloc = this->getNodeFromMap(startx,starty);
	if(!startloc)
		return NULL;
	
	/* make sure we don't try to create a cluster which originates on a hard obstacle (ie. non traversable tile) */
	if(startloc->getClearance(startloc->getTerrainType()) == 0) 
		return NULL;
							
	AnnotatedCluster *c = new AnnotatedCluster(startx, starty, this);
	clusters.push_back(c);
	
//	addNodesToCluster(c);
	
	return c;
}
