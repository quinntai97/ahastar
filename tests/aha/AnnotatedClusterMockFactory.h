/*
 *  AnnotatedClusterMockFactory.h
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERMOCKFACTORY_H
#define ANNOTATEDCLUSTERMOCKFACTORY_H

#include "AnnotatedClusterFactory.h"
#include "AnnotatedClusterAbstractionTest.h"

class AnnotatedCluster;
class AnnotatedClusterMockFactory : public IAnnotatedClusterFactory
{
	public:
		AnnotatedClusterMockFactory(AnnotatedClusterAbstractionTest* testharness) : myharness(testharness) { currentTest = 0; }
		virtual AnnotatedCluster* createCluster(int, int, int, int);
		void setTest(int current) { currentTest = current; }
		
	private:
		AnnotatedClusterAbstractionTest *myharness;
		int currentTest;
		
};

#endif
