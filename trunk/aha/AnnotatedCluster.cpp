/*
 *  AnnotatedCluster.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 6/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstraction.h"
#include "AnnotatedCluster.h"
#include "AHAConstants.h"
#include <sstream>

const char* AnnotatedClusterAbstractionIsNullException::what() const throw()
{
	return std::string("Null map abstraction parameter found").c_str();
}

const char* InvalidClusterDimensionsException::what() const throw()
{
	std::ostringstream oss;
	oss << "Tried to create a cluster with invalid dimension parameters. Details: \n";
	oss << "width: "<<width<<" height: "<<height<<". Cluster origin @ ("<<xorigin<<","<<yorigin<<")"<<std::endl;
	return oss.str().c_str();
}

const char* InvalidClusterOriginCoordinatesException::what() const throw()
{
	std::ostringstream oss;
	oss << "Tried to create a cluster with invalid origin coordinate parameters. Details: \n";
	oss << "Cluster origin @ ("<<xorigin<<","<<yorigin<<")"<<std::endl;
	return oss.str().c_str();
}


const char* AnnotatedClusterException::what() const throw()
{	
		std::ostringstream oss;
		oss << getExceptionErrorMessage();
		
		if(problemNode != NULL)
		{	
			oss << "Details: \n";
			oss << "node @ ("<<problemNode->getLabelL(kFirstData)<<","<<problemNode->getLabelL(kFirstData+1)<<") ";
			oss << "terrain: "<<problemNode->getTerrainType()<< " base clearance: "<<problemNode->getClearance(problemNode->getTerrainType());	
		}
		if(ac != NULL)
			oss << "\n cluster origin @ ("<<ac->getHOrig() <<", "<<ac->getVOrig()<<") "<< "height: "<<ac->getHeight()<<" width: "<<ac->getWidth()<<std::endl;
			
		return oss.str().c_str();
}

const char* NodeIsHardObstacleException::getExceptionErrorMessage() const
{
		return std::string("Tried to assign a node which is hard obstacle to a cluster. ").c_str();
}

const char* NodeIsAlreadyAssignedToClusterException::getExceptionErrorMessage() const
{
	const char* msg = "Tried to assign a node which is already assigned to another cluster.";
	return msg;
}

const char* ClusterFullException::getExceptionErrorMessage() const
{
		return std::string("Tried to add a node to an already full cluster").c_str();
}

const char* NodeIsNullException::getExceptionErrorMessage() const
{
		return std::string("Found a null node parameter.").c_str();
}

CannotBuildEntranceToSelfException::CannotBuildEntranceToSelfException(node* n1, node* n2) : EntranceException(n1, n2)
{
	std::ostringstream oss;
	oss << 	"tried to build an entrance using two nodes from the same cluster. Endpoint1 @ ";
	oss << "("<<endpoint1->getLabelL(kFirstData) << ", "<< endpoint1->getLabelL(kFirstData+1)<< " ) ";
	oss << " Endpoint2 @ "<< "("<<endpoint2->getLabelL(kFirstData) << ", "<< endpoint2->getLabelL(kFirstData+1)<< " ) ";
	oss << std::endl;
	
	message = new std::string(oss.str().c_str());
}

const char* CannotBuildEntranceToSelfException::what() const throw() { return message->c_str(); }


unsigned AnnotatedCluster::uniqueClusterIdCnt = 0;

AnnotatedCluster::AnnotatedCluster(int startx, int starty, int width, int height) throw(InvalidClusterDimensionsException, InvalidClusterOriginCoordinatesException)
	:  Cluster(uniqueClusterIdCnt++,0,0,startx,starty,width,height)
{

	if(width <= 0 || height <=0)
		throw InvalidClusterDimensionsException(width, height, startx, starty);
		
	if(startx < 0 || starty < 0)
		throw InvalidClusterOriginCoordinatesException(startx, starty);
}

/* annotated clusters cannot contain hard obstacles. 
   NB: deprecates addNode(int) in Cluster. Avoid using the version in the base class when adding annotated nodes as it isn't safe.
*/
bool AnnotatedCluster::addNode(node* mynode) throw(NodeIsAlreadyAssignedToClusterException, ClusterFullException, NodeIsNullException)
{
//	if(mynode->getClearance(mynode->getTerrainType()) <= 0)
//		throw NodeIsHardObstacleException(mynode, this);
	
	if(mynode == NULL)
		throw NodeIsNullException(mynode, this);
	
	if(mynode->getParentCluster() != -1)
		throw NodeIsAlreadyAssignedToClusterException(mynode, this);
	
	if(this->getNumNodes() == this->getHeight()*this->getWidth())
		throw ClusterFullException(mynode, this);
	
	mynode->setParentCluster(this->getClusterId());
	Cluster::addNode(mynode->getNum());
	return true;
}

