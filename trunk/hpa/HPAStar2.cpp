/*
 *  HPAStar.cpp
 *  hog
 *
 *  Created by dharabor on 17/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAStar2.h"
#include "HPAClusterAbstraction.h"
#include "ClusterNode.h"
#include "HPACluster.h"
#include <stdexcept>

/* 
Find an abstract path and refine it using the path cache

 NB: sometimes we may require a cached path which was obtained by planning in the reverse direction to current requirements
 ie. we store the path from n1 -> n2, but we may require the path from n2 -> n1. 
 In such cases, we specify the id of the node that should be at the head of the path. if the cached path doesn't meet those
 requirements, we reverse it. 
*/
path* HPAStar2::getPath(graphAbstraction* aMap, node* _from, node* _to, reservationProvider *rp)
{
	resetMetrics();
	if(!_from || !_to || !aMap)
	{
		std::cerr << "HPAStar2::getPath failed. from, to & mapAbstraction parameters cannot be NULL"<<std::endl;
		return NULL;
	}

	if(_from->getLabelL(kAbstractionLevel) > 0 || _to->getLabelL(kAbstractionLevel) > 0)
	{
		std::cerr << "HPAStar2::getPath failed. from/to nodes must be non-abstract nodes"<<std::endl;
		return NULL; 
	}
		
	HPAClusterAbstraction* hpamap = dynamic_cast<HPAClusterAbstraction*>(aMap);
	assert(hpamap != 0); 		
	ClusterNode* from = dynamic_cast<ClusterNode*>(_from);
	ClusterNode* to = dynamic_cast<ClusterNode*>(_to);
					
	ClusterAStar castar;
	path* thepath=0;

	// if from/goal are in the same cluster, try to find a path directly
	if(from->getParentClusterId() == to->getParentClusterId())
	{
		HPACluster *cluster = hpamap->getCluster(from->getParentClusterId());
		castar.setCorridorNodes(cluster->getNodes());
		thepath = castar.getPath(aMap, from, to, rp);
		updateMetrics(castar);
	}
	
	// no direct path or from/goal not in the same cluster
	if(thepath==0)
	{
		hpamap->insertStartAndGoalNodesIntoAbstractGraph(from, to);		
		this->nodesExpanded += hpamap->getNodesExpanded();
		this->nodesTouched += hpamap->getNodesTouched();
		this->searchtime += hpamap->getSearchTime();
		if(this->peakmemory < hpamap->getPeakMemory())
			this->peakmemory = hpamap->getPeakMemory();
				
		// find a path in the abstract graph
		graph *absg = hpamap->getAbstractGraph(1); 
		node* absstart = absg->getNode(from->getLabelL(kParent));
		node* absgoal = absg->getNode(to->getLabelL(kParent));
		path* abspath = castar.getPath(aMap, absstart, absgoal);
		updateMetrics(castar);
		
		// refine the path
		if(abspath && refineAbstractPath)
			thepath = refinePath(abspath, hpamap, castar);

		hpamap->removeStartAndGoalNodesFromAbstractGraph();
		delete abspath;
	}
		
	//std::cout << "\n thepath distance: "<<aMap->distance(thepath);
	return thepath;
}

path* HPAStar2::refinePath(path* abspath, HPAClusterAbstraction* hpamap, ClusterAStar& castar) 
{
	graph *absg = hpamap->getAbstractGraph(1); 
	path* thepath = 0;
	path* tail;
	while(abspath->next)
	{
		path* segment;
		if(fastRefinement)
		{
			edge* e = absg->findEdge(abspath->n->getNum(), abspath->next->n->getNum());
			if(e == NULL)
				throw std::runtime_error("HPAStar2::getPath -- something went horribly wrong; couldn't find cached path. ");

			segment = hpamap->getPathFromCache(e)->clone();
			if(e->getFrom() != abspath->n->getNum()) // fix segments if necessary
				segment = segment->reverse();				
		}
		else
		{
			node* llstart = hpamap->getNodeFromMap(abspath->n->getLabelL(kFirstData), abspath->n->getLabelL(kFirstData+1));
			node* llgoal = hpamap->getNodeFromMap(abspath->next->n->getLabelL(kFirstData), abspath->next->n->getLabelL(kFirstData+1));
			segment = castar.getPath(hpamap,llstart, llgoal); 
			updateMetrics(castar);
		}
		
		// append segment to refined path
		if(thepath == 0)
			thepath = segment;										
		tail = thepath->tail();	

		// avoid overlap between successive segments (one ends where another begins)
		if(tail->n->getNum() == segment->n->getNum()) 
			tail->next = segment->next;
		
		abspath = abspath->next;
	}
}


void HPAStar2::updateMetrics(ClusterAStar& castar)
{	
	this->nodesExpanded += castar.getNodesExpanded();
	this->nodesTouched += castar.getNodesTouched();
	if(this->peakmemory < castar.getPeakMemory())
		this->peakmemory = castar.getPeakMemory();
	this->searchtime += castar.getSearchTime();
}

void HPAStar2::logFinalStats(statCollection* stats)
{
	ClusterAStar::logFinalStats(stats);
	
	stats->addStat("insNodesExpanded",getName(),getInsertNodesExpanded());
	stats->addStat("insNodesTouched",getName(),getInsertNodesTouched());
	stats->addStat("insPeakMemory",getName(),getInsertPeakMemory());
	stats->addStat("insSearchTime",getName(),getInsertSearchTime());
}

// debugging function
void HPAStar2::printPath(path* p)
{
	std::cout << "\n abstract path: ";
	while(p)
	{
		node* n = p->n;
		std::cout << "\n id: "<<n->getUniqueID()<<" node @ "<<n->getLabelL(kFirstData) << ","<<n->getLabelL(kFirstData+1);
		p = p->next;
	}		
}

void HPAStar2::resetMetrics()
{
	insertNodesExpanded = insertNodesTouched = insertPeakMemory = 0;
	insertSearchTime = 0;
	this->nodesExpanded = 0;
	this->nodesTouched = 0;
	this->peakmemory = 0;
	this->searchtime = 0;
}