/*
 *  EdgeTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 4/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EDGETEST_H
#define EDGETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "graph.h"
#include "map.h"

using namespace CppUnit;

class edge;
class EdgeTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( EdgeTest );
	CPPUNIT_TEST( setClearanceShouldStoreAGivenCapabilityAndItsCorrespondingClearanceValue );
//	CPPUNIT_TEST( setClearanceShouldDoNothingWhenCapabilityIsNotValid );
	CPPUNIT_TEST( constructorShouldInitialiseClearanceAndCapabilityValuesToZero );
	CPPUNIT_TEST( setClearanceShouldDoNothingWhenClearanceValueIsLessThanOrEqualToZero );
	CPPUNIT_TEST( getClearanceShouldReturnZeroGivenACapabilityThatDoesNotMatchTheStoredCapability );
	CPPUNIT_TEST( getClearanceShouldReturnZeroGivenACapabilityThatIsASubsetOfTheStoredCapability );
	CPPUNIT_TEST( cloneShouldDeepCopyEdgeAnnotations );
	CPPUNIT_TEST_SUITE_END();
	
	public:
		void setUp();
		void tearDown();
		void setClearanceShouldStoreAGivenCapabilityAndItsCorrespondingClearanceValue();
		void setClearanceShouldDoNothingWhenCapabilityIsNotValid();
		void constructorShouldInitialiseClearanceAndCapabilityValuesToZero();
		void setClearanceShouldDoNothingWhenClearanceValueIsLessThanOrEqualToZero();
		void getClearanceShouldReturnZeroGivenACapabilityThatDoesNotMatchTheStoredCapability();
		void getClearanceShouldReturnZeroGivenACapabilityThatIsASubsetOfTheStoredCapability();
		void getClearanceShouldReturnStoredClearanceGivenACapabilityThatIsASupersetOfTheStoredCapability(); // TODO: implement this test
		void cloneShouldDeepCopyEdgeAnnotations();
	
	private:
		edge* e;


};
#endif