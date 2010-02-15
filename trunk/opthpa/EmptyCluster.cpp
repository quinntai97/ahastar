#include "EmptyCluster.h"

#include "ClusterNode.h"
#include "HPAClusterAbstraction.h"
#include "map.h"

const int DUMMYDIMENSION = 10; // pass this to keep HPACluster constructor happy

EmptyCluster::EmptyCluster(const int x, const int y) throw(std::invalid_argument)
	: HPACluster(x, y, DUMMYDIMENSION, DUMMYDIMENSION)
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

	for(int x=getHOrigin(); x<getHOrigin()+getWidth(); x++)
	{
		for(int y=getVOrigin(); y<getVOrigin()+getHeight(); y++)
		{
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x, y));
			if(n)
			{
				addNode(n);
			}
			else
			{
				throw std::invalid_argument("EmptyCluster: tried to add to cluster a node of type other than ClusterNode");
			}
		}
	}

	//frameCluster(aMap);

}

// Every node framing the cluster is added to the abstract graph. Also, 
// every edge between two adjacent framed nodes will appear in the graph.
// NB: there is a special case where the cluster has size 1x1.
// In this case the single node will be added to the graph when 
// buildHorizontalEntrances and buildVerticalEntrances are called
// rather than by this method
void EmptyCluster::frameCluster(HPAClusterAbstraction* aMap)
{
	// add nodes along left border
	node* last = 0;
	int x = this->getHOrigin();
	int y = this->getVOrigin();
	for( ; y<this->getVOrigin()+this->getWidth(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n && last && n->getUniqueID() != last->getUniqueID())
		{
			addTransitionPoint(n, last, aMap);
			last = n;
		}
		else
			last = n;
	}

	// add nodes along bottom border
	y = this->getVOrigin()+this->getHeight()-1;
	for(x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n && last && n->getUniqueID() != last->getUniqueID())
		{
			addTransitionPoint(n, last, aMap);
			last = n;
		}
	}

	// add nodes along right border
	x = this->getHOrigin()+this->getWidth()-1;
	for(y=this->getVOrigin(); y<this->getVOrigin()+this->getHeight(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n && last && n->getUniqueID() != last->getUniqueID())
		{
			addTransitionPoint(n, last, aMap);
			last = n;
		}
	}
	// add nodes along top border
	y = this->getVOrigin();
	for(x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n && last && n->getUniqueID() != last->getUniqueID())
		{
			addTransitionPoint(n, last, aMap);
			last = n;
		}
	}
}

void EmptyCluster::extend(HPAClusterAbstraction* aMap)
{
	Map* themap = aMap->getMap();
	int hsize=0;
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<themap->getMapWidth(); x++)
	{
		ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
		if(!n || n->getParentClusterId() != -1)
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
			ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			if(!n || n->getParentClusterId() != -1)
			{
				rowok=false;
			}
		}
		if(!rowok)
			break;
		vsize++;
	}
	this->setHeight(vsize);

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

void EmptyCluster::connectParent(node*, HPAClusterAbstraction*)
{
}

void EmptyCluster::buildHorizontalEntrances(HPAClusterAbstraction* hpamap)
{
}

void EmptyCluster::buildVerticalEntrances(HPAClusterAbstraction* hpamap)
{
}

