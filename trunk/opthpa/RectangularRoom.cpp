#include "RectangularRoom.h"

#include "ClusterNode.h"
#include "Entrance.h"
#include "graph.h"
#include "EmptyClusterAbstraction.h"
#include "MacroEdge.h"
#include "MacroNode.h"
#include "map.h"

const int DUMMYDIMENSION = 10; // pass this to keep EmptyCluster constructor happy

RectangularRoom::RectangularRoom(const int x, const int y, 
		EmptyClusterAbstraction* map, bool perimeterReduction_, 
		bool bfReduction_)  throw(std::invalid_argument)
	: EmptyCluster(x, y, map, perimeterReduction_, bfReduction_) 
{
	macro = 0;
	width = height = 0;
	resetBest();
}

RectangularRoom::~RectangularRoom()
{
}

void 
RectangularRoom::buildCluster()
{
	ClusterNode* n = dynamic_cast<ClusterNode*>(
		map->getNodeFromMap(getHOrigin(), getVOrigin()));

	if(!n || n->getParentClusterId() != -1)
	{
		height = 0;
		width = 0;
		std::cout << "WARNING!! aborting cluster creation; "
			"origin is invalid or already assigned!!"<<std::endl;
		return;
	}

	height++;
	width++;
	//std::cout << "cluster: "<<getHOrigin()<<", "<<getVOrigin()<<" ht: "<<height<<" wt: "<<width<<std::endl;
	while(canExtendClearanceSquare())
	{
		height++;
		width++;
	}
	//std::cout << "csq done; height: "
	//	<<height<<" width: "<<width<<std::endl;


	while(canExtendHorizontally())
	{
		width++;
	//	std::cout << " extending horizontally!"<<std::endl;
	}

	while(canExtendVertically())
	{
		height++;
	//	std::cout << " extending vertically!"<<std::endl;
	}

	// assign nodes to the cluster
	for(int x=getHOrigin(); x<getHOrigin()+width; x++)
		for(int y=getVOrigin(); y<getVOrigin()+height; y++)
			addNode(map->getNodeFromMap(x, y));


	initOpenGLCoordinates();
}

bool 
RectangularRoom::canExtendClearanceSquare()
{
	int x = getHOrigin() + width;
	int y = getVOrigin();
	for( ; y <= getVOrigin() + height; y++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(
			map->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}

	x = getHOrigin();
	y = getVOrigin() + height;
	for( ; x <= getHOrigin() + width; x++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(
			map->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}

	return true;
}

bool 
RectangularRoom::canExtendHorizontally()
{
	int x = getHOrigin() + width;
	for(int y = getVOrigin(); y<getVOrigin() + height; y++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(map->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}
	return true;
}

bool 
RectangularRoom::canExtendVertically()
{
	int y = getVOrigin() + height;
	for(int x = getHOrigin(); x<getHOrigin() + width; x++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(map->getNodeFromMap(x, y));
		if(!n || n->getParentClusterId() != -1)
			return false;
	}
	return true;

}

void 
RectangularRoom::initOpenGLCoordinates()
{
	Map* themap = map->getMap();
	GLdouble xx, yy, zz,rr;

	themap->getOpenGLCoord(this->getHOrigin(), this->getVOrigin(), xx, yy, zz, rr);
	glx = xx;
	gly = yy;
	glz = zz-rr*0.5;

	themap->getOpenGLCoord(this->getHOrigin()+this->width-1, 
			this->getVOrigin(), xx, yy, zz, rr);
	glWidth = xx - glx;

	themap->getOpenGLCoord(this->getHOrigin(), this->getVOrigin()+this->height-1, 
			xx, yy, zz, rr);
	glHeight = yy - gly;

	glx-=rr;
	gly-=rr;
	glHeight+=2*rr;
	glWidth+=2*rr;
}

void 
RectangularRoom::openGLDraw()
{
//	glColor3f (0.6F, 0.9F, 0.4F);
	glColor3f (0.0F, 0.6F, 0.0F);
	//glColor3f(0.1, 0.1, 0.7);
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(glx, gly, glz);
	glVertex3f(glx+glWidth, gly, glz);
	glVertex3f(glx+glWidth, gly+glHeight, glz);
	glVertex3f(glx, gly+glHeight, glz);
	glVertex3f(glx, gly, glz);
	glEnd();
	glLineWidth(1.0f);
}


void 
RectangularRoom::setBestExpandedNode(node *n)
{
	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	if(ny == this->getVOrigin() && nx >= this->getHOrigin())
	{
		bestTop = n;
	}
	if(ny == (this->getVOrigin()+this->height-1) && nx >= this->getHOrigin())
	{
		bestBottom = n;
	}
	if(nx == this->getHOrigin() && ny >= this->getVOrigin())
	{
		bestLeft = n;
	}
	if(nx == (this->getHOrigin()+this->width-1) && ny >= this->getVOrigin())
	{
		bestRight = n;
	}
}

node* 
RectangularRoom::getBestExpandedNode(RoomSide side)
{
	switch(side)
	{
		case TOP:
			return bestTop;
		case BOTTOM:
			return bestBottom;
		case LEFT:
			return bestLeft;
		case RIGHT: 
			return bestRight;
		default:
			return 0;
	}
}

void
RectangularRoom::resetBest()
{
	bestLeft = bestRight = bestTop = bestBottom = 0; 
}

// calculate which side of the room's perimeter this node belongs to
RectangularRoomNS::RoomSide 
RectangularRoom::whichSide(node* n_)
{
	MacroNode* n = static_cast<MacroNode*>(n_);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	if(ny == this->getVOrigin() && nx >= this->getHOrigin())
		return TOP;

	if(ny == (this->getVOrigin()+this->getHeight()-1) && 
		nx >= this->getHOrigin())
		return BOTTOM;

	if(nx == this->getHOrigin() && ny >= this->getVOrigin())
		return LEFT;

	if(nx == (this->getHOrigin()+this->getWidth()-1) && 
		ny >= this->getVOrigin())
		return RIGHT;	
	
	return NONE;
}

