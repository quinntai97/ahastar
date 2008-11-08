/*
 *  AnnotatedMapAbstractionMock.h
 *  hog
 *
 *  Created by Daniel Harabor on 8/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ANNOTATEDMAPABSTRACTIONMOCK_H
#define ANNOTATEDMAPABSTRACTIONMOCK_H

#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"
#include "ExperimentManager.h"

class AnnotatedMapAbstractionMock : public AbstractAnnotatedMapAbstraction
{
	public:
		AnnotatedMapAbstractionMock(Map* m, AbstractAnnotatedAStar* searchalg) : AbstractAnnotatedMapAbstraction(m, searchalg) { annotateMap(); }
		~AnnotatedMapAbstractionMock() {}
				
		virtual void annotateMap();
		virtual bool pathable(node*, node*, int, int);
		bool pathable(node*, node*);
		void verifyHierarchy() {}
		void removeNode(node*) {} 
		void removeEdge(edge*, unsigned int) {}
		void addNode(node*) {}
		void addEdge(edge*, unsigned int) { /* need to  implement to add missing edges?? */ }
		void repairAbstraction() {}
		mapAbstraction *clone(Map *) {}
		void openGLDraw() { }
		
		void setCurrentTestExperiment(ExpMgrUtil::TestExperiment* exp);
		static void loadClearanceInfo(const std::string&, AnnotatedMapAbstraction*);
		
	private:
		ExpMgrUtil::TestExperiment* curexp;
		
};

#endif