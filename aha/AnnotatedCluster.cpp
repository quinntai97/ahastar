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
		
	if(from->getClearance(capability) < clearance || to->getClearance(capability) < clearance)
		throw EntranceNodeIsNotTraversable();

	addEndpointsToAbstractGraph(from, to, aca);

	double weight = 1.0;
	graph* absg = aca->getAbstractGraph(1);
	node* absfrom = absg->getNode(from->getLabelL(kParent));
	node* absto = absg->getNode(to->getLabelL(kParent));
	addTransitionToAbstractGraph(absfrom, absto, capability, clearance, weight, aca);
}

// TODO: in a higher level function, need to call this for each capability type
void AnnotatedCluster::buildVerticalEntrances(int curCapability, AnnotatedClusterAbstraction* aca)
{

	if(aca == NULL)
		throw AnnotatedClusterAbstractionIsNullException();
	
	int mapheight = aca->getMap()->getMapHeight();
	int mapwidth = aca->getMap()->getMapWidth();

	node* candidateNode1 = 0; // multiple entrances may exist along a border so we track candidate w/ largest clearance so far
	node* candidateNode2 = 0;
	int candidateClearance=0;
		
	/* scan for vertical entrances along the eastern border */
	int x = this->getHOrig()+this->getWidth();
	if(x == mapwidth)
		return; // no eastern neighbour; nothing to build
	
	for(int y=getVOrig(); y<getVOrig()+getHeight(); y++)
	{
		node *c1 = aca->getNodeFromMap(x,y); // node in neighbouring cluster
		node *c2 = aca->getNodeFromMap(x-1, y); // border node in 'this' cluster
		int clearance = c1->getClearance(curCapability)>c2->getClearance(curCapability)?
							c2->getClearance(curCapability):c1->getClearance(curCapability);

		if(clearance > candidateClearance)
		{
			candidateNode1 = c1;
			candidateNode2 = c2;
			candidateClearance = clearance;
		}
		if(clearance == 0 && candidateClearance != 0) // hard obstacle encountered. build the largest entrance so far	
		{
				this->addEntrance(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
				candidateNode1 = candidateNode2 = 0;
				candidateClearance = 0;
		}
	}
	
	if(candidateClearance != 0)
		this->addEntrance(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
}

void AnnotatedCluster::buildHorizontalEntrances(int curCapability, AnnotatedClusterAbstraction* aca)
{

	if(aca == NULL)
		throw AnnotatedClusterAbstractionIsNullException();
	
	int mapheight = aca->getMap()->getMapHeight();
	int mapwidth = aca->getMap()->getMapWidth();

	node* candidateNode1 = 0; // multiple entrances may exist along a border so we track candidate w/ largest clearance so far
	node* candidateNode2 = 0;
	int candidateClearance=0;
		
	/* scan for horizontal entrances along the southern border */
	int	y = this->getVOrig()+this->getHeight();
	
	if(y == mapheight)
		return; // no southern neighbour; nothing to build

	for(int x=getHOrig(); x<getHOrig()+getWidth(); x++)
	{
		node *c1 = aca->getNodeFromMap(x,y); // node in neighbouring cluster
		node *c2 = aca->getNodeFromMap(x, y-1); // border node in 'this' cluster
		int clearance = c1->getClearance(curCapability)>c2->getClearance(curCapability)?
							c2->getClearance(curCapability):c1->getClearance(curCapability);

		if(clearance > candidateClearance)
		{
			candidateNode1 = c1;
			candidateNode2 = c2;
			candidateClearance = clearance;
		}
		if(clearance == 0 && candidateClearance != 0) // hard obstacle encountered. build the largest entrance so far	
		{
				this->addEntrance(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
				candidateNode1 = candidateNode2 = 0;
				candidateClearance = 0;
		}
	}
	
	if(candidateClearance != 0)
		this->addEntrance(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
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
	
	/* need to add nodes to abstract graph to represent the entrance; some entrances may share endpoints so we minimise graph size by reusing nodes */
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
	edge* interedge = g->findAnnotatedEdge(from,to, capability, clearance, weight); // can we re-use any edge that fits caps/clearance criteria?
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
/* some notes:
	When we add a new abstract node (or endpoint) it must be connected to all the other entrance endpoints in the cluster. 
	In the worst case [numCapabilities*numAgentSizes] number of edges need to be created. Creating an edge requires running AnnotatedA*. 
	This is expensive, especially for short paths. So, we try to minimise this by checking if we already found an optimal length path
	between the two endpoints which is traversable using the given capability.
*/
void AnnotatedCluster::connectEntranceEndpoints(node* newendpoint, AnnotatedClusterAbstraction* aca)
{
	for(int i=0; i<getParents().size(); i++)
	{
		node* existingendpoint = getParents()[i];
		
		switch(aca->getQualityParam())
		{
			case ACAUtil::kLowQualityAbstraction:
				{
					for(int capindex=0; capindex < NUMCAPABILITIES ; capindex++) // assumes all single-terrain first, multi-terrain last
					{
						int capability = capabilities[capindex];
						for(int sizeindex = NUMAGENTSIZES-1; sizeindex>=0; sizeindex--) // build largest single-capability first and re-use as many as possible
						{
							int size = agentsizes[sizeindex]; // assumes agentsize is ordered 0..n -- smallest to largest
							connectEntranceEndpointsForAGivenCapabilityAndSize(newendpoint, existingendpoint, capability, size, aca);
						}
					}
				}
				break;
			
			case ACAUtil::kHighQualityAbstraction:
				{
					for(int capindex=0; capindex < NUMCAPABILITIES ; capindex++) 
					{
						int capability = capabilities[capindex];
						for(int sizeindex = NUMAGENTSIZES-1; sizeindex>=0; sizeindex--)
						{
							int size = agentsizes[sizeindex]; // assumes agentsize is ordered 0..n -- smallest to largest
							connectEntranceEndpointsForAGivenCapabilityAndSize(newendpoint, existingendpoint, capability, size, aca);
						}
					}

				}
		
		
		
		}
	}
}

void AnnotatedCluster::connectEntranceEndpointsForAGivenCapabilityAndSize(node* newendpoint, node* existingendpoint, int capability, int size, AnnotatedClusterAbstraction* aca)
{
	graph* absg = aca->getAbstractGraph(1);
	double maxdist = getWidth()*getHeight(); // use maximum possible distance between these two endpoints as an upperbound param when searching for existing edges that may exist between these two endpoints
	edge* e=0;
	
	if(aca->getQualityParam() == ACAUtil::kLowQualityAbstraction)
	{
		e = absg->findAnnotatedEdge(newendpoint,existingendpoint,capability,size,maxdist); // try to re-use an existing path
		if(e == 0)
			findShortestPathBetweenTwoEndpoints(newendpoint, existingendpoint, capability, size, aca);
	}
	else
		findShortestPathBetweenTwoEndpoints(newendpoint, existingendpoint, capability, size, aca); // find them all
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
				e = absg->findAnnotatedEdge(n1,n2,capability,size,dist); 			
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
