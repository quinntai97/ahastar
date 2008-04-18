/*
 *  AnnotatedHierarchicalAStar.h
 *  hog
 *
 *  Created by Daniel Harabor on 7/04/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDHIERARCHICALASTAR_H
#define ANNOTATEDHIERARCHICALASTAR_H

#include "AnnotatedAStar.h"

class AnnotatedHierarchicalAStar : public AnnotatedAStar
{
	#ifdef UNITTEST
		friend class AnnotatedHierarchicalAStarTest;
	#endif
	
	public:	
		virtual const char* getName() { return "AnnotatedHierarchicalAStar"; }
		virtual path* getPath(graphAbstraction* aMap, node* from, node* to, reservationProvider *rp=0);
		int getInsertNodesExpanded() { return insertNodesExpanded; }
		int getInsertNodesTouched() { return insertNodesTouched; }
		int getInsertPeakMemory() { return insertPeakMemory; }
		double getInsertSearchTime() { return insertSearchTime; }
		int getAbsNodesExpanded() { return absNodesExpanded; }
		int getAbsNodesTouched() { return absNodesTouched; }
		int getAbsPeakMemory() { return absPeakMemory; }
		double getAbsSearchTime() { return absSearchTime; }

		
	protected:
		virtual path* getAbstractPath(graphAbstraction* aMap, node* from, node* to) 
		{	
			return AnnotatedAStar::getPath(aMap, from, to); 			
		}
		virtual bool evaluate(node* n, node* target);
		
	private:		
		int insertNodesExpanded, absNodesExpanded;
		int insertNodesTouched, absNodesTouched;
		int insertPeakMemory, absPeakMemory;
		double insertSearchTime, absSearchTime;
};

#endif

