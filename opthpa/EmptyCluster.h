#ifndef EMPTYCLUSTER_H
#define EMPTYCLUSTER_H

#ifdef OS_MAC
	#include "GLUT/glut.h"
	#include <OpenGL/gl.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
#endif

#include "HPACluster.h"

class HPAClusterAbstraction;

class EmptyCluster : public HPACluster
{
	public:
		EmptyCluster(const int x, const int y) throw(std::invalid_argument);
		virtual ~EmptyCluster();

		virtual void 
			addNodesToCluster(HPAClusterAbstraction*) throw(std::invalid_argument);
		virtual void openGLDraw();

	protected:
		virtual void connectParent(node*, HPAClusterAbstraction*);
		virtual void buildHorizontalEntrances(HPAClusterAbstraction* hpamap);
		virtual void buildVerticalEntrances(HPAClusterAbstraction* hpamap);

	private:
		void initOpenGLCoordinates(HPAClusterAbstraction*);
		void extend(HPAClusterAbstraction*);
		void frameCluster(HPAClusterAbstraction*);

		GLdouble glx, gly, glz;  // OpenGL origin coordinates
		GLdouble glHeight, glWidth;
};

#endif

