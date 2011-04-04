#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "MacroEdge.h"
#include "MacroNode.h"

EmptyCluster::EmptyCluster(const int _x, const int _y, 
		EmptyClusterAbstraction* map, bool pr, bool bfr) 
	: AbstractCluster(map)
{
	if(_x < 0 || _y < 0)
		throw std::invalid_argument("EmptyCluster::EmptyCluster: "
				"cluster (x,y) coordinates must be >= 0");

	startx = _x;
	starty = _y;
	height = 1;
	width = 1;
}

EmptyCluster::~EmptyCluster()
{
}

// Starting from a seed location (startx, starty), build a maximally
// sized rectangular room that is free of obstacles.
void 
EmptyCluster::buildCluster()
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
	height=1;
	width=1;

	while(canExtendClearanceSquare())
	{
		height++;
		width++;
	}

	int squareheight = height;
	int squarewidth = width;
	while(canExtendHorizontally())
	{
		width++;
	}
	int maxwidth = width;

	width = squarewidth;
	while(canExtendVertically())
	{
		height++;
	}
	int maxheight = height;

	if(maxheight*squarewidth > squareheight*maxwidth)
	{
		height = maxheight;
		width = squarewidth;
	}
	else
	{
		height = squareheight;
		width = maxwidth;
	}

	// assign nodes to the cluster
	for(int x=getHOrigin(); x<getHOrigin()+width; x++)
		for(int y=getVOrigin(); y<getVOrigin()+height; y++)
			addNode(map->getNodeFromMap(x, y));

	// identfy perimeter nodes
	frameCluster();
}

bool 
EmptyCluster::canExtendClearanceSquare()
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
EmptyCluster::canExtendHorizontally()
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
EmptyCluster::canExtendVertically()
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

// Every node from the perimeter, which has a neighbour in an adjacent cluster,
// is added to the abstract graph. 
//
// NB: there is a special case where the cluster has size 1x1.
// In this case the single node will be added to the graph when 
// buildHorizontalEntrances and buildVerticalEntrances are called
// rather than by this method
void 
EmptyCluster::frameCluster()
{
        if(getVerbose())
                std::cout << "frameCluster "<<this->getId()<<std::endl;
		graph* absg = map->getAbstractGraph(1);

        // add nodes along left border
        MacroNode* first = 0;
        MacroNode* last = 0;
        int x = this->getHOrigin();
        int y = this->getVOrigin();
        for( ; y<this->getVOrigin()+this->getHeight(); y++)
        {
			MacroNode* n = dynamic_cast<MacroNode*>(map->getNodeFromMap(x,y));
			assert(n);
			if(!perimeterReduction || 
					(isIncidentWithInterEdge(n) && perimeterReduction))
			{
				addParent(n);
				if(last)
				{
					if(n->getUniqueID() != last->getUniqueID() &&
					     ((isIncidentWithInterEdge(last) && 
						   perimeterReduction) || !perimeterReduction))
					{
						addSingleMacroEdge(
								absg->getNode(n->getLabelL(kParent)), 
								absg->getNode(last->getLabelL(kParent)), 
								map->h(n, last), absg);
						last = n;
					}
				}
				else
				{
					first = n;
					last = n;
				}
			}
        }

        // add nodes along bottom border
        y = this->getVOrigin()+this->getHeight()-1;
        for(x=this->getHOrigin()+1; x<this->getHOrigin()+this->getWidth(); x++)
        {
			MacroNode* n = dynamic_cast<MacroNode*>(map->getNodeFromMap(x,y));
			assert(n);
			if(!perimeterReduction || 
					(isIncidentWithInterEdge(n) && perimeterReduction))
			{
				addParent(n);
				if(last)
				{
					if(n->getUniqueID() != last->getUniqueID() &&
					     ((isIncidentWithInterEdge(last) && 
						   perimeterReduction) || !perimeterReduction))
					{
						addSingleMacroEdge(
								absg->getNode(n->getLabelL(kParent)), 
								absg->getNode(last->getLabelL(kParent)), 
								map->h(n, last), absg);
						last = n;
					}
				}
				else
				{
					first = n;
					last = n;
				}
			}
        }

        // don't try to add edges twice if the cluster has dimension = 1
        if(getWidth() > 1 && getHeight() > 1)
        {
			// add nodes along right border
			x = this->getHOrigin()+this->getWidth()-1;
			for(y=this->getVOrigin()+this->getHeight()-2; 
					y>=this->getVOrigin(); y--)
			{
				MacroNode* n = dynamic_cast<MacroNode*>(map->getNodeFromMap(x,y));
				assert(n);
				if(!perimeterReduction || 
						(isIncidentWithInterEdge(n) && perimeterReduction))
				{
					addParent(n);
					if(last)
					{
						if(n->getUniqueID() != last->getUniqueID() &&
							 ((isIncidentWithInterEdge(last) && 
							   perimeterReduction) || !perimeterReduction))
						{
							addSingleMacroEdge(
								absg->getNode(n->getLabelL(kParent)), 
								absg->getNode(last->getLabelL(kParent)), 
								map->h(n, last), absg);
							last = n;
						}
					}
					else
					{
						first = n;
						last = n;
					}
				}
			}

			// add nodes along top border
			y = this->getVOrigin();
			for(x=this->getHOrigin()+this->getWidth()-2; 
					x>=this->getHOrigin()+1; x--)
			{
				MacroNode* n = dynamic_cast<MacroNode*>(map->getNodeFromMap(x,y));
				assert(n);
				if(!perimeterReduction || 
						(isIncidentWithInterEdge(n) && perimeterReduction))
				{
					addParent(n);
					if(last)
					{
						if(n->getUniqueID() != last->getUniqueID() &&
							 ((isIncidentWithInterEdge(last) && 
							   perimeterReduction) || !perimeterReduction))
						{
							addSingleMacroEdge(
								absg->getNode(n->getLabelL(kParent)), 
								absg->getNode(last->getLabelL(kParent)), 
								map->h(n, last), absg);
							last = n;
						}
					}
					else
					{
						first = n;
						last = n;
					}
				}
			}

			// connect the first and last nodes to be considered
			if(first && last && first->getUniqueID() != last->getUniqueID())
			{
				addSingleMacroEdge(
						absg->getNode(first->getLabelL(kParent)), 
						absg->getNode(last->getLabelL(kParent)), 
						map->h(first, last), absg);
			}
        }
}

