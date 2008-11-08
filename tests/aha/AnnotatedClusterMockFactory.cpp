/*
 *  AnnotatedClusterMockFactory.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 30/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterMockFactory.h"
#include "AnnotatedClusterMock.h"

USING_NAMESPACE_MOCKPP

AnnotatedCluster* AnnotatedClusterMockFactory::createCluster(int startx, int starty, int width, int height)
{
	AnnotatedClusterMock* acm =  new AnnotatedClusterMock(startx, starty, width, height);
	
	/* TODO: horrid. replace with some decent dependency injection code when no longer suitable */
	switch(currentTest)
	{
		case 1: 
			myharness->setupMockClusterExpectationsForBuildClusterTests(acm);
			break;
		case 2:
			myharness->setupMockClusterExpectationsForBuildEntranceTests(acm);
			break;
	}

	return acm;
}
