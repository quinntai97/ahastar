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
	public:	
		virtual const char* getName() { return "AnnotatedHierarchicalAStar"; }
		
	protected:
		virtual bool evaluate(node* n, node* target);
};

#endif