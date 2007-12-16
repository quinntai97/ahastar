/*
 *  AnnotatedMapAbstraction.h
 *  hog	
 *
 
	Assumptions: 
		- Range of valid terrains = {kGround, kTrees, (kGround|kTrees)}
		- Range of valid agentsizes = {1, 2}
	Added these for simplicity of implementation; a more general solution is very possible but would take too long atm.
 
 *  Created by Daniel Harabor on 5/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDMAPABSTRACTION_H
#define ANNOTATEDMAPABSTRACTION_H

#include "mapAbstraction.h"
#include "graph.h"

class AnnotatedMapAbstraction : public mapAbstraction
{
	public:
		AnnotatedMapAbstraction(Map *m);

		void annotateMap();
		void setDebugInfo(bool debug) { debuginfo = debug; }
		bool getDebugInfo() { return debuginfo; }
		
		virtual bool pathable(node*, node*, int, int);
		
		/* why is all this crap pure virtual in graph abstraction?! why doesn't mapAbstraction implement any!?!?! */
		virtual bool pathable(node*, node*);
		virtual void verifyHierarchy() {}
		virtual void removeNode(node*) {} 
		virtual void removeEdge(edge*, unsigned int) {}
		virtual void addNode(node*) {}
		virtual void addEdge(edge*, unsigned int) { /* need to  implement to add missing edges??*/ }
		virtual void repairAbstraction() {}
		virtual mapAbstraction *clone(Map *) {}

		// display function
		virtual void openGLDraw();
	
	private:
		void drawClearanceInfo();
		
		int validterrains[3];
		void addMissingEdges();
		bool debuginfo;
		bool drawCV; 

};

#endif