#include "EmptyCluster.h"

#include "ClusterNode.h"
#include "HPAClusterAbstraction.h"
#include "MacroEdge.h"
#include "map.h"

const int DUMMYDIMENSION = 10; // pass this to keep HPACluster constructor happy

EmptyCluster::EmptyCluster(const int x, const int y) throw(std::invalid_argument)
	: HPACluster(x, y, DUMMYDIMENSION, DUMMYDIMENSION)
{
	if(x < 0 || y <0)
		throw std::invalid_argument("EmptyCluster: x,y coordinates must be >= 0");

	macro = 0;
}

EmptyCluster::~EmptyCluster()
{
}

void EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap, int** clearance)
	throw(std::invalid_argument)
{
	if(aMap == NULL)
		throw std::invalid_argument("EmptyCluster::addNodesToCluster cluster"
			   	"abstraction parameter is null");
	extend(aMap, clearance);

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

	frameCluster(aMap);
	addMacroEdges(aMap);
}

void EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap)
	throw(std::invalid_argument)
{
	if(aMap == NULL)
		throw std::invalid_argument("EmptyCluster::addNodesToCluster cluster"
			   	"abstraction parameter is null");
	extend(aMap);

	if(getVerbose())
	{
		std::cout << "cheight: "<<getHeight()<<" cwidth: ";
		std::cout <<getWidth()<<std::endl;
	}

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

	frameCluster(aMap);
	addMacroEdges(aMap);
}


// Every node framing the cluster is added to the abstract graph. Also, 
// every edge between two adjacent framed nodes will appear in the graph.
// NB: there is a special case where the cluster has size 1x1.
// In this case the single node will be added to the graph when 
// buildHorizontalEntrances and buildVerticalEntrances are called
// rather than by this method
void EmptyCluster::frameCluster(HPAClusterAbstraction* aMap)
{
	if(getVerbose())
		std::cout << "frameCluster"<<std::endl;

	// add nodes along left border
	ClusterNode* last = 0;
	int x = this->getHOrigin();
	int y = this->getVOrigin();
	for( ; y<this->getVOrigin()+this->getHeight(); y++)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
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
		ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
		if(n && last && n->getUniqueID() != last->getUniqueID())
		{
			addTransitionPoint(n, last, aMap);
			last = n;
		}
	}

	// don't try to add edges twice if the cluster has dimension = 1
	if(getWidth() > 1 && getHeight() > 1)
	{
		// add nodes along right border
		x = this->getHOrigin()+this->getWidth()-1;
		for(y=this->getVOrigin()+this->getHeight()-1; y>=this->getVOrigin(); y--)
		{
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			if(n && last && n->getUniqueID() != last->getUniqueID())
			{
				addTransitionPoint(n, last, aMap);
				last = n;
			}
		}
		// add nodes along top border
		y = this->getVOrigin();
		for(x=this->getHOrigin()+this->getWidth()-1; x>=this->getHOrigin(); x--)
		{
			ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			if(n && last && n->getUniqueID() != last->getUniqueID())
			{
				addTransitionPoint(n, last, aMap);
				last = n;
			}
		}
	}

	// frame single-node clusters
	if(getWidth() == 1 && getHeight() == 1)
	{
		ClusterNode* n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(getHOrigin(), getVOrigin()));
		ClusterNode* absn = dynamic_cast<ClusterNode*>(n->clone());
		absn->setLabelL(kAbstractionLevel, 1);
		graph* g = aMap->getAbstractGraph(1);
		g->addNode(absn);
		addParent(absn, aMap); // also creates intra-edges
		n->setLabelL(kParent, absn->getNum());
	}
}

