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

CannotBuildEntranceToSelfException::CannotBuildEntranceToSelfException(node* n1, node* n2, int caps, int clearance) : EntranceException(n1, n2)
{
	std::ostringstream oss;
	oss << 	"tried to build an entrance using two nodes from the same cluster. Endpoint1 @ ";
	oss << "("<<endpoint1->getLabelL(kFirstData) << ", "<< endpoint1->getLabelL(kFirstData+1)<< " ) ";
	oss << " Endpoint2 @ "<< "("<<endpoint2->getLabelL(kFirstData) << ", "<< endpoint2->getLabelL(kFirstData+1)<< " ) ";
	oss << " capability: "<<caps<<" clearance: "<<clearance;
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

void AnnotatedCluster::addParent(node* parentnode)
{
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

void AnnotatedCluster::addInterEdge(node* from, node* to, int capability, int clearance, AnnotatedClusterAbstraction* aca) 
	throw(EntranceNodeIsNullException, EntranceNodesAreIdenticalException, CannotBuildEntranceFromAbstractNodeException, 
		CannotBuildEntranceToSelfException, EntranceNodesAreNotAdjacentException, EntranceNodeIsNotTraversable, InvalidClearanceParameterException)
{
	if(from == NULL || to == NULL)
		throw EntranceNodeIsNullException();

	if( from == to)
		throw EntranceNodesAreIdenticalException();
	
	if(clearance <= 0)
		throw InvalidClearanceParameterException();
		
	if(from->getClearance(capability) < clearance || to->getClearance(capability) < clearance)
		throw EntranceNodeIsNotTraversable();
					
	if(from->getLabelL(kAbstractionLevel) != 0 || to->getLabelL(kAbstractionLevel) != 0)
		throw CannotBuildEntranceFromAbstractNodeException();
		
	if(from->getParentCluster() == to->getParentCluster())
		throw CannotBuildEntranceToSelfException(from, to, capability, clearance);
	
	if(AnnotatedAStar::getDirection(from, to) == kStay)
		throw EntranceNodesAreNotAdjacentException();
		
		
	double weight = 1.0;
//	int capability = from->getTerrainType()|to->getTerrainType();
//	int clearance = from->getClearance(capability)>to->getClearance(capability)?to->getClearance(capability):from->getClearance(capability);		
	
	addEdgeToAbstractGraph(from, to, capability, clearance, weight, aca);
	
}

void AnnotatedCluster::addEdgeToAbstractGraph(node* from, node* to, int capability, int clearance, double weight, AnnotatedClusterAbstraction* aca)
{
	/* need to add nodes to abstract graph to represent the entrance; some entrances may share endpoints so we minimise graph size by reusing nodes */
	graph* g = aca->getAbstractGraph(1);
	
	node *absfrom, *absto;
	if(from->getLabelL(kParent) == -1)
	{
		absfrom = dynamic_cast<node*>(from->clone());
		absfrom->setLabelL(kAbstractionLevel, 1);
		g->addNode(absfrom);
		AnnotatedCluster* fromCluster = aca->getCluster(absfrom->getParentCluster());		
		fromCluster->addParent(absfrom);

		from->setLabelL(kParent, absfrom->getNum());
	}
	else
		absfrom = g->getNode(from->getLabelL(kParent));
	
	if(to->getLabelL(kParent) == -1)
	{
		absto = dynamic_cast<node*>(to->clone());
		absto->setLabelL(kAbstractionLevel, 1);
		g->addNode(absto);
		AnnotatedCluster* toCluster = aca->getCluster(absto->getParentCluster());
		toCluster->addParent(absto);

		to->setLabelL(kParent, absto->getNum());
	}
	else
		absto = g->getNode(to->getLabelL(kParent));
				
	/* annotate the edge representing the entrance with appropriate capabilities and clearance values */
	edge* interedge = g->findAnnotatedEdge(absfrom,absto, capability, clearance); // can we re-use any edge that fits caps/clearance criteria?
	if(interedge == 0)
	{
		//std::cout << "\nadding new edge to abs graph: "<<from->getLabelL(kFirstData)<<","<<from->getLabelL(kFirstData+1)<<" and " << to->getLabelL(kFirstData)<<","<<to->getLabelL(kFirstData+1) << " caps: "<<capability<<" clearance: "<<clearance;
		interedge = new edge(absfrom->getNum(), absto->getNum(), weight);
		interedge->setClearance(capability,clearance);
		g->addEdge(interedge);
	}
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
				this->addInterEdge(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
				candidateNode1 = candidateNode2 = 0;
				candidateClearance = 0;
		}
	}
	
	if(candidateClearance != 0)
		this->addInterEdge(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
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
	int y = this->getVOrig()+this->getHeight();
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
				this->addInterEdge(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
				candidateNode1 = candidateNode2 = 0;
				candidateClearance = 0;
		}
	}
	
	if(candidateClearance != 0)
		this->addInterEdge(candidateNode2,candidateNode1, curCapability, candidateClearance, aca);
}

void AnnotatedCluster::buildEntrances(AnnotatedClusterAbstraction* aca)
{
//	if(aca == NULL)
//		throw AnnotatedClusterAbstractionIsNullException();

	for(int i=0; i<NUMCAPABILITIES; i++)
	{
		buildVerticalEntrances(capabilities[i], aca);
		buildHorizontalEntrances(capabilities[i], aca);
	}
}
