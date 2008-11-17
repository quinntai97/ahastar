/*
 *  HPAStar.h
 *  hog
 *
 *  Created by dharabor on 17/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPASTAR2_H
#define HPASTAR2_H

#include "ClusterAStar.h"

class HPAClusterAbstraction;

class HPAStar2 : public ClusterAStar
{
	public:
		HPAStar2(bool _refine, bool _fastRefinement) : refineAbstractPath(_refine), fastRefinement(_fastRefinement) {}
		HPAStar2() {  refineAbstractPath = true; fastRefinement = false; }
		virtual ~HPAStar2() {}
		virtual const char* getName() { return "HPAStar2"; };
		virtual path *getPath(graphAbstraction *aMap, node *from, node *to, reservationProvider *rp = 0);	
		
		long getInsertNodesExpanded() { return insertNodesExpanded; }
		long getInsertNodesTouched() { return insertNodesTouched; }
		long getInsertPeakMemory() { return insertPeakMemory; }
		double getInsertSearchTime() { return insertSearchTime; }
		virtual void logFinalStats(statCollection* sc);
		
		void setRefineAbstractPathFlag(bool _refine) { refineAbstractPath = _refine; }
		bool getRefineAbstractPathFlag() { return refineAbstractPath; }

		void setFastRefinement(bool _fastRefinement) { refineAbstractPath = true;  fastRefinement = _fastRefinement; }
		bool getFastRefinement() { return fastRefinement; }

		
	protected:
		virtual path* refinePath(path* abspath, HPAClusterAbstraction* hpamap, ClusterAStar& castar);
				
	private:		
		long insertNodesExpanded;
		long insertNodesTouched;
		long insertPeakMemory;
		double insertSearchTime;
		bool refineAbstractPath;
		bool fastRefinement; // should we use the path cache in HPAClusterAbstraction to speed up refinement?
		
		void updateMetrics(ClusterAStar& castar);  
		void resetMetrics();
		void printPath(path* p);
};

#endif
