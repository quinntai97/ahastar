/*
 *  EdgeTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 4/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "EdgeTest.h"
#include "graph.h"
#include "AHAConstants.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EdgeTest );

void EdgeTest::setUp()
{	
	e = new edge(1, 2, 1.0);
}

void EdgeTest::tearDown()
{
	delete e;
}

void EdgeTest::setClearanceShouldStoreAGivenCapabilityAndItsCorrespondingClearanceValue()
{
	int expectedClearance = 1;
	int expectedCapability = kGround;
	
	e->setClearance(expectedCapability, expectedClearance);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge capability value (kGround)", expectedCapability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge clearance value (kGround)", expectedClearance, e->getClearance(expectedCapability));

	expectedCapability = kTrees;
	e->setClearance(expectedCapability, expectedClearance);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge capability value (kTrees)", expectedCapability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge clearance value (kTrees)", expectedClearance, e->getClearance(expectedCapability));
	
	expectedCapability = (kGround|kTrees);
	e->setClearance(expectedCapability, expectedClearance);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge capability value (kGround|kTrees)", expectedCapability, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to store edge clearance value (kGround|kTrees)", expectedClearance, e->getClearance(expectedCapability));
}

void EdgeTest::setClearanceShouldDoNothingWhenCapabilityIsNotValid()
{
	int testClearance = 1;
	int testCapability = kWater;
	
	int previousCapability = e->getCapability();
	e->setClearance(testCapability, testClearance);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly stored clearance value for invalid capability", 0, e->getClearance(testCapability));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly stored invalid capability", previousCapability, e->getCapability());
}

void EdgeTest::constructorShouldInitialiseClearanceAndCapabilityValuesToZero()
{
	CPPUNIT_ASSERT_EQUAL_MESSAGE("default capabilty not initialised to zero", 0, e->getCapability());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("default clearance for default capability not initialised to zero", 0, e->getClearance(e->getCapability()));
	
}

void EdgeTest::setClearanceShouldDoNothingWhenClearanceValueIsLessThanOrEqualToZero()
{
	int initialCV = 1;
	int testCapability = kGround;
	e->setClearance(testCapability, initialCV);
	
	int testCV = -1;
	e->setClearance(testCapability, testCV);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly updated clearance with a value < 0", initialCV, e->getClearance(testCapability));
	
	testCV=0;
	e->setClearance(testCapability, testCV);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly updated clearance with a value == 0", initialCV, e->getClearance(testCapability));
}

void EdgeTest::getClearanceShouldReturnZeroGivenACapabilityThatDoesNotMatchTheStoredCapability()
{
	int initialCV = 1;
	int initialCapability = kGround;
	e->setClearance(initialCapability, initialCV);
	
	
	int testCapability = kWater;
	CPPUNIT_ASSERT_EQUAL_MESSAGE("incorrectly returned a non-zero clearance value for an invalid capability", 0, e->getClearance(testCapability));
	
}

void EdgeTest::cloneShouldDeepCopyEdgeAnnotations()
{
	int initialCV = 1;
	int initialCapability = kGround;
	e->setClearance(initialCapability, initialCV);
	
	edge* eclone = dynamic_cast<edge*>(e->clone());	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to clone clearance value", initialCV, eclone->getClearance(initialCapability));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to clone capability value", initialCapability, eclone->getCapability());
	
	delete eclone;
}

void EdgeTest::getClearanceShouldReturnZeroGivenACapabilityThatIsASubsetOfTheStoredCapability()
{
	int initialCV = 1;
	int initialCapability = kGround|kTrees;
	e->setClearance(initialCapability, initialCV);
	
	int expectedResult = 0;

	CPPUNIT_ASSERT_EQUAL(expectedResult, e->getClearance(kGround));
}