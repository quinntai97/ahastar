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
#include "AnnotatedAStar.h"
#include "graph.h"

class AbstractAnnotatedMapAbstraction : public mapAbstraction
{
	public:
		AbstractAnnotatedMapAbstraction(Map *m, AbstractAnnotatedAStar* searchalg); 
		~AbstractAnnotatedMapAbstraction() { delete searchalg; }
		
		virtual void annotateMap()=0; // add annotation info
		virtual void openGLDraw()=0; // display function
		virtual bool pathable(node* from, node* to, int terrain, int agentsize) = 0; // check if path is OK for some size agent w/ some caps.
		
		void setDebugInfo(bool debug) { this->debuginfo = debug; }
		bool getDebugInfo() { return this->debuginfo; }
		AbstractAnnotatedAStar* getSearchAlgorithm() { return searchalg; }
	
	private:
		bool debuginfo;
		AbstractAnnotatedAStar* searchalg;

};

class AnnotatedMapAbstraction : public AbstractAnnotatedMapAbstraction
{
	public:
		AnnotatedMapAbstraction(Map *m, AbstractAnnotatedAStar* searchalg);		
		virtual bool pathable(node* from, node* to, int terrain, int agentsize);
		virtual void openGLDraw(); 
	
		/* all the pure-virtual stuff we need to implement for a concrete mapAbstraction */
		virtual void annotateMap(); 
		bool pathable(node*, node*);
		void verifyHierarchy() {}
		void removeNode(node*) {} 
		void removeEdge(edge*, unsigned int) {}
		void addNode(node*) {}
		void addEdge(edge*, unsigned int) { /* need to  implement to add missing edges?? */ }
		void repairAbstraction() {}
		mapAbstraction *clone(Map *) {}

	
	private:
		void drawClearanceInfo();
		int validterrains[3];
		void addMissingEdges();
		bool drawCV; 

};

#endif