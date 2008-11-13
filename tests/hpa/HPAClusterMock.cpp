/*
 *  HPAClusterMock.cpp
 *  hog
 *
 *  Created by dharabor on 12/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterMock.h"
#include <stdexcept>

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
	if(!(n == this->n && hpacaMap == this->hpac))
		throw std::invalid_argument("addParent called with unexpected arguments");
	this->addParentInvocationCountActual++;
	//this->addParentMocker.forward(n, hpacaMap);// (n);
}

void HPAClusterMock::removeParent(int nodeId)
{
	this->removeParentMocker.forward(nodeId);
}

void HPAClusterMock::verify() 
{ 
	if(this->addParentInvocationCountActual != addParentInvocationCount)
		throw std::invalid_argument("addParent called wrong number of times");
	ChainableMockObject::verify();
}