void EmptyCluster::addMacroEdges(HPAClusterAbstraction *aMap)
{
	if(getVerbose())
	{
		std::cout << "adding macro edges for cluster "<<getId()<<" origin ";
		std::cout <<"("<< getHOrigin()<<", "<<getVOrigin()<<")";
		std::cout <<" diagonal edges allowed? "<<getAllowDiagonals()<< " ";
	}
	graph* absg = aMap->getAbstractGraph(1);
	macro = 0;

	if(this->getWidth() > 1)
	{
		int lx = this->getHOrigin();
		int rx = lx + this->getWidth()-1;
		for(int y=this->getVOrigin()+1; y<this->getVOrigin()+this->getHeight()-1; y++)
		{
			node *left = absg->getNode(
					aMap->getNodeFromMap(lx, y)->getLabelL(kParent));
			node *right = absg->getNode(
					aMap->getNodeFromMap(rx, y)->getLabelL(kParent));
			addSingleMacroEdge(left, right, aMap->h(left, right), absg);

			if(getAllowDiagonals())
			{
				if(y>this->getVOrigin())
				{
					right = absg->getNode(
						aMap->getNodeFromMap(rx, y-1)->getLabelL(kParent));
					addSingleMacroEdge(left, right, aMap->h(left, right), absg);
				}
				if(y<this->getVOrigin()+this->getHeight()-1)
				{
					right = absg->getNode(
						aMap->getNodeFromMap(rx, y+1)->getLabelL(kParent));
					addSingleMacroEdge(left, right, aMap->h(left, right), absg);
				}
			}
		}
	}

	if(this->getHeight() > 1)
	{
		int ty = this->getVOrigin();
		int by = this->getVOrigin()+this->getHeight()-1;
		for(int x=this->getHOrigin()+1; x<this->getHOrigin()+this->getWidth()-1; x++)
		{
			node *top = absg->getNode(
					aMap->getNodeFromMap(x, ty)->getLabelL(kParent));
			node *bottom = absg->getNode(
					aMap->getNodeFromMap(x, by)->getLabelL(kParent));
			addSingleMacroEdge(top, bottom, aMap->h(top, bottom), absg);

			if(getAllowDiagonals())
			{
				if(x>this->getHOrigin())
				{
					bottom = absg->getNode(
						aMap->getNodeFromMap(x-1, by)->getLabelL(kParent));
					addSingleMacroEdge(top, bottom, aMap->h(top, bottom), absg);
				}
				if(x<this->getHOrigin()+this->getWidth()-1)
				{
					bottom = absg->getNode(
						aMap->getNodeFromMap(x+1, by)->getLabelL(kParent));
					addSingleMacroEdge(top, bottom, aMap->h(top, bottom), absg);
				}
			}
		}
	}

	if(getVerbose())
		std::cout << macro << " macro edges added for cluster "<<getId()<<std::endl;
}

