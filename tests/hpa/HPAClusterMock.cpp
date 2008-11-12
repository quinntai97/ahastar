/*
 *  HPAClusterMock.cpp
 *  hog
 *
 *  Created by dharabor on 12/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterMock.h"

void HPAClusterMock::buildEntrances(HPAClusterAbstraction* hpacaMap) 
{
	this->buildEntrancesMocker.forward(hpacaMap);
}

void HPAClusterMock::addNodesToCluster(HPAClusterAbstraction* hpacaMap) throw(std::invalid_argument)
{
	this->addNodesToClusterMocker.forward(hpacaMap);	
}

void HPAClusterMock::addParent(node* n, HPAClusterAbstraction* hpacaMap)
{
	this->addParentMocker.forward(n, hpacaMap);// (n);
	//HPACluster::addParent(n, hpacaMap);
}

void HPAClusterMock::removeParent(int nodeId)
{
	this->removeParentMocker.forward(nodeId);
}