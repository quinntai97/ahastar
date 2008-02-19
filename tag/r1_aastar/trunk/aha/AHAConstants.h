/*
 *  AHAConstants.h
 *  hog
 *
	A collection of useful constants. 
	AHA* is currently limited to supporting a specific subset of terrains and agentsizes.
	These restrictions exist to make this (the initial) implementation simpler. The code can be extended to cope
	with dynamic parameters.
	
 *  Created by Daniel Harabor on 19/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AHACONSTANTS_H
#define AHACONSTANTS_H

#include "map.h"
#include <vector>

/* supported terrain types */
const int NUMTERRAINS = 2;
const int validterrains[NUMTERRAINS] = {kGround, kTrees};

/* supported agent sizes and capabilities */
const int NUMAGENTSIZES = 2;
const int NUMCAPABILITIES = 3;
const int capabilities[NUMCAPABILITIES] = {kGround, kTrees, (kGround|kTrees)};
const int agentsizes[NUMAGENTSIZES] = {1, 2};
const int MINAGENTSIZE = 1;
const int MAXAGENTSIZE = 2;

#endif