void 
EmptyCluster::buildEntrances()
{
}

void 
EmptyCluster::connectParent(node* n) 
	throw(std::invalid_argument)
{
}

// returns true if node n_ has a neighbour with a different parent cluster. 
bool 
EmptyCluster::isIncidentWithInterEdge(node* n_)
{
	bool retVal = false;
	MacroNode* n = static_cast<MacroNode*>(n_);
	assert(n);

	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);
	for(int nbx = nx-1; nbx < nx+2; nbx++)
		for(int nby = ny-1; nby < ny+2; nby++)
		{
			MacroNode* nb = static_cast<MacroNode*>(
					map->getNodeFromMap(nbx, nby));
			if(!map->getAllowDiagonals() && nbx != nx && nby != ny)
				continue;
			else if(nb && 
					nb->getParentClusterId() != n->getParentClusterId())
			{
				retVal = true;
			}
		}
	
	return retVal;
}

void 
EmptyCluster::addSingleMacroEdge(node* from_, node* to_, double weight, 
		graph* absg, bool secondaryEdge)
{
	assert(from_ && to_);
	assert(from_->getUniqueID() != to_->getUniqueID());

	MacroNode* from = static_cast<MacroNode*>(from_);
	MacroNode* to = static_cast<MacroNode*>(to_);


	assert(from && to);
	assert(from->getParentClusterId() == to->getParentClusterId());

	MacroEdge* e = static_cast<MacroEdge*>(
			absg->findEdge(from->getNum(), to->getNum()));
	if(e == 0 && from->getParentClusterId() == to->getParentClusterId())
	{
		e = static_cast<MacroEdge*>(findSecondaryEdge(from->getNum(), to->getNum()));
		if(e == 0 && secondaryEdge && bfReduction)
		{
			e = new MacroEdge(from->getNum(), to->getNum(), weight);
			from->addSecondaryEdge(e);
			to->addSecondaryEdge(e);
			secondaryEdges.push_back(e);
		}
		else
		{
			e = new MacroEdge(from->getNum(), to->getNum(), weight);
			absg->addEdge(e);
		}

		macro++;
		if(getVerbose())
		{
			std::cout << "added";
			if(secondaryEdge)
				std::cout << " secondary ";
			std::cout << " macro edge: [("<<from->getLabelL(kFirstData)<<", ";
			std::cout <<from->getLabelL(kFirstData+1)<<") <-> (";
			std::cout << to->getLabelL(kFirstData) << ", ";
			std::cout << to->getLabelL(kFirstData+1);
			std::cout <<") wt: "<<weight<< " ] "<<std::endl;
		}
	}
	else
	{
		e->setSecondary(secondaryEdge);
	}
}

edge* 
EmptyCluster::findSecondaryEdge(unsigned int fromId, unsigned int toId)
{
	edge* retVal = 0;
	for(unsigned int i=0; i<secondaryEdges.size(); i++)
	{
		edge* e = secondaryEdges.at(i);
		if((e->getFrom() == fromId && e->getTo() == toId) ||
			(e->getTo() == fromId && e->getFrom() == toId))
		{
			e = retVal;
			break;
		}
	}
	return retVal;
}

void 
EmptyCluster::openGLDraw()
{
	Map* themap = map->getMap();
	GLdouble glx, gly, glz;  
	GLdouble glHeight, glWidth;
	GLdouble xx, yy, zz,rr;

	// calc position of cluster origin in OpenGL coordinate space
	themap->getOpenGLCoord(getHOrigin(), getVOrigin(), xx, yy, zz, rr);
	glx = xx;
	gly = yy;
	glz = zz-rr*0.5;

	// calculate cluster height and width in OpenGL coordinate space
	themap->getOpenGLCoord(getHOrigin()+width-1, getVOrigin(), xx, yy, zz, rr);
	glWidth = xx - glx;
	themap->getOpenGLCoord(getHOrigin(), getVOrigin()+height-1, xx, yy, zz, rr);
	glHeight = yy - gly;

	glx-=rr;
	gly-=rr;
	glHeight+=2*rr;
	glWidth+=2*rr;
	glColor3f(0.2, 0.6, 0.2);
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
