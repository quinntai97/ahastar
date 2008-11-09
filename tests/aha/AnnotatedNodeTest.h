/*
 *  AnnotatedNodeTest.h
 *  hog
 *
 *	Tests for HOG's "Node" class. Focus:
		- Test Annotations; each node can be annotated with clearance values for different types of terrain. 
		- Test TerrainType; each node is annotated with the terrain type of the corresponding map tile it represents.
		
 *  Created by Daniel Harabor on 28/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDNODETEST_H
#define ANNOTATEDNODETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

class AnnotatedNode;
class graph;
class edge;

class AnnotatedNodeTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( AnnotatedNodeTest );
  CPPUNIT_TEST( TerrainAnnotationsTest );
  CPPUNIT_TEST( setClearanceFailsWhenNodeTerrainNotValid );
  CPPUNIT_TEST( setParentClusterFailsWhenClusterIdLessThanZero );
  CPPUNIT_TEST( setParentClusterStoresClusterIdWhenClusterIdMoreThanZero );
  CPPUNIT_TEST( setParentClusterStoresClusterIdWhenClusterIdEqualsZero );
  CPPUNIT_TEST( cloneShouldDeepCopyNodeAndAllAnnotationsExceptParentClusterId );
  CPPUNIT_TEST( cloneShouldNotDeepCopyEdges );
  
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
	/* fixture stuff */
	void setUp();
	void tearDown();

	/* test cases */
	void TerrainAnnotationsTest();
	void setClearanceFailsWhenNodeTerrainNotValid();
	void setParentClusterFailsWhenClusterIdLessThanZero();
	void setParentClusterStoresClusterIdWhenClusterIdMoreThanZero();
	void setParentClusterStoresClusterIdWhenClusterIdEqualsZero();
	void cloneShouldDeepCopyNodeAndAllAnnotationsExceptParentClusterId();
	void cloneShouldNotDeepCopyEdges();
	
	void addEdgeToNode(int, int, double);
	
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


private:
	/* test data */
	graph* g;
	AnnotatedNode *n;
	AnnotatedNode *from, *to;
	edge* e;
	int terrains[3];
	int clval[4];

};

#endif