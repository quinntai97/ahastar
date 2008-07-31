/*
 *  CapabilityUnit.h
 *  hog
 *
 *  Created by Daniel Harabor on 19/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CAPABILITYUNIT_H
#define CAPABILITYUNIT_H

#include "searchUnit.h"
#include "AnnotatedAStar.h"

class CapabilityUnit : public searchUnit 
{
	public:
		CapabilityUnit(int x, int y, unit *target, AbstractAnnotatedAStar *alg) : searchUnit(x,y,target,alg) { }
		CapabilityUnit(int _x, int _y, int _r, int _g, int _b, unit *_target, AbstractAnnotatedAStar *alg) : searchUnit(_x,_y,_r,_g,_b,_target,alg) { }
		CapabilityUnit(int _x, int _y, float _r, float _g, float _b, unit *_target, AbstractAnnotatedAStar *alg) : searchUnit(_x,_y,_r,_g,_b,_target,alg) { }
		virtual ~CapabilityUnit() { }

		virtual void openGLDraw(mapProvider *, simulationInfo *);
};

#endif

