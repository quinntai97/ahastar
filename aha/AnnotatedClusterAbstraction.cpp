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
		
	Cluster *c = new Cluster(-1, -1, -1, -1, -1, -1, -1);
	clusters.push_back(c);
	
	return c;
}