void EmptyCluster::addSingleMacroEdge(node* from, node* to, double weight, graph* absg)
{
			edge* e = absg->findEdge(from->getNum(), to->getNum());
			if(e == 0)
			{
				e = new MacroEdge(from->getNum(), to->getNum(), weight);
				absg->addEdge(e);
				macro++;
				if(getVerbose())
				{
					std::cout << "[("<<from->getLabelL(kFirstData)<<", ";
					std::cout <<from->getLabelL(kFirstData+1)<<") <-> (";
					std::cout << to->getLabelL(kFirstData) << ", ";
					std::cout << to->getLabelL(kFirstData+1);
					std::cout <<") wt: "<<weight<< " ] ";
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
			if(getVerbose())
			{
				if(n)
					std::cout << "("<<x<<", "<<y<<") is ok!"<<std::endl;
			}
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

void EmptyCluster::extend(HPAClusterAbstraction* aMap, int** clearance)
{
	int cvOrigin = clearance[this->getHOrigin()][this->getVOrigin()];

	Map* themap = aMap->getMap();
	int hsize=0;
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<themap->getMapWidth(); x++)
	{
		int cv = clearance[x][y];
		ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
		if(!n || n->getParentClusterId() != -1 || cv > cvOrigin)
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
			int cv = clearance[x][y];
			ClusterNode *n = dynamic_cast<ClusterNode*>(aMap->getNodeFromMap(x,y));
			if(!n || n->getParentClusterId() != -1 || cv > cvOrigin)
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

	glx-=rr;
	gly-=rr;
	glHeight+=2*rr;
	glWidth+=2*rr;
}

void EmptyCluster::openGLDraw()
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

void EmptyCluster::connectParent(node*, HPAClusterAbstraction*)
{
}

// identifies entrances between horizontally adjacent clusters.
// an abstract node is added for each node in the entrance area
// and an abstract edge is added for each (straight) edge in the entrance area 
// connecting the two clusters.
//
// if 'getAllowDiagonals()' is set we also add an abstract edge for each diagonal
// edge in the entrance area between the two clusters
void EmptyCluster::processHorizontalEntrance(
		HPAClusterAbstraction* hpamap, int x, int y, int length)
{
//	if(getVerbose())
//	{
//		std::cout << "processing hEnts for ";
//		print(std::cout);
//		std::cout << std::endl;
//	}

	for(int xprime=x; xprime <x+length; xprime++)
	{	
		node* endpoint1 = hpamap->getNodeFromMap(xprime, y); 
		node* endpoint2 = hpamap->getNodeFromMap(xprime, y-1);
		this->addTransitionPoint(endpoint1, endpoint2, hpamap);
	}

	if(getAllowDiagonals())
	{
		// first set of diagonals
		for(int xprime=x+1; xprime <x+length; xprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(xprime, y); 
			node* endpoint2 = hpamap->getNodeFromMap(xprime-1, y-1);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<xprime<<","<<y<<") <-> ("
				"" << xprime-1<<", "<<y-1<<")"<<std::endl;
			}
		}

		// second set of diagonals
		for(int xprime=x+1; xprime <x+length; xprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(xprime, y-1); 
			node* endpoint2 = hpamap->getNodeFromMap(xprime-1, y);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<xprime<<","<<y-1<<") <-> ("
				"" << xprime-1<<", "<<y<<")"<<std::endl;
			}
		}
	}
}

void EmptyCluster::buildHorizontalEntrances(HPAClusterAbstraction* hpamap)
{
	int mapheight = hpamap->getMap()->getMapHeight();
	int y = this->getVOrigin()+this->getHeight();
	if(y == mapheight)
		return; 

	// scan for vertical entrances along the eastern border 
	int x = this->getHOrigin();
	while(x < this->getHOrigin()+this->getWidth())
	{
		int length = findHorizontalEntranceLength(x,y, hpamap);
	
		if(length == 0)
			x++;
		else
		{
			// add inter-edges to connect adjacent clusters 
			processHorizontalEntrance(hpamap, x, y, length);
			x += length;
		}
	}
}

// each node in the veritcal entrance area is represented by a node in 
// the abstract graph. 
// further, each (straight) edge in the entrance area, connecting the two
// adjacent clusters, is represented by a node in the abstract graph.
//
// if 'getAllowDiagonals()' is true, every diagonal edge connecting the two
// adjacent clusters is represented by an edge in the abstract graph.
void EmptyCluster::processVerticalEntrance(
		HPAClusterAbstraction* hpamap, int x, int y, int length)
{
//	if(getVerbose())
//	{
//		std::cout << "processing vEnts for ";
//		print(std::cout);
//		std::cout << std::endl;
//	}

	for(int yprime=y; yprime <y+length; yprime++)
	{
		node* endpoint1 = hpamap->getNodeFromMap(x, yprime); 
		node* endpoint2 = hpamap->getNodeFromMap(x-1, yprime);
		this->addTransitionPoint(endpoint1, endpoint2, hpamap);
	}

	if(getAllowDiagonals())
	{
		// first set of diagonals
		for(int yprime=y+1; yprime <y+length; yprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(x, yprime); 
			node* endpoint2 = hpamap->getNodeFromMap(x-1, yprime-1);
			assert(endpoint1 && endpoint2);

			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<x<<","<<yprime<<") <-> ("
				"" << x-1<<","<<yprime-1<<")"<<std::endl;
			}
		}

		// second set of diagonals
		for(int yprime=y+1; yprime <y+length; yprime++)
		{	
			node* endpoint1 = hpamap->getNodeFromMap(x-1, yprime); 
			node* endpoint2 = hpamap->getNodeFromMap(x, yprime-1);
			assert(endpoint1 && endpoint2);
				
			this->addTransitionPoint(endpoint1, endpoint2, hpamap);
			if(getVerbose())
			{
				std::cout << "dtp: ("<<x-1<<","<<yprime<<") <-> ("
				"" << x<<","<<yprime-1<<")"<<std::endl;
			}
		}
	}
}

void EmptyCluster::buildVerticalEntrances(HPAClusterAbstraction* hpamap)
{
	int mapwidth = hpamap->getMap()->getMapWidth();
	int x = this->getHOrigin()+this->getWidth();
	if(x == mapwidth)
		return; 

	// scan for vertical entrances along the eastern border 
	int y = this->getVOrigin();
	while(y < this->getVOrigin()+this->getHeight())
	{
		int length = findVerticalEntranceLength(x,y, hpamap);
	
		// build transition points; long entrances have 2, short entrances have 1.
		if(length == 0)
			y++;
		else
		{
			processVerticalEntrance(hpamap, x, y, length);
			y += length;
		}
	}
}


