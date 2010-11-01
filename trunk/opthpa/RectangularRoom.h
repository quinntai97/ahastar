/*
 * RectangularRoom.h
 *
 *  Creates a maximal size cluster which is obstacle free. 
 *  Nodes and edges which appear along the perimeter of such clusters are
 *  added to the abstract graph. 
 *
 * 	NB: Clusters are extended in size using a floodfill approach.
 * 	NB2: Every possible transition point is created when an entrance
 * 	between two adjacent clusters is identified.
 *
 * @author: dharabor
 * @created: 10/02/2010
 */

#ifndef RECTANGULARROOM_H
#define RECTANGULARROOM_H

#ifdef OS_MAC
	#include "GLUT/glut.h"
	#include <OpenGL/gl.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
#endif

#include "HPACluster.h"
#include <vector>

class graph;
class HPAClusterAbstraction;
class Entrance;
class MacroNode;
class edge;


class RectangularRoom : public EmptyCluster
{
	#ifdef UNITTEST
		friend class RectangularRoomTest;
	#endif

	public:
		RectangularRoom(const int x, const int y, bool perimeterReduction=true, bool bfReduction = false)
			throw(std::invalid_argument);
		virtual ~RectangularRoom();

		void growCluster(HPAClusterAbstraction*);
		void growCluster(HPAClusterAbstraction*, int** clearance);

		MacroNode* nextNodeInColumn(int x, int y, HPAClusterAbstraction* hpamap,
				bool leftToRight);
		MacroNode* nextNodeInRow(int x, int y, HPAClusterAbstraction* hpamap,
				bool topToBottom);

	protected:
		virtual void buildHorizontalEntrances(HPAClusterAbstraction* hpamap);
		virtual void buildVerticalEntrances(HPAClusterAbstraction* hpamap);

		virtual void processVerticalEntrance(HPAClusterAbstraction* hpamap,
				int x, int y, int length);
		virtual void processHorizontalEntrance(HPAClusterAbstraction* hpamap,
				int x, int y, int length);

	private:
		void initOpenGLCoordinates(HPAClusterAbstraction*);
		bool canExtendClearanceSquare(HPAClusterAbstraction* hpamap);
		bool canExtendHorizontally(HPAClusterAbstraction* hpamap); 
		bool canExtendVertically(HPAClusterAbstraction* hpamap);

		GLdouble glx, gly, glz;  // OpenGL origin coordinates
		GLdouble glHeight, glWidth;

};

#endif

