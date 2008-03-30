/*
 *  AnnotatedClusterAbstractionMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 13/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ANNOTATEDCLUSTERABSTRACTIONMOCK_H
#define ANNOTATEDCLUSTERABSTRACTIONMOCK_H

// need to undefine conflicting macros (from Apple's AssertMacros.h) for mockpp to work
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

#include "AnnotatedClusterAbstraction.h"

USING_NAMESPACE_MOCKPP

class AnnotatedClusterAbstractionMock :  public AnnotatedClusterAbstraction, public MOCKPP_NS::ChainableMockObject
{
	public:
		AnnotatedClusterAbstractionMock(Map* m, AbstractAnnotatedAStar* aastar, int clustersize)
			: MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ACAMock"), 0)
			, buildClustersMocker("buildClusters", this)
			, getClusterMocker("getCluster", this)
			, AnnotatedClusterAbstraction(m, aastar, clustersize) 
		{	
			abstractions.push_back(new graph());
		}
				
		virtual void buildClusters(); 
		virtual AnnotatedCluster* getCluster(int cid) 
		{  return getClusterMocker.forward(cid); }

		MOCKPP_NS::ChainableMockMethod<void> buildClustersMocker;
		MOCKPP_NS::ChainableMockMethod<AnnotatedCluster*, int> getClusterMocker;
		
};

#endif