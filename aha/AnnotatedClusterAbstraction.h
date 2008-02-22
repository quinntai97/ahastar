/*
 *  AnnotatedClusterAbstraction.h
 *  hog
 *
 *  Created by Daniel Harabor on 22/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERABSTRACTION_H
#define ANNOTATEDCLUSTERABSTRACTION_H

#include "AnnotatedMapAbstraction.h"

class Map;
class Cluster;

class AnnotatedClusterAbstraction : public AnnotatedMapAbstraction
{
	
	#ifdef UNITTEST
		friend class AnnotatedClusterAbstractionTest;
	#endif
	
	public: 
		AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg);
		~AnnotatedClusterAbstraction();
		
	private: 
		Cluster* buildCluster(int, int);
		std::vector<Cluster *> clusters;

};

#endif