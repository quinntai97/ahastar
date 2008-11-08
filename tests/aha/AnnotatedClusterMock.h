/*
 *  AnnotatedClusterMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERMOCK_H
#define ANNOTATEDCLUSTERMOCK_H

#include "AnnotatedCluster.h"

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

class AnnotatedClusteAbstraction;

class AnnotatedClusterMock : public AnnotatedCluster, public MOCKPP_NS::ChainableMockObject
{
	public:
		AnnotatedClusterMock(int startx, int starty, int width, int height) 
		:	AnnotatedCluster(startx, starty, width, height)
			, MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ACMock"), 0)
			, buildEntrancesMocker("buildEntrances", this)
			, addNodesToClusterMocker("addNodesToCluster", this) 
		{ }
		
		virtual void buildEntrances(AnnotatedClusterAbstraction*) throw(AnnotatedClusterAbstractionIsNullException);
		virtual void addNodesToCluster(AnnotatedClusterAbstraction*);
		virtual void addParent(node*, AnnotatedClusterAbstraction*);
		
		MOCKPP_NS::ChainableMockMethod<void, AnnotatedClusterAbstraction*> buildEntrancesMocker;
		MOCKPP_NS::ChainableMockMethod<void, AnnotatedClusterAbstraction*> addNodesToClusterMocker;

};

#endif