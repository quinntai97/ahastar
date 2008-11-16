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
#include "HPAUtil.h"

#include "ClusterAStar.h"
#include "graph.h"

#include <stdexcept>
#include <sstream>
#include <string>

unsigned HPACluster::uniqueClusterIdCnt = 0;

HPACluster::HPACluster(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* _alg)
{
	init(x, y, _width, _height, _alg);
}

HPACluster::HPACluster(const int x, const int y, const int _width, const int _height) 
{
	init(x, y, _width, _height, new ClusterAStar());

}

void HPACluster::init(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* _alg)
{
	this->clusterId = ++uniqueClusterIdCnt;
	startx = x;
	starty = y;
	width = _width;
	height = _height;
	alg = _alg;
}

HPACluster::~HPACluster()
{
	delete alg;
	nodes.erase(nodes.begin(), nodes.end());
	parents.erase(parents.begin(), parents.end());
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

void HPACluster::addParent(node* _parentnode, HPAClusterAbstraction* hpamap) throw(std::invalid_argument)
{
	if(hpamap == NULL || _parentnode == NULL)
		throw std::invalid_argument("HPACluster::addParent called with NULL arguments");

	ClusterNode* parentnode = dynamic_cast<ClusterNode*>(_parentnode);

	if(parentnode == NULL)
		throw std::invalid_argument("HPACluster::addParent; failed trying to cast node parameter to type ClusterNode");
		
	if(parentnode->getParentClusterId() != -1)
	{
		if(parentnode->getParentClusterId() != this->getClusterId())
		{
			std::stringstream ss;
			ss << "HPACluster::addParent: Tried to add to cluster "<<this->getClusterId() << " node @ (" <<parentnode->getLabelL(kFirstData)<<
				","<<parentnode->getLabelL(kFirstData+1)<< ") but this node is already assigned to cluster "<<parentnode->getParentClusterId()<<std::endl;
			throw std::invalid_argument(ss.str());
		}
		else
		{
			std::cout << "\nWARNING: skipping parent node @ ("<<parentnode->getLabelL(kFirstData)<<","<<parentnode->getLabelL(kFirstData+1)<<"); already belongs to cluster "<<getClusterId()<<std::endl;
			return;
		}
	}
	else
		parentnode->setParentClusterId(this->getClusterId());

	if(hpamap->getAbstractGraph(1)->getNode(parentnode->getNum()) == NULL)
	{
			std::stringstream ss;
			ss << "HPACluster::addParent: Tried to add to cluster "<<this->getClusterId() << " node @ (" <<parentnode->getLabelL(kFirstData)<<
				","<<parentnode->getLabelL(kFirstData+1)<< ") but this node is not in the abstract graph "<<std::endl;
			throw std::invalid_argument(ss.str());
	}
	
	this->insertParent(parentnode,hpamap);	
	parents[parentnode->getUniqueID()] = parentnode;
}

void HPACluster::removeParent(int clusterId)
{
	node* n = nodes[clusterId];
	if(n != 0)
	{
		nodes.erase(clusterId); // only removed from parents hashmap
	}	
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

void HPACluster::buildEntrances(HPAClusterAbstraction* hpamap)
{

}

// inserts a new parent node into the abstract graph by connecting it to all other parents in the cluster
void HPACluster::insertParent(node* absStart, HPAClusterAbstraction* hpamap)
{	
	for(nodeTable::iterator nodeIter = parents.begin(); nodeIter != parents.end(); nodeIter++)
	{
		node* absGoal = (*nodeIter).second;
		graph* absg = hpamap->getAbstractGraph(1);
		
		alg->setCorridorNodes(&nodes);

		// get low-level nodes
		node* from = hpamap->getNodeFromMap(absStart->getLabelL(kFirstData),absStart->getLabelL(kFirstData+1)); 
		node* to = hpamap->getNodeFromMap(absGoal->getLabelL(kFirstData),absGoal->getLabelL(kFirstData+1)); 

		path* solution = alg->getPath(hpamap, from, to);
		if(solution != 0)
		{
			double dist = hpamap->distance(solution);
			edge* e = new edge(from->getNum(), to->getNum(), dist);
			absg->addEdge(e);
			hpamap->addPathToCache(e, solution);				
			//std::cout << "\n adding way cool edege for cluster "<<getClusterId();
		}
		
		/* record some metrics about the operation */
		hpamap->setNodesExpanded(hpamap->getNodesExpanded() + alg->getNodesExpanded());
		hpamap->setNodesTouched(hpamap->getNodesTouched() + alg->getNodesTouched());
		hpamap->setPeakMemory(alg->getPeakMemory()>hpamap->getPeakMemory()?alg->getPeakMemory():hpamap->getPeakMemory());
		hpamap->setSearchTime(hpamap->getSearchTime() + alg->getSearchTime());
	}
}

