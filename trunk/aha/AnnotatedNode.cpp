/*
 *  AnnotatedNode.cpp
 *  hog
 *
 *  Created by dharabor on 9/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedNode.h"
#include "constants.h"

using namespace std;

AnnotatedNode::AnnotatedNode(const char* name) : node(name)
{
	clusterid = -1; // no parent cluster set
	terraintype=0; // no default terraintype assumed (untraversable node)
	for(int i=0;i<3;i++) 	// init clvals. everything is zero unless otherwise specified.
		clearance[i]=0;
}

// NB: we do not copy the clusterid
AnnotatedNode::AnnotatedNode(const AnnotatedNode* n) : node(n)
{
	clusterid = -1;
	terraintype = n->terraintype;
	for(int i=0; i<3;i++) clearance[i] = n->clearance[i];
}

// deprecated. use copy constructor instead.
graph_object *AnnotatedNode::clone() const
{
	return new AnnotatedNode(this);
}


/* setClearance
	@desc:
		Clearance value annotations represent some maximal amount of free space at a map location for some given terrain type. 
		if a node is traversable, minval = 1, maxval = unbounded.
		else, minvalue = maxvalue = 0. 
			
		The number of annotations for each node is a function of the # of basic terrain types. More precisely, 
		total # of node annotations = (2^n - n), where n is the number of basic terrains.
		
		Supported terrains: kGround, kTrees, kGround|kTrees.
*/
void AnnotatedNode::setClearance(int terraintype, int value)
{
	if(this->terraintype == 0) // only nodes with valid terrain are traversable (and hence need clearance > 0)
	{ 
		if(debuginfo) cout << "AnnotatedNode::setClearance: trying to set clearance > 0 for invalid terrain (t: "<<terraintype<<" @ "<<getLabelL(kFirstData)<<","<<getLabelL(kFirstData+1)<<")"<<endl;
		return;
	}
	if(value < 0)
	{
		if(debuginfo) cout << "AnnotatedNode::setClearance: Clearance value < 0 ("<<terraintype<<")"<<endl;
		return;
	}
	
	switch(terraintype)
	{
		case 0x04:
			clearance[0] = value;
			break;
		case 0x40:
			clearance[1] = value;
			break;
		case 0x44:
			clearance[2] = value;
			break;
		default:
			if(debuginfo) cout << "AnnotatedNode::setClearance: Invalid terrain type ("<<terraintype<<")"<<endl;
			break;
	}

}

int AnnotatedNode::getClearance(int terrain)
{
	int cv = 0;
	switch(terrain)
	{
		case 0x04:
			cv = clearance[0];
			break;
		case 0x40:
			cv = clearance[1];
			break;
		case 0x44:
			cv = clearance[2];
			break;
		default:
			if(debuginfo) cout << "AnnotatedNode::getClearance: Invalid terrain type ("<<terraintype<<")"<<endl;
			break;
	}

	return cv;
}

int AnnotatedNode::getTerrainType()
{
	return terraintype;
}

void AnnotatedNode::setTerrainType(int terrain)
{	
	switch(terrain)
	{
		case 0x04:
			terraintype = terrain;
			break;
		case 0x40:
			terraintype = terrain;
			break;
		case 0x44:
			terraintype = terrain;
			break;
		default:
			if(debuginfo) cout << "AnnotatedNode::setTerraintype: Invalid terrain type ("<<terraintype<<")"<<endl;
			break;
	}
}

int AnnotatedNode::getParentCluster()
{
	return clusterid;
}

void AnnotatedNode::setParentCluster(int clusterid)
{
	if(clusterid >= 0)
		this->clusterid = clusterid;
		
	return;
}

edge* AnnotatedNode::findAnnotatedEdge(node* to, int capability, int clearance, double weight)
{
        if(!to)
                return 0;

		edge* shortestSoFar=0;
		
        edge_iterator ei = this->getEdgeIter();
        edge * e = this->edgeIterNext(ei);
        while(e)
        {
                        unsigned int which;
                        if ((which = e->getFrom()) == this->getNum()) which = e->getTo();

                        /* found an existing edge between these two nodes */
                        if(which == to->getNum())
                        {
                                /* is the edge traversable with the given capability parameter? */
								int eclearance = e->getClearance(capability);
								if(e->getWeight() <= weight)
									if(eclearance >= clearance)
									{
										if(shortestSoFar==0)
											shortestSoFar = e;
										else
										{
											if(e->getWeight() < shortestSoFar->getWeight())
												shortestSoFar = e;
										}
										
									}
//									else
//										std::cout << "\nWARNING! findAnnotatedEdge being queried about a more optimal path between endpoints\n";
						}

                        e = this->edgeIterNext(ei);
        }

        return shortestSoFar;
}