/*
 *  AnnotatedAStar.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedAStar.h"

path* AnnotatedAStar::getPath(graphAbstraction *aMap, node *from, node* to, reservationProvider *resProv)
{
	return getPath(aMap, from, to, (kGround|kTrees), 1); // TODO: fix hardcoded; need a const of supported terrains somewhere
}

path* AnnotatedAStar::getPath(graphAbstraction *aMap, node *from, node* to, int terrain, int agentsize)
{
	return 0;
}