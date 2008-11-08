/*
 *  NodeFactoryTest.h
 *  hog
 *
 *  Created by dharabor on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef NODEFACTORYTEST_H
#define NODEFACTORYTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class NodeFactory;

using namespace CppUnit;

class NodeFactoryTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( NodeFactoryTest );
	CPPUNIT_TEST( newNodeShouldConstructANewNodeObject );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void newNodeShouldConstructANewNodeObject();
		
	private:
		NodeFactory* nf;
};

#endif