void AnnotatedCluster::addParent(node* parentnode, AnnotatedClusterAbstraction* aca)
{
	if(parentnode->getParentCluster() != -1)
	{
		if(parentnode->getParentCluster() != this->getClusterId())
			throw NodeIsAlreadyAssignedToClusterException(parentnode, this);
		else
		{
			std::cout << "\nWARNING: skipping parent node @ ("<<parentnode->getLabelL(kFirstData)<<","<<parentnode->getLabelL(kFirstData+1)<<"); already belongs to cluster "<<getClusterId()<<std::endl;
			return;
		}
	}
	else
		parentnode->setParentCluster(this->getClusterId());
		
	this->connectEntranceEndpoints(parentnode,aca);	
	Cluster::addParent(parentnode);
}

/* add all traversable nodes in the cluster area to the cluster */
void AnnotatedCluster::addNodesToCluster(AnnotatedClusterAbstraction* aMap)
{

	if(aMap == NULL)
		throw AnnotatedClusterAbstractionIsNullException();
		
	for(int x=this->getHOrig(); x<getHOrig()+getWidth(); x++)
		for(int y=this->getVOrig(); y<getVOrig()+getHeight(); y++)
		{
			addNode(aMap->getNodeFromMap(x,y));
		}
}

void AnnotatedCluster::addEntrance(node* from, node* to, int capability, int clearance, AnnotatedClusterAbstraction* aca) 
	throw(InvalidClearanceParameterException, EntranceNodeIsNotTraversable)
{					
	if(clearance <= 0)
		throw InvalidClearanceParameterException();
	if(clearance > MAXAGENTSIZE) // TODO: low quality only?
		clearance = MAXAGENTSIZE;
		
	if(from->getClearance(capability) < clearance || to->getClearance(capability) < clearance)
		throw EntranceNodeIsNotTraversable();

	addEndpointsToAbstractGraph(from, to, aca);

	double weight = 1.0;
	graph* absg = aca->getAbstractGraph(1);
	node* absfrom = absg->getNode(from->getLabelL(kParent));
	node* absto = absg->getNode(to->getLabelL(kParent));
	addTransitionToAbstractGraph(absfrom, absto, capability, clearance, weight, aca);
}

void AnnotatedCluster::buildVerticalEntrances(int curCapability, AnnotatedClusterAbstraction* aca)
{
	if(aca == NULL)
		throw AnnotatedClusterAbstractionIsNullException();
	
	int mapwidth = aca->getMap()->getMapWidth();

	/* scan for vertical entrances along the eastern border */
	int x = this->getHOrig()+this->getWidth();
	if(x == mapwidth)
		return; // no eastern neighbour; nothing to build
	int y = this->getVOrig();
	while(y < this->getVOrig()+this->getHeight())
	{
		int minY, maxY;
		minY = expandVerticalEntrance(x, y, curCapability, aca);
		maxY = findMaximalClearanceForVerticalEntrance(x, y, minY, curCapability, aca); // maximum clearance between current pos and some local minima nearby
	
		node* endpoint1 = aca->getNodeFromMap(x, maxY); // inside eastern neighbour
		node* endpoint2 = aca->getNodeFromMap(x-1, maxY);
		int clearance = endpoint1->getClearance(curCapability)>endpoint2->getClearance(curCapability)?
				endpoint2->getClearance(curCapability):endpoint1->getClearance(curCapability);

		if(clearance > 0)
			this->addEntrance(endpoint2, endpoint1, curCapability, clearance, aca); // each transition we identify is a local maxima clearance for curCapability
		y = minY+1;
	}
}

