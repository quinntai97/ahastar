#ifndef RECTANGULARROOM_H
#define RECTANGULARROOM_H

// RectangularRoom.h
//
//  Creates a maximal size cluster which is obstacle free. 
//  Nodes and edges which appear along the perimeter of such clusters are
//  added to the abstract graph. 
//
// 	NB1: Clusters are grown by first computing a maximal size empty square
// 	beginning at the cluster originl (x, y) and then extended into a maximal 
// 	size rectangle.
//
// 	NB2: Both straight and diagonal transitions are considered when computing
// 	entrances between two adjacent clusters.
//
// @author: dharabor
// @created: 10/02/2010
//


#ifdef OS_MAC
	#include "GLUT/glut.h"
	#include <OpenGL/gl.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
#endif

#include "EmptyCluster.h"
#include <vector>

namespace RectangularRoomNS
{
	typedef enum 
	{LEFT, RIGHT, TOP, BOTTOM, NONE} 
	RoomSide;
}

using namespace RectangularRoomNS; 
class RectangularRoom : public EmptyCluster
{
	#ifdef UNITTEST
		friend class RectangularRoomTest;
	#endif

	public:
		RectangularRoom(const int x, const int y, EmptyClusterAbstraction* map, 
				bool perimeterReduction=true, bool bfReduction=false)
			throw(std::invalid_argument);
		virtual ~RectangularRoom();

		virtual void buildCluster();

		int getHeight() { return height; }
		int getWidth() { return width; }
	
		// methods supporting online node pruning
		void resetBest();
		void setBestExpandedNode(node* n);
		node* getBestExpandedNode(RoomSide side);
		node *bestLeft, *bestRight, *bestTop, *bestBottom;
		RoomSide whichSide(node* n);

		//virtual void openGLDraw();

	protected:

	private:
		void initOpenGLCoordinates();
		bool canExtendClearanceSquare();
		bool canExtendHorizontally(); 
		bool canExtendVertically();

		GLdouble glx, gly, glz;  // OpenGL origin coordinates
		GLdouble glHeight, glWidth;
		int width, height;

};

#endif

