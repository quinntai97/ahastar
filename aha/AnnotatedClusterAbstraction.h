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
#include "AnnotatedAStar.h"

class Map;
class AnnotatedCluster;

class AnnotatedClusterAbstraction : public AnnotatedMapAbstraction
{
	
	#ifdef UNITTEST
		friend class AnnotatedClusterAbstractionTest;
	#endif
	
	public: 
		AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize);
		~AnnotatedClusterAbstraction();
		int getNumClusters() { return clusters.size(); } 
		int getClusterSize() { return clustersize; }
		int getNumberOfAbstractionLevels() { return abstractions.size(); }
		
	private: 
		void buildClusters();
		void identifyHorizontalEntrances();
		void identifyVeritcalEntrances();
		void buildAbstractGraph();
		
		std::vector<AnnotatedCluster *> clusters;
		int clustersize;

};


#endif