int AnnotatedCluster::expandVerticalEntrance(int x, int startY, int curCapability, AnnotatedClusterAbstraction* aca)
{
	int minClearance = MAXINT;
	int minY=startY;
	int lastdepthLeft=0; // initialised after first depth is found
	int lastdepthRight=0; 
	for(int y=startY; y<getVOrig()+getHeight();y++)
	{
		int dLeft = findVerticalEntraceDepthInLeftClusterAtLocation(x-1, y, curCapability, aca);
		int dRight = findVerticalEntraceDepthInRightClusterAtLocation(x, y, curCapability, aca);

		/* if either cluster has depth zero there is an obstacle along the border between the clusters */
		if(dLeft == 0 || dRight == 0)
			return minY;
			
		/* changing depth indicates map Leftography is changing. sLeft expanding the cluster and return minY */
		if(lastdepthLeft == 0)
			lastdepthLeft = dLeft;
		else
			if(dLeft != lastdepthLeft)
				return minY;
		
		if(lastdepthRight == 0)
			lastdepthRight = dRight;
		else
			if(dRight != lastdepthRight)
				return minY;
		
		lastdepthLeft = dLeft; 
		lastdepthRight = dRight;
		
		minY = y;
		
	}
	
	return minY;
}

int AnnotatedCluster::findVerticalEntraceDepthInLeftClusterAtLocation(int x, int y, int capability, AnnotatedClusterAbstraction* aca)
{

	int d=0;
	for(int i=x; i>x-MAXAGENTSIZE;i--)
	{
		node* n = aca->getNodeFromMap(i, y);
		if(n->getClearance(capability) > 0)
			d++;
		else
			break;
	}

	return d;
}


int AnnotatedCluster::findVerticalEntraceDepthInRightClusterAtLocation(int x, int y, int capability, AnnotatedClusterAbstraction* aca)
{
	int d=0;
	int maxdepth = aca->getMap()->getMapWidth();
	for(int i=x; i<x+MAXAGENTSIZE;i++)
	{
		if(i == maxdepth)
			break;
			
		node* n = aca->getNodeFromMap(i, y);
		if(n->getClearance(capability) > 0)
			d++;
		else
			break;
	}
	return d;
}

int AnnotatedCluster::findMaximalClearanceForVerticalEntrance(int x, int startY, int endY, int curCapability, AnnotatedClusterAbstraction* aca)
{
	int maxClearance = 0;
	int maxY = startY;

	for(int y=startY; y<=endY; y++)
	{
		node *c1 = aca->getNodeFromMap(x,y); // node in neighbouring cluster
		node *c2 = aca->getNodeFromMap(x-1, y); // border node in 'this' cluster
		int clearance = c1->getClearance(curCapability)>c2->getClearance(curCapability)?
							c2->getClearance(curCapability):c1->getClearance(curCapability);
		
		if(clearance > maxClearance)
		{
			maxClearance = clearance;
			maxY = y;
		}
	}
	
	return maxY;
}

void AnnotatedCluster::buildHorizontalEntrances(int curCapability, AnnotatedClusterAbstraction* aca)
{

	if(aca == NULL)
		throw AnnotatedClusterAbstractionIsNullException();
	
	int mapheight = aca->getMap()->getMapHeight();
		
	/* scan for horizontal entrances along the southern border */
	int	y = this->getVOrig()+this->getHeight();
	
	if(y == mapheight)
		return; // no southern neighbour; nothing to build

	int x = this->getHOrig();
	while(x < this->getHOrig()+this->getWidth())
	{
		int minX, maxX;
		minX = expandHorizontalEntrance(y, x, curCapability, aca);
		maxX = findMaximalClearanceForHorizontalEntrance(y, x, minX, curCapability, aca); // maximum clearance between current pos and some local minima nearby

//		std::cout << "x: "<<x<<" y: "<<y;
		node* endpoint1 = aca->getNodeFromMap(maxX, y); // inside eastern neighbour
		node* endpoint2 = aca->getNodeFromMap(maxX, y-1);
		int clearance = endpoint1->getClearance(curCapability)>endpoint2->getClearance(curCapability)?
				endpoint2->getClearance(curCapability):endpoint1->getClearance(curCapability);

		if(clearance > 0)
			this->addEntrance(endpoint2, endpoint1, curCapability, clearance, aca); // each transition we identify is a local maxima clearance for curCapability
		x = minX+1;
	}


}

