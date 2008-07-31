/*
 *  TestGraph.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 26/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestGraph.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( TestGraph );

/*void TestGraph::setUp()
{
	g = new graph();
	from = new node("");
	to = new node("");
	g->addNode(from);
	g->addNode(to);
}

void TestGraph::tearDown()
{
	delete g;
}

void TestGraph::addEdgeToGraph(int caps, int clearance, double weight)
{
	e = new edge(from->getNum(), to->getNum(), weight);
	e->setClearance(caps,clearance);
	g->addEdge(e);
}



void TestGraph::findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsIdenticalToWhatParametersAskFor()
{
	double weight = 1.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance, weight);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find identical existing annotated edge", e, expectedEdge);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find existing annotated edge using capability superset", e, expectedEdge);
}

void TestGraph::findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsShorterButOtherwiseIdenticalToWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance, weight-1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find identical existing annotated edge", e, expectedEdge);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find existing annotated edge using capability superset", e, expectedEdge);
}

void TestGraph::findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderAndShorterThanWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance+1, weight-1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find identical existing annotated edge", e, expectedEdge);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find existing annotated edge using capability superset", e, expectedEdge);	
}

void TestGraph::findAnnotatedEdgeShouldReturnAnExistingEdgeIfOneExistsWhichIsWiderButOtherwiseIdenticalToWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance+1, weight);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find identical existing annotated edge", e, expectedEdge);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to find existing annotated edge using capability superset", e, expectedEdge);	
}

void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenNoEdgeExistsBetweenParameterEndpoints()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);

	edge *expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero when no edge exists connecting target endpoints", expectedEdge == 0);	
}

// NB: if capability values are identical, this method returning zero is a warning sign something is probably wrong with cluster annotation 
// ie. we're querying about an edge connecting two endpoints which represents a shorter path with larger clearance. why didn't we find it before?
void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsShorterAndMoreNarrowThanWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance-1, weight-1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero using capability superset", expectedEdge == 0);	
}

void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsOfTheSameLengthButMoreNarrowThanWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance-1, weight);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero using capability superset", expectedEdge == 0);	
}

void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndMoreNarrowThanWhatParametersAskedFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance-1, weight+1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero using capability superset", expectedEdge == 0);	
}

// NB: another warning method. we're querying about an edge which is more optimal than the current connection between the two endpoints
void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerButOtherwiseIdenticalToWhatParametersAskFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance, weight+1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero using capability superset", expectedEdge == 0);
}

void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenExistingEdgeIsLongerAndWiderThanWhatParametersAskedFor()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
	
	addEdgeToGraph(kGround, targetClearance+1, weight+1);
		
	edge* expectedEdge = g->findAnnotatedEdge(from, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, to, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero using capability superset", expectedEdge == 0);	
}

void TestGraph::findAnnotatedEdgeShouldReturnZeroWhenEndpointParametersAreNull()
{
	double weight = 3.0;
	int targetClearance = 2;
	int targetCapability = (kTrees|kGround);
			
	edge* expectedEdge = g->findAnnotatedEdge(NULL, to, kGround, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero when from is NULL", expectedEdge == 0);

	expectedEdge = g->findAnnotatedEdge(from, NULL, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero when to is NULL", expectedEdge == 0);	

	expectedEdge = g->findAnnotatedEdge(NULL, NULL, targetCapability, targetClearance, weight);
	CPPUNIT_ASSERT_MESSAGE("failed to return zero when both from and to are NULL", expectedEdge == 0);	
}

void TestGraph::findAnnotatedEdgeShouldReturnTheShortestEdgeIfSeveralCandidateEdgesExistThatMatchWhatParametersAskedFor()
{
	addEdgeToGraph(kGround, 2, 5);
	addEdgeToGraph(kGround, 1, 4.5);


	edge* e = g->findAnnotatedEdge(from, to, kGround, 1, 50);
	
	CPPUNIT_ASSERT_EQUAL_MESSAGE("failed to return shortest edge among those existing between endpoints", 4.5, e->getWeight());
}
*/