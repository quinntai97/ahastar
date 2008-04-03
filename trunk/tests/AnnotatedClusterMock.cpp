/*
 *  AnnotatedClusterMock.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterMock.h"

void AnnotatedClusterMock::buildEntrances(AnnotatedClusterAbstraction* aca) throw(AnnotatedClusterAbstractionIsNullException)
{
	this->buildEntrancesMocker.forward(aca);
}

void AnnotatedClusterMock::addNodesToCluster(AnnotatedClusterAbstraction* aca)
{
	this->addNodesToClusterMocker.forward(aca);	
}

void AnnotatedClusterMock::addParent(node* n, AnnotatedClusterAbstraction* aca)
{
	Cluster::addParent(n);
}