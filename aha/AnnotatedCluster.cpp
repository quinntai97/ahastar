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
	return;
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

void AnnotatedCluster::addInterEdge(node* from, node* to, AnnotatedClusterAbstraction* aca) 
	throw(EntranceNodeIsNullException, EntranceNodesAreIdenticalException, CannotBuildEntranceFromAbstractNodeException, 
		CannotBuildEntranceToSelfException, EntranceNodeIsHardObstacleException, EntranceNodesAreNotAdjacentException)
{
	if(from == NULL || to == NULL)
		throw EntranceNodeIsNullException();

	if( from == to)
		throw EntranceNodesAreIdenticalException();
		
	if(from->getClearance(from->getTerrainType()) == 0 || to->getClearance(to->getTerrainType()) == 0)
		throw EntranceNodeIsHardObstacleException();
	
	if(from->getLabelL(kAbstractionLevel) != 0 || to->getLabelL(kAbstractionLevel) != 0)
		throw CannotBuildEntranceFromAbstractNodeException();
		
	if(from->getParentCluster() == to->getParentCluster())
		throw CannotBuildEntranceToSelfException();
	
	if(AnnotatedAStar::getDirection(from, to) == kStay)
		throw EntranceNodesAreNotAdjacentException();
		
		
	double weight = 1.0;
	int capability = from->getTerrainType()|to->getTerrainType();
	int clearance = from->getClearance(capability)>to->getClearance(capability)?to->getClearance(capability):from->getClearance(capability);		
	
	addEdgeToAbstractGraph(from, to, capability, clearance, weight, aca);
	
}

void AnnotatedCluster::addEdgeToAbstractGraph(node* from, node* to, int capability, int clearance, double weight, AnnotatedClusterAbstraction* aca)
{
	/* need to add nodes to abstract graph to represent the entrance */
	node* absfrom = dynamic_cast<node*>(from->clone());
	node* absto = dynamic_cast<node*>(to->clone());
	absfrom->setLabelL(kAbstractionLevel, 1);
	absto->setLabelL(kAbstractionLevel, 1);
	
	graph* g = aca->getAbstractGraph(1);
	g->addNode(absfrom);
	g->addNode(absto);
	
	/* need to annotate the edge representing the entrance with appropriate capabilities and clearance values so agents can determine if 
		the edge is traversable */
	edge* interedge = new edge(absfrom->getNum(), absto->getNum(), weight);
	interedge->setClearance(capability,clearance);
	g->addEdge(interedge);
		
	/* need to add the endpoints of the new entrance to their respective clusters */
	AnnotatedCluster* fromCluster = aca->getCluster(absfrom->getParentCluster());
	AnnotatedCluster* toCluster = aca->getCluster(absto->getParentCluster());
}
