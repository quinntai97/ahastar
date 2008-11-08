/*
 *  TestGraph.h
 *  hog
 *
 *  Created by Daniel Harabor on 26/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "graph.h"
#include "map.h"

using namespace CppUnit;

class TestGraph : public CPPUNIT_NS::TestFixture
{
/*  CPPUNIT_TEST_SUITE( TestGraph );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsIdenticalToWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsShorterButOtherwiseIdenticalToWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderAndShorterThanWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderButOtherwiseIdenticalToWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenNoEdgeExistsBetweenParameterEndpoints );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerButOtherwiseIdenticalToWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsShorterAndMoreNarrowThanWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsOfTheSameLengthButMoreNarrowThanWhatParametersAskFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndMoreNarrowThanWhatParametersAskedFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndWiderThanWhatParametersAskedFor );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnZeroWhenEndpointParametersAreNull );
  CPPUNIT_TEST( findAnnotatedEdgeShouldReturnTheShortestEdgeIfSeveralCandidateEdgesExistThatMatchWhatParametersAskedFor );
  CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void addEdgeToGraph(int, int, double);
	
	// positive cases
	void findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsShorterButOtherwiseIdenticalToWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsIdenticalToWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderAndShorterThanWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderButOtherwiseIdenticalToWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnTheShortestEdgeIfSeveralCandidateEdgesExistThatMatchWhatParametersAskedFor();
	
	// negative cases
	void findAnnotatedEdgeShouldReturnZeroWhenNoEdgeExistsBetweenParameterEndpoints();
	void findAnnotatedEdgeShouldReturnZeroWhenEndpointParametersAreNull();
	void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerButOtherwiseIdenticalToWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsShorterAndMoreNarrowThanWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsOfTheSameLengthButMoreNarrowThanWhatParametersAskFor();
	void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndMoreNarrowThanWhatParametersAskedFor(); 
	void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndWiderThanWhatParametersAskedFor();

	
	// throw exception
	//void findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerButOtherwiseIdenticalToWhatWasAsked
			
private:
	graph *g;
	node *from, *to;
	edge *e;
	*/
};

#endif