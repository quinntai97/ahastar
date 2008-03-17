/*
 *  AnnotatedClusterAbstractionMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 13/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ANNOTATEDCLUSTERABSTRACTIONMOCK_H
#define ANNOTATEDCLUSTERABSTRACTIONMOCK_H

#include "AnnotatedClusterAbstraction.h"

class AnnotatedClusterAbstractionMock : public AnnotatedClusterAbstraction
{
	public:
		AnnotatedClusterAbstractionMock(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize);
		~AnnotatedClusterAbstractionMock() { }

		virtual void buildClusters(); 
};

#endif