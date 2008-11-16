/*
 *  ClusterAStarMock.h
 *  hog
 *
 *  Created by dharabor on 14/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLUSTERASTARMOCK_H
#define CLUSTERASTARMOCK_H

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
#include "ClusterAStar.h"

class ClusterAStarMock : public ClusterAStar, public MOCKPP_NS::ChainableMockObject
{
	public:
		ClusterAStarMock() : MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("ClusterAStarMock"), 0)
			, getNodesExpandedMocker("getNodesExpanded", this)
			, getNodesTouchedMocker("getNodesExpanded", this)
			, getPeakMemoryMocker("getNodesExpanded", this)
			, getSearchTimeMocker("getSearchTime", this)
			, getPathMocker("getPath", this)
		{ }
		
		void setNodesExpanded(long ne) { this->nodesExpanded = ne; }
		void setNodesTouched(long nt) { this->nodesTouched = nt; }
		void setPeakMemory(long pm) { this->peakmemory = pm;}
		void setSearchTime(double time) { this->searchtime = time; }
		
		virtual path* getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0);
		
		MOCKPP_NS::ChainableMockMethod<long> getNodesExpandedMocker;
		MOCKPP_NS::ChainableMockMethod<long> getNodesTouchedMocker;
		MOCKPP_NS::ChainableMockMethod<long> getPeakMemoryMocker;
		MOCKPP_NS::ChainableMockMethod<double> getSearchTimeMocker;
		MOCKPP_NS::ChainableMockMethod<path*, graphAbstraction*, node*, node*> getPathMocker;



};

#endif
