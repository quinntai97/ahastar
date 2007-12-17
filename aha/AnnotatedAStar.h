/*
 *  AnnotatedAStar.h
 *  hog
 *
	Search algorithm which supports different agent capabilities:
		- terrain traversal (agent differentiates between different types of terrain)
		- size (agents may occupy several tiles at a time)
		
	Intended for graphs produced by AnnotatedMapAbstraction and AnnotatedClusterAbstraction.
	
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDASTAR_H
#define ANNOTATEDASTAR_H

#include "searchAlgorithm.h"
#include "graph.h"
#include "graphAbstraction.h"

class AnnotatedAStar : public searchAlgorithm
{
	public:
		virtual path* getPath(graphAbstraction*, node *, node*, reservationProvider*);
		virtual path* getPath(graphAbstraction*, node*, node*, int, int);
		virtual const char* getName() { return "AnnotatedAStar"; }
};

#endif