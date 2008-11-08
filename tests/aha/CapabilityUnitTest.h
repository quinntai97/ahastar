/*
 *  CapabilityUnitTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 19/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef CAPABILITYUNIT_H
#define CAPABILITYUNIT_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "constants.h"

class CapabilityUnit;
class unit;
class mapProvider;
class reservationProvider;
class simulationInfo;

class CapabilityUnitTest : CPPUNIT_NS::TestFixture
{
/*	CPPUNIT_TEST_SUITE( CapabilityUnitTest );
	CPPUNIT_TEST( makeMoveReturnsStayWhenNoTargetExists );
	CPPUNIT_TEST_SUITE_END();
*/	
	public:
		void setUp();
		void tearDown();

		void makeMoveReturnsStayWhenNoTargetExists();
		void makeMoveReturnsStayWhenTargetIsReached();
		void makeMoveReturnsStayWhenTargetIsUnreachable();
		void makeMoveReturnsNextStepInPlan();
		void makeMoveReturnsStayWhenParametersNull();

	private: 
		mapProvider *mprov;
		simulationInfo *sim;
		
		CapabilityUnit* cUnit;
		unit* target;
		int x, y;
		

};

#endif CAPABILITYUNIT_H