/* find the end of a horizontal entrance beginning at [startX, y] */
int AnnotatedCluster::expandHorizontalEntrance(int y, int startX, int curCapability, AnnotatedClusterAbstraction* aca)
{
	int minClearance = MAXINT;
	int lastdepthTop = 0;
	int lastdepthBottom = 0;
	int minX=startX;
	for(int x=startX; x<getHOrig()+getWidth();x++)
	{			
		int dTop = findHorizontalEntraceDepthInTopClusterAtLocation(x, y-1, curCapability, aca);
		int dBottom = findHorizontalEntraceDepthInBottomClusterAtLocation(x, y, curCapability, aca);

		/* if either cluster has depth zero there is an obstacle along the border between the clusters */
		if(dTop == 0 || dBottom == 0)
			return minX;
			
		/* changing depth indicates map topography is changing. stop expanding the cluster and return minX */
		if(lastdepthTop == 0)
			lastdepthTop = dTop;
		else
			if(dTop != lastdepthTop)
				return minX;
		
		if(lastdepthBottom == 0)
			lastdepthBottom = dBottom;
		else
			if(dBottom != lastdepthBottom)
				return minX;
		
		lastdepthTop = dTop; 
		lastdepthBottom = dBottom;
		
		minX = x;
	}
	
	return minX;
}

int AnnotatedCluster::findHorizontalEntraceDepthInTopClusterAtLocation(int x, int y, int capability, AnnotatedClusterAbstraction* aca)
{

	int d=0; // cluster depth
	for(int i=y; i>y-MAXAGENTSIZE;i--)
	{			
		node* n = aca->getNodeFromMap(x, i);
		if(n->getClearance(capability) > 0)
			d++;
		else
			break;
	}
	
	return d;
}

int AnnotatedCluster::findHorizontalEntraceDepthInBottomClusterAtLocation(int x, int y, int capability, AnnotatedClusterAbstraction* aca)
{

	int d=0; // cluster depth
	int maxdepth = aca->getMap()->getMapHeight();
	for(int i=y; i<y+MAXAGENTSIZE;i++)
	{
		if(i == maxdepth)
			break;
			
		node* n = aca->getNodeFromMap(x, i);
		if(n->getClearance(capability) > 0)
			d++;
		else
			break;
	}
	
	return d;
}



int AnnotatedCluster::findMaximalClearanceForHorizontalEntrance(int y, int startX, int endX, int curCapability, AnnotatedClusterAbstraction* aca)
{
	int maxClearance = 0;
	int maxX = startX;

	for(int x=startX; x<=endX; x++)
	{
		node *c1 = aca->getNodeFromMap(x,y); // node in neighbouring cluster
		node *c2 = aca->getNodeFromMap(x, y-1); // border node in 'this' cluster
		int clearance = c1->getClearance(curCapability)>c2->getClearance(curCapability)?
							c2->getClearance(curCapability):c1->getClearance(curCapability);
		
		if(clearance > maxClearance)
		{
			maxClearance = clearance;
			maxX = x;
		}
	}
	
	return maxX;
}

void AnnotatedCluster::buildEntrances(AnnotatedClusterAbstraction* aca) throw(AnnotatedClusterAbstractionIsNullException)
{
	if(aca == NULL)
		throw AnnotatedClusterAbstractionIsNullException();

	for(int i=0; i<NUMCAPABILITIES; i++)
	{
		buildVerticalEntrances(capabilities[i], aca);
		buildHorizontalEntrances(capabilities[i], aca);
	}
}

void AnnotatedCluster::validateMapAbstraction(AnnotatedClusterAbstraction* aca) throw(ValidateMapAbstractionException)
{
	if(aca == NULL)
		throw ValidateMapAbstractionException();
}

