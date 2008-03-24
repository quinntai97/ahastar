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
	
	if(filename.compare("annotatedcluster.map"))
	{
		int clusterids[6][9] = 
			{	
				{0,0,0,0,0,1,1,1,1},
				{0,0,0,0,0,1,1,1,1},
				{0,0,0,0,0,1,1,1,1},
				{0,0,0,0,0,1,1,1,1},
				{0,0,0,0,0,1,1,1,1},
				{2,2,2,2,2,3,3,3,3}
			};
					
		for(int x=0; x<this->getMap()->getMapWidth(); x++)
			for(int y=0; y<this->getMap()->getMapHeight(); y++)
			{
				node* current = this->getNodeFromMap(x,y);
				current->setParentCluster(clusterids[y][x]);
			}
	}
}
