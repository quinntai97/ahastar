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

class AnnotatedAStarMock : public AbstractAnnotatedAStar
{
	public:
		AnnotatedAStarMock() { curexp = NULL; }
		virtual path* getPath(graphAbstraction*, node*, node*, int, int);
		virtual const char* getName() { return "AnnotatedAStarMock"; }
		virtual bool evaluate(node* n, node* target, edge* e); 
		void setCurrentTestExperiment(ExpMgrUtil::TestExperiment* exp) { curexp = exp; }

	private:
		ExpMgrUtil::TestExperiment* curexp;
};
		
#endif