void AnnotatedCluster::validateTransitionEndpoints(node* from, node* to) throw(ValidateTransitionEndpointsException)
{
	if(from == NULL || to == NULL)
		throw ValidateTransitionEndpointsException("transition endpoint cannot be null");
	
	if(from->getLabelL(kFirstData) == to->getLabelL(kFirstData) && from->getLabelL(kFirstData+1) == to->getLabelL(kFirstData+1))
		throw ValidateTransitionEndpointsException("transition endpoints cannot have identical (x,y) coordinates");
}

/* adds an inter-cluster transition to the abstract graph 
	TODO: rename this to something more appropriate (less about endpoints, more about transitions)
*/
void AnnotatedCluster::addEndpointsToAbstractGraph(node* from, node* to, AnnotatedClusterAbstraction* aca) 
	throw(EntranceNodesAreNotAdjacentException, CannotBuildEntranceToSelfException, CannotBuildEntranceFromAbstractNodeException)
{
	if(AnnotatedAStar::getDirection(from, to) == kStay)
		throw EntranceNodesAreNotAdjacentException();
	if(from->getParentCluster() == to->getParentCluster())
		throw CannotBuildEntranceToSelfException(from, to);
	if(from->getLabelL(kAbstractionLevel) != 0 || to->getLabelL(kAbstractionLevel) != 0)
		throw CannotBuildEntranceFromAbstractNodeException();

	graph* g = aca->getAbstractGraph(1);
	node *absfrom, *absto;
	
	/* some entrances may share endpoints so we minimise graph size by reusing nodes */
	if(from->getLabelL(kParent) == -1)
	{
		absfrom = dynamic_cast<node*>(from->clone());
		absfrom->setLabelL(kAbstractionLevel, 1);
		g->addNode(absfrom);
		AnnotatedCluster* fromCluster = aca->getCluster(from->getParentCluster());		
		fromCluster->addParent(absfrom, aca);

		from->setLabelL(kParent, absfrom->getNum());
	}
	else
		absfrom = g->getNode(from->getLabelL(kParent));
	
	if(to->getLabelL(kParent) == -1)
	{
		absto = dynamic_cast<node*>(to->clone());
		absto->setLabelL(kAbstractionLevel, 1);
		g->addNode(absto);
		AnnotatedCluster* toCluster = aca->getCluster(to->getParentCluster());
		toCluster->addParent(absto, aca);

		to->setLabelL(kParent, absto->getNum());
	}
	else
		absto = g->getNode(to->getLabelL(kParent));
}

void AnnotatedCluster::addTransitionToAbstractGraph(node* from, node* to, int capability, int clearance, double weight, AnnotatedClusterAbstraction* aca) 
	throw(InvalidTransitionWeightException)
{
	if(weight <=0 )
		throw InvalidTransitionWeightException();

	graph* g = aca->getAbstractGraph(1);

	/* annotate the edge representing the entrance with appropriate capabilities and clearance values */
	edge* interedge = from->findAnnotatedEdge(to, capability, clearance, weight); // can we re-use any edge that fits caps/clearance criteria?
	if(interedge == 0)
	{
		//std::cout << "\nadding new edge to abs graph: "<<from->getLabelL(kFirstData)<<","<<from->getLabelL(kFirstData+1)<<" and " << to->getLabelL(kFirstData)<<","<<to->getLabelL(kFirstData+1) << " caps: "<<capability<<" clearance: "<<clearance;
		interedge = new edge(from->getNum(), to->getNum(), weight);
		interedge->setClearance(capability,clearance);
		g->addEdge(interedge);
		path* p2 = new path(aca->getNodeFromMap(to->getLabelL(kFirstData), to->getLabelL(kFirstData+1)), NULL);
		path *p = new path(aca->getNodeFromMap(from->getLabelL(kFirstData), from->getLabelL(kFirstData+1)), p2);
		aca->addPathToCache(interedge,p);
	}
}

void AnnotatedCluster::connectEntranceEndpoints(node* newendpoint, AnnotatedClusterAbstraction* aca)
{
	for(int i=0; i<getParents().size(); i++)
	{
		/* simplest capabilities (those involving fewest terrains) first and others last. important to avoid creating identical edges 
		NB: assumes capabilities array is sorted accordingly  */
		node* existingendpoint = getParents()[i];		
		for(int capindex=0; capindex < NUMCAPABILITIES ; capindex++) 
		{
			int capability = capabilities[capindex];
			
			/* find paths for largest size agents first and smallest ones last. NB: assumes agentsizes array is sorted accordingly */
			for(int sizeindex = NUMAGENTSIZES-1; sizeindex>=0; sizeindex--) 
			{
				int size = agentsizes[sizeindex]; 
				connectEntranceEndpointsForAGivenCapabilityAndSize(newendpoint, existingendpoint, capability, size, aca);
			}
		}
	}
}

void AnnotatedCluster::connectEntranceEndpointsForAGivenCapabilityAndSize(node* newendpoint, node* existingendpoint, int capability, int size, AnnotatedClusterAbstraction* aca)
{
	double maxdist = getWidth()*getHeight(); // use maximum possible distance between these two endpoints as an upperbound param when searching for existing edges that may exist between these two endpoints
	edge* e=0;
	
	if(aca->getQualityParam() == ACAUtil::kLowQualityAbstraction)
	{
		/* check if an existing intra-edge dominates the proposed transition */
		e = newendpoint->findAnnotatedEdge(existingendpoint,capability,size,maxdist); 
		if(e == 0)
			findShortestPathBetweenTwoEndpoints(newendpoint, existingendpoint, capability, size, aca);
	}
	else
		findShortestPathBetweenTwoEndpoints(newendpoint, existingendpoint, capability, size, aca); // don't use intra-edge dominance-checking
}

void AnnotatedCluster::findShortestPathBetweenTwoEndpoints(node* n1, node* n2, int capability, int size, AnnotatedClusterAbstraction* aca)
{
		graph* absg = aca->getAbstractGraph(1);
		AbstractAnnotatedAStar* aastar = aca->getSearchAlgorithm();
		aastar->limitSearchToClusterCorridor(true);

		node* from = aca->getNodeFromMap(n1->getLabelL(kFirstData),n1->getLabelL(kFirstData+1)); // get low-level nodes
		node* to = aca->getNodeFromMap(n2->getLabelL(kFirstData),n2->getLabelL(kFirstData+1)); 

		aastar->setCapability(capability);
		aastar->setClearance(size);
		path* solution = aastar->getPath(aca, from, to);
		if(solution != 0)
		{
			double dist = aca->distance(solution);
			edge* e = 0;
			if(aca->getQualityParam() == ACAUtil::kHighQualityAbstraction) // don't add paths twice (optimal paths between two nodes may be identical for two capabilities/sizes)
			{
				e = n1->findAnnotatedEdge(n2,capability,size,dist); 			
				if(e != 0)
					return; 
			}

			//std::cout << "connecting endpoints in cluster "<<getClusterId()<<". new edge, dist: "<<dist<<std::endl;
			e = new edge(n1->getNum(), n2->getNum(), dist);
			e->setClearance(capability,size);
			absg->addEdge(e);
			aca->addPathToCache(e, solution);				
			//std::cout << "\n adding way cool edege for cluster "<<getClusterId();
		}
		
		/* record some metrics about the operation */
		aca->setNodesExpanded(aca->getNodesExpanded() + aastar->getNodesExpanded());
		aca->setNodesTouched(aca->getNodesTouched() + aastar->getNodesTouched());
		aca->setPeakMemory(aastar->getPeakMemory()>aca->getPeakMemory()?aastar->getPeakMemory():aca->getPeakMemory());
		aca->setSearchTime(aca->getSearchTime() + aastar->getSearchTime());
		
		aastar->limitSearchToClusterCorridor(false);
}
// BROKEN BROKEN BROKEN -- DO NOT USE -- 
//Find out the smallest clearance value along some path
//	TODO: can we somehow rationalise about capability in the same way?
void AnnotatedCluster::getPathClearance(path *p, int& capability, int& clearance)
{
	while(p)
	{
		int tempclearance = p->n->getClearance(capability);
		if(tempclearance < clearance)
			clearance = tempclearance;
		
		p = p->next;
	}
}
