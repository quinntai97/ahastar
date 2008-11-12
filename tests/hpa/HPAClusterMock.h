/*
 *  HPAClusterMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERMOCK_H
#define HPACLUSTERMOCK_H

#include "HPACluster.h"

#ifdef verify
#undef verify
#endif
#ifdef check
#undef check
#endif
#ifdef require
#undef require
#endif

#include <mockpp/mockpp.h>
#include <mockpp/chaining/ChainableMockObject.h> 
#include <mockpp/chaining/CountedChainableMethod.h>
#include <mockpp/chaining/ChainingMockObjectSupport.h>
#include <stdexcept>

class HPAClusterAbstraction;

class HPAClusterMock : public HPACluster, public MOCKPP_NS::ChainableMockObject
{
	public:
		HPAClusterMock(int startx, int starty, int width, int height) 
		:	HPACluster(startx, starty, width, height)
			, MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("HPAClusterMock"), 0)
			, buildEntrancesMocker("buildEntrances", this)
			, addNodesToClusterMocker("addNodesToCluster", this) 
			, addParentMocker("addParent", this)
			, removeParentMocker("removeParent", this)
		{ }
		
		virtual void buildEntrances(HPAClusterAbstraction*);
		virtual void addNodesToCluster(HPAClusterAbstraction*) throw(std::invalid_argument);
		virtual void addParent(node*, HPAClusterAbstraction*);
		virtual void removeParent(int);
		
		MOCKPP_NS::ChainableMockMethod<void, HPAClusterAbstraction*> buildEntrancesMocker;
		MOCKPP_NS::ChainableMockMethod<void, HPAClusterAbstraction*> addNodesToClusterMocker;
		MOCKPP_NS::ChainableMockMethod<void, node*, HPAClusterAbstraction*> addParentMocker;
		MOCKPP_NS::ChainableMockMethod<void, int> removeParentMocker;
};

#endif