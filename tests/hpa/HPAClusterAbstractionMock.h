/*
 *  HPAClusterAbstractionMock.h
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTERABSTRACTIONMOCK_H
#define HPACLUSTERABSTRACTIONMOCK_H

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

#include "HPAClusterAbstraction.h"

class HPAClusterAbstractionMock : public HPAClusterAbstraction, public MOCKPP_NS::ChainableMockObject
{
	public:
		HPAClusterAbstractionMock(Map* m, IHPAClusterFactory* cf, INodeFactory* nf, 
				IEdgeFactory* ef) 
			throw(std::invalid_argument)
		:	HPAClusterAbstraction(m, cf, nf, ef),
			 MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("HPAClusterAbstractionMock"), 0)
			//, buildClustersMocker("buildClusters", this)
			//, getClusterMocker("getCluster", this)
		{
		}
		
		~HPAClusterAbstractionMock() 
		{ 			
		}
};

#endif
