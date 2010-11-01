#ifndef EMPTYCLUSTER_H
#define EMPTYCLUSTER_H

// EmptyCluster.h
//
// An empty cluster is one which contains no obstacles.
// It features a perimeter of abstract nodes connected to each other 
// such that an optimal traversal is always possible from any perimeter node 
// to any other.
//
// NB: This is an abstract class. To make it concrete it is necessary to
// implement the following functions:
//		- growCluster: Starting from an initial seed node, this function grows
//
// For concrete examples of empty clusters, see:
// 	[Harabor and Botea, 2010] 
// 	[Harabor and Botea, 2011]
//
// @author: dharabor
// @created: 29/10/2010

#include "Cluster.h"

namespace EmptyClusterNS
{
	typedef enum 
	{LEFT, RIGHT, TOP, BOTTOM, NONE} 
	RoomSide;

	typedef enum 
	{N, S, E, W, NE, NW, SE, SW} 
	Direction;
}

using EmptyClusterNS;
class EmptyCluster : public Cluster
{
	public:
		EmptyCluster(graphAbstraction*, bool pr=false, bool bfr=false);
		virtual ~EmptyCluster();

		virtual void buildCluster(); // grow cluster + add macro edges
		virtual void buildEntrances(); // process all nodes; identify parents, add transitions
		virtual void connectParent(node*, HPAClusterAbstraction*);

		edge* findSecondaryEdge(unsigned int fromId, unsigned int toId);
		unsigned int getNumSecondaryEdges() { return secondaryEdges.size(); }
		virtual void openGLDraw();


	protected:
		void addMacroEdges(HPAClusterAbstraction *aMap);
		virtual void growCluster() = 0;
		void resetBest() { bestLeft = bestRight = bestTop = bestBottom = 0; }
		void setBestExpandedNode(node* n);
		node* getBestExpandedNode(RoomSide side);
		RoomSide whichSide(node* n);

		int macro;

	private:
		void addSingleMacroEdge(node* from, node* to, double weight, 
				graph* absg, bool secondaryEdge = false);
		void addCardinalMacroEdges(HPAClusterAbstraction *aMap);
		void addDiagonalMacroEdges(HPAClusterAbstraction* hpamap);
		void addDiagonalFanMacroEdges(HPAClusterAbstraction* hpamap);
		bool isIncidentWithInterEdge(node* n, HPAClusterAbstraction* hpamap);

		bool perimeterReduction;
		bool bfReduction;
		std::vector<edge*> secondaryEdges;
		
		// expanded node with lowest g-cost along each side of the perimeter
		node *bestLeft, *bestRight, *bestTop, *bestBottom;
};

#endif

