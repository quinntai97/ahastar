#include "EmptyCluster.h"
#include "HPAClusterAbstraction.h"
#include "map.h"


EmptyCluster::EmptyCluster(const int x, const int y) throw(std::invalid_argument)
	: HPACluster(x, y, 0, 0)
{
	if(x < 0 || y <0)
		throw std::invalid_argument("EmptyCluster: x,y coordinates must be >= 0");
}

EmptyCluster::~EmptyCluster()
{

}

void EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap)
	throw(std::invalid_argument)
{
	if(aMap == NULL)
		throw std::invalid_argument("EmptyCluster::addNodesToCluster cluster"
			   	"abstraction parameter is null");
	extend(aMap);

	// add nodes along left border
	int x = this->getHOrigin();
	for(int y=this->getVOrigin(); y<this->getVOrigin()+this->getWidth(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}

	// add nodes along right border
	x = this->getHOrigin()+this->getWidth()-1;
	for(int y=this->getVOrigin(); y<this->getVOrigin()+this->getHeight(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
	
	// add nodes along top border
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
	
	// add nodes along bottom border
	y = this->getVOrigin()+this->getHeight()-1;
	for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
}

void EmptyCluster::extend(HPAClusterAbstraction* aMap)
{
	Map* themap = aMap->getMap();
	int hsize=0;
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<themap->getMapWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(!n)
			break;
		hsize++;
	}
	
	this->setWidth(hsize);
	int vsize=0;
	for( ; y<themap->getMapHeight(); y++)
	{
		bool rowok=true;
		for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
		{
			node* n = aMap->getNodeFromMap(x,y);
			if(!n)
			{
				rowok=false;
			}
		}
		if(!rowok)
			break;
		vsize++;
	}

	initOpenGLCoordinates(aMap);
}

void EmptyCluster::initOpenGLCoordinates(HPAClusterAbstraction* aMap)
{
	Map* map = aMap->getMap();
	GLdouble xx, yy, zz,rr;

	map->getOpenGLCoord(this->getHOrigin(), this->getVOrigin(), xx, yy, zz, rr);
	glx = xx;
	gly = yy;
	glz = zz-rr*0.5;

	map->getOpenGLCoord(this->getHOrigin()+this->getWidth()-1, 
			this->getVOrigin(), xx, yy, zz, rr);
	glWidth = xx - glx;

	map->getOpenGLCoord(this->getHOrigin(), this->getVOrigin()+this->getHeight()-1, 
			xx, yy, zz, rr);
	glHeight = yy - gly;
}

void EmptyCluster::openGLDraw()
{
	glColor3f (0.6F, 0.9F, 0.4F);
	glBegin(GL_LINE_STRIP);
	glVertex3f(glx, gly, glz);
	glVertex3f(glx+glWidth, gly, glz);
	glVertex3f(glx+glWidth, gly+glHeight, glz);
	glVertex3f(glx, gly+glHeight, glz);
	glVertex3f(glx, gly, glz);
	glEnd();
}
