/*
 *  AnnotatedNode.h
 *  hog
 *
	Extends the basic node class by adding new attributes for:
	 - terrain type
	 - clearance
	 - parent cluster id
	 
	 TODO: can probably remove clusterid attribute. this is cheaply computed from the (x,y) coordinates of a node if cluster size is known and clusterIds 
	 are assigned in some sequential manner.
 
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDNODE_H
#define ANNOTATEDNODE_H

#include "graph.h"

// add a copy constructor to node/edge; then the child class can copy its attributes and the parent the ones specific to it.

class AnnotatedNode : public node
{
	public:
		AnnotatedNode(const char* name);
		AnnotatedNode(const AnnotatedNode* n);
		
		virtual graph_object* clone() const;
		void setClearance(int terraintype, int value);
		int getClearance(int terrain);
		void setTerrainType(int terrain);
		int getTerrainType();
		void setParentCluster(int clusterid);
		int getParentCluster(); 
		edge* findAnnotatedEdge(node* to, int capability, int clearance, double dist);
	
	private:
		int clearance[3];
		int terraintype;
		int clusterid;
};

#endif