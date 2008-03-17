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
		AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize);
		~AnnotatedClusterAbstraction() { clusters.clear(); };
		virtual void buildClusters();
		virtual int getClusterSize() { return clustersize; } 
		virtual int getNumClusters() { return clusters.size(); } 
		virtual AnnotatedCluster* getCluster(int cid);

	protected: 
		virtual void addCluster(AnnotatedCluster* ac);
		virtual int getNumberOfAbstractionLevels() { return abstractions.size(); }
		
	private:
		int clustersize;
		std::vector<AnnotatedCluster *> clusters;
	
};


#endif