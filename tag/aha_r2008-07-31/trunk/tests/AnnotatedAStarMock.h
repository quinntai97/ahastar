/*
 *  AnnotatedAStarMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 7/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTARMOCK_H
#define ANNOTATEDASTARMOCK_H

#include "AnnotatedAStar.h"
#include "ExperimentManager.h"

class statCollection;
class AnnotatedAStarMock : public AbstractAnnotatedAStar
{
	public:
		AnnotatedAStarMock(int capability=0, int clearance=0) : AbstractAnnotatedAStar(capability, clearance) { curexp = NULL; }
		virtual path* getPath(graphAbstraction*, node*, node*, reservationProvider *rp=0);
		virtual const char* getName() { return "AnnotatedAStarMock"; }
		virtual bool evaluate(node* n, node* target); 
		void setCurrentTestExperiment(ExpMgrUtil::TestExperiment* exp) { curexp = exp; }
//		void logStats(statCollection* sc) { }

	private:
		ExpMgrUtil::TestExperiment* curexp;
};
		
#endif