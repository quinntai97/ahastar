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

AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg) : AnnotatedMapAbstraction(m, searchalg)
{
	return;
}

AnnotatedClusterAbstraction::~AnnotatedClusterAbstraction()
{
	clusters.clear();
}

Cluster* AnnotatedClusterAbstraction::buildCluster(int startx, int starty)
{
	node* startloc = this->getNodeFromMap(startx,starty);
	if(!startloc)
		return NULL;
	
	/* make sure we don't try to create a cluster which originates on a hard obstacle (ie. non traversable tile) */
	if(startloc->getClearance(startloc->getTerrainType()) == 0) 
		return NULL;
							
	Cluster *c = new Cluster((int)clusters.size(), -1, -1, startx, starty, -1, -1);
	clusters.push_back(c);
	
	addNodesToCluster(c);
	
	return c;
}

void AnnotatedClusterAbstraction::addNodesToCluster(Cluster* target)
{
	node* toAdd = getNodeFromMap(target->getHOrig(), target->getVOrig());
	target->addNode(toAdd->getNum());
}

