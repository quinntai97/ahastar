/*
 *  AnnotatedClusterAbstractionMock.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 13/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstractionMock.h"
#include "AnnotatedMapAbstractionMock.h"
#include "TestConstants.h"

#include <mockpp/mockpp.h>
#include <mockpp/chaining/ChainableMockObject.h> 
#include <mockpp/chaining/CountedChainableMethod.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>


void AnnotatedClusterAbstractionMock::buildClusters()
{
	this->buildClustersMocker.forward();
	std::string filename(this->getMap()->getMapName());
	
	loadClusterAnnotations(filename, this);
	
}

void AnnotatedClusterAbstractionMock::loadClusterAnnotations(const std::string& filename, AbstractAnnotatedMapAbstraction* aca)
{
	if(filename.compare("annotatedcluster.map"))
	{
		int clusterids[6][9] = 
			{	
				{0,0,0,0,0,2,2,2,2},
				{0,0,0,0,0,2,2,2,2},
				{0,0,0,0,0,2,2,2,2},
				{0,0,0,0,0,2,2,2,2},
				{0,0,0,0,0,2,2,2,2},
				{1,1,1,1,1,3,3,3,3}
			};
					
		for(int x=0; x<aca->getMap()->getMapWidth(); x++)
			for(int y=0; y<aca->getMap()->getMapHeight(); y++)
			{
				node* current = aca->getNodeFromMap(x,y);
				current->setParentCluster(clusterids[y][x]);
			}
	}
}