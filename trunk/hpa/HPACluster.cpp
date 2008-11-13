/*
 *  HPACluster.cpp
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPACluster.h"
#include "HPAClusterAbstraction.h"
#include "ClusterNode.h"
#include "graph.h"
#include "HPAUtil.h"
#include <stdexcept>
#include <sstream>
#include <string>

unsigned HPACluster::uniqueClusterIdCnt = 0;

HPACluster::HPACluster(const int x, const int y, const int _width, const int _height) : startx(x), starty(y), width(_width), height(_height)
{
	this->clusterId = ++uniqueClusterIdCnt;
}

void HPACluster::addNode(node* _mynode) throw(std::invalid_argument)
{
	ClusterNode* mynode = dynamic_cast<ClusterNode*>(_mynode);
	if(mynode == NULL)
	{
		std::ostringstream oss;
		oss << "tried to add null node to cluster at origin: "<<this->getHOrigin()<<", "<<this->getVOrigin()<<" with id: "<<this->getClusterId();
		throw std::invalid_argument(oss.str().c_str());
	}

	if(nodes[mynode->getUniqueID()] != NULL) // already added
		return;

	int nx = mynode->getLabelL(kFirstData);
	int ny = mynode->getLabelL(kFirstData+1);
	
	if(nx < this->getHOrigin() || nx >= this->getHOrigin() + this->getWidth())
	{
		std::ostringstream oss;
		oss << "node @ " << nx<<","<<ny<<" has x coordinate outside cluster boundary. Cluster horigin: "<<this->getHOrigin();
		oss <<" yorigin: "<<this->getVOrigin()<<" width: "<<this->getWidth()<<" height: "<<this->getHeight();
		throw std::invalid_argument(oss.str().c_str());
	}
	
	if(mynode->getParentClusterId() != -1)
	{
		std::ostringstream oss;
		oss << "node @ "<< nx <<","<< ny <<" is already assigned to cluster "<<mynode->getParentCluster();
		throw std::invalid_argument(oss.str().c_str());
	}
			
	mynode->setParentClusterId(this->getClusterId());
	nodes[mynode->getUniqueID()] = mynode;
}

void HPACluster::addParent(node* _parentnode, HPAClusterAbstraction* aca)
{
	ClusterNode* parentnode = dynamic_cast<ClusterNode*>(_parentnode);
	parentnode->setParentClusterId(this->getClusterId());
	nodes[parentnode->getUniqueID()] = parentnode;
	
/*	if(parentnode->getParentCluster() != -1)
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
*/
}

void HPACluster::removeParent(int clusterId)
{
	node* n = nodes[clusterId];
	if(n != 0)
	{
		nodes.erase(clusterId); // only removed from parents hashmap
	}	
}

bool HPACluster::hasaParent(node* n)
{
	int nx = n->getLabelL(kFirstData);
	int ny = n->getLabelL(kFirstData+1);

	HPAUtil::nodeTable::iterator it = parents.begin();	
	while(it != parents.end())
	{
		node* p = (*it).second;
		if(p->getLabelL(kFirstData) == nx && p->getLabelL(kFirstData+1) == ny)
			return true;
		it++;	
	}
	
	return false;
}

/* add all traversable nodes in the cluster area to the cluster */
void HPACluster::addNodesToCluster(HPAClusterAbstraction* aMap) throw(std::invalid_argument)
{

	if(aMap == NULL)
		throw std::invalid_argument("tried to add nodes to cluster but cluster abstraction parameter is null");
		
	for(int x=this->getHOrigin(); x<getHOrigin()+getWidth(); x++)
		for(int y=this->getVOrigin(); y<getVOrigin()+getHeight(); y++)
		{
			addNode(aMap->getNodeFromMap(x,y));
		}
}

void HPACluster::buildEntrances(HPAClusterAbstraction* hpacaMap)
{

}

