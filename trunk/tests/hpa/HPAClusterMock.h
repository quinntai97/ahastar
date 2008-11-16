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

#include "HPACluster.h"
#include <stdexcept>

class HPAClusterAbstraction;
class AbstractClusterAStar;

class HPAClusterMock : public HPACluster, public MOCKPP_NS::ChainableMockObject
{
	public:
		HPAClusterMock(int x, int y, int _width, int _height, AbstractClusterAStar* _alg) throw(std::invalid_argument)
		:	HPACluster(x, y, _width, _height, _alg)
			, MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("HPAClusterMock"), 0)
			, buildEntrancesMocker("buildEntrances", this)
			, addNodesToClusterMocker("addNodesToCluster", this) 
			//, addParentMocker("addParent", this)
			, removeParentMocker("removeParent", this)
		{ addParentInvocationCount=0; addParentInvocationCountActual=0;	}
		HPAClusterMock(int x, int y, int _width, int _height) 
		:	HPACluster(x, y, _width, _height)
			, MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("HPAClusterMock"), 0)
			, buildEntrancesMocker("buildEntrances", this)
			, addNodesToClusterMocker("addNodesToCluster", this) 
			//, addParentMocker("addParent", this)
			, removeParentMocker("removeParent", this)
		{ addParentInvocationCount=0; addParentInvocationCountActual=0;	}


		virtual ~HPAClusterMock() {}
		
		virtual void buildEntrances(HPAClusterAbstraction*);
		virtual void addNodesToCluster(HPAClusterAbstraction*) throw(std::invalid_argument);
		virtual void addParent(node*, HPAClusterAbstraction*) throw(std::invalid_argument);
		virtual void removeParent(int);
		
		void setExpectedAddParentParameters(node* n, HPAClusterAbstraction* hpac) { this->n = n; this->hpac = hpac; }
		void setExpectedOccurencesForAddParent(int i) { addParentInvocationCount = i;}
		virtual void verify(); 
		
		MOCKPP_NS::ChainableMockMethod<void, HPAClusterAbstraction*> buildEntrancesMocker;
		MOCKPP_NS::ChainableMockMethod<void, HPAClusterAbstraction*> addNodesToClusterMocker;
		//MOCKPP_NS::ChainableMockMethod<void, node*, HPAClusterAbstraction*> addParentMocker;
		MOCKPP_NS::ChainableMockMethod<void, int> removeParentMocker;
		
		private:
			int addParentInvocationCount;
			int addParentInvocationCountActual;
			node* n;
			HPAClusterAbstraction* hpac;
};

#endif