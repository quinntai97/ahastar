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

AnnotatedClusterAbstractionMock::AnnotatedClusterAbstractionMock(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize)
	: AnnotatedClusterAbstraction(m, searchalg, clustersize)
{ 
	AnnotatedMapAbstractionMock::loadClearanceInfo(std::string(m->getMapName()), this);
	abstractions.push_back(new graph());
}


void AnnotatedClusterAbstractionMock::buildClusters()
{

}
