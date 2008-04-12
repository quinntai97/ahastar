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
		virtual path* getPath(graphAbstraction* aMap, node* from, node* to, int capability, int size);
		
	protected:
		virtual path* getAbstractPath(graphAbstraction* aMap, node* from, node* to, int capability, int size) 
		{ 
				return AnnotatedAStar::getPath(aMap, from, to, capability, size); 
		}
		virtual bool evaluate(node* n, node* target);
};

#endif