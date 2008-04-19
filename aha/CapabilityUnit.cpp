/*
 *  CapabilityUnit.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 19/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "CapabilityUnit.h"

void CapabilityUnit::openGLDraw(mapProvider *mp, simulationInfo *si)
{
	searchUnit::openGLDraw(mp, si);
	
	GLdouble xx, yy, zz, rad;
	Map *map = mp->getMap();


	int posx = x, posy = y;
	int size = ((AbstractAnnotatedAStar*)this->getAlgorithm())->getClearance();
	
	if (onTarget)
	{
		double perc = (1.0-sqrt(sqrt(abs(sin(targetTime+0.25*si->getSimulationTime())))));
		glColor3f(r*perc, g*perc, b*perc);
	}
	else	
		glColor3f(r, g, b);	
	
	map->getOpenGLCoord(posx, posy, xx, yy, zz, rad);
	drawSphere(xx, yy, zz, rad*size);

}

void CapabilityUnit::logStats(statCollection *stats)
{
	((AnnotatedAStar*)getAlgorithm())->logStats(stats);

}

void CapabilityUnit::logFinalStats(statCollection *stats)
{
	return; // why does HOG require two log methods??
}
