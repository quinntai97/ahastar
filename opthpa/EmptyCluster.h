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
// implement ::buildCluster, which is responsible for adding a set of nodes 
// to the cluster. 
//
// For concrete examples of empty clusters, see:
// 	[Harabor and Botea, 2010] 
// 	[Harabor and Botea, 2011]
//
// @author: dharabor
// @created: 29/10/2010

#include "AbstractCluster.h"
#include <vector>

namespace EmptyClusterNS
{
	typedef enum 
	{N, S, E, W, NE, NW, SE, SW} 
	Direction;
}

class graph;
class node;
class edge;
class EmptyClusterAbstraction;
class EmptyCluster : public AbstractCluster
{
	public:
		EmptyCluster(int x, int y, EmptyClusterAbstraction*, 
				bool pr=false, bool bfr=false);
		virtual ~EmptyCluster();

		virtual void buildCluster() = 0; 

		virtual void buildEntrances(); 
		virtual void connectParent(node* n) throw(std::invalid_argument);
		virtual void removeParent(node* n);

		void setPerimeterReduction(bool pr) { this->perimeterReduction = pr; }
		void setBFReduction(bool bfr) { this->bfReduction = bfr; }

		edge* findSecondaryEdge(unsigned int fromId, unsigned int toId);
		unsigned int getNumSecondaryEdges() { return secondaryEdges.size(); }
		int macro; // macro edge refcount

		virtual void openGLDraw();


	private:
		void frameCluster();
		void reducePerimeter();

		void addMacroEdges(node* parent);
		void addCardinalMacroEdges(node* n);
		void addAdjacentEdges(node* parent);
		void addDiagonalFanMacroEdgeSet(node* parent, 
				EmptyClusterNS::Direction cd, 
				EmptyClusterNS::Direction dd);
		void addShortcutMacroEdges(node *n);
		void addCardinalMacroEdgeSet(node* n);

		node* findPerimeterNode(node* n, EmptyClusterNS::Direction d);
		bool isPerimeterNode(node* n);

		void addSingleMacroEdge(node* from, node* to, double weight, 
				graph* absg, bool secondaryEdge = false);
		int DiagonalStepsBetween(node* n1, node* n2);

		bool perimeterReduction;
		bool bfReduction;
		std::vector<edge*> secondaryEdges; 

};

#endif

