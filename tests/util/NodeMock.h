/*
 *  MockNode.h
 *  hog
 *
 *  Created by dharabor on 13/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  nodeMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NODEMOCK_H
#define NODEMOCK_H

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
#include "graph.h"

class nodeAbstraction;

class nodeMock : public node, public MOCKPP_NS::ChainableMockObject
{
	public:
		nodeMock(const char* name) 
		:	node(name)
			, MOCKPP_NS::ChainableMockObject(MOCKPP_PCHAR("nodeMock"), 0)
			, cloneMocker("clone", this)
		{ }
		
		virtual graph_object *clone() const; 
		
		MOCKPP_NS::ChainableMockMethod<graph_object*> cloneMocker;
};

#endif