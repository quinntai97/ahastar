/*
 *  AnnotatedClusterAbstraction.cpp
 *  hog
	
	Extends a HPA Cluster in several ways:
		- Is concerned with annotated nodes
		- each cluster must be free of hard obstacles (ie. intraversable nodes). <- implicit if annotations are OK
		- each cluster must have a rectangular or square shape <- but, if these are fscked, we might try to add hard obstacles; need exception
		- each node assigned to the cluster will not be assigned to some other cluster
		- each node in the cluster will not have a larger clearance than the origin node (node at top-left corner of the cluster);
			the clearance value we use for this test is the superset of all single terrain types (currently, kGround|kTrees)
 *
 *  Created by Daniel Harabor on 22/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstraction.h"
#include "AnnotatedCluster.h"
#include "clusterAbstraction.h"
#include "AHAConstants.h"
#include "AnnotatedAStar.h"
#include <sstream>


AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize) 
	: AnnotatedMapAbstraction(m, searchalg)
{
	this->clustersize = clustersize;
	buildClusters();
	return;
}

AnnotatedClusterAbstraction::~AnnotatedClusterAbstraction()
{
	clusters.clear();
}

void AnnotatedClusterAbstraction::buildClusters()
{	
	int mapwidth = this->getMap()->getMapWidth();
	int mapheight= this->getMap()->getMapHeight();

	/* need to split the map into fixed-size cluster areas that will form the basis of our abstract graph building later */
	for(int x=0; x<mapwidth; x+=clustersize)
		for(int y=0; y<mapheight; y+= clustersize)
		{	
			int cwidth=clustersize;
			if(x+cwidth > mapwidth)
				cwidth = mapwidth - x;
			int cheight=clustersize;
			if(y+cheight > mapheight)
				cheight = mapheight - y;
				
			clusters.push_back( new AnnotatedCluster (x, y, cwidth, cheight) );
		}
}

void AnnotatedClusterAbstraction::buildAbstractGraph()
{	
	abstractions.push_back(new graph());
}
