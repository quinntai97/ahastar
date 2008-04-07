/*
 *  AnnotatedClusterAbstraction.cpp
 *  hog
	
	Extends a HPA Cluster in several ways:
		- Is concerned with annotated nodes
		- each cluster must be free of hard obstacles (ie. intraversable nodes). <- implicit if annotations are OK
		- each cluster must have a rectangular or square shape <- but, if these are fscked, we might try to add hard obstacles; need exception
		- each node assigned to the cluster will not be assigned to some other cluster
		- each node in the cluster will not have a larger clearance than the origin node (node at top-left corner of the cluster);
			the clearance value we use for this test is the superset of all single terrain types (currently, kGround|kTrees)
 *
 *  Created by Daniel Harabor on 22/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedClusterAbstraction.h"
#include "AnnotatedCluster.h"
#include "clusterAbstraction.h"
#include "AHAConstants.h"
#include "AnnotatedAStar.h"
#include <sstream>


AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize)
	: AnnotatedMapAbstraction(m, searchalg)
{
	this->clustersize = clustersize;
	abstractions.push_back(new graph());	
	startid = goalid = -1;
}

void AnnotatedClusterAbstraction::addCluster(AnnotatedCluster* ac) 
{ 
	ac->setClusterId(clusters.size()); clusters.push_back(ac); 
} 


AnnotatedCluster* AnnotatedClusterAbstraction::getCluster(int cid)
{
	if(cid < 0 || cid >= clusters.size())
		return 0;
		
	return clusters[cid];
}

void AnnotatedClusterAbstraction::buildClusters(IAnnotatedClusterFactory* acfactory)
{	
	int mapwidth = this->getMap()->getMapWidth();
	int mapheight= this->getMap()->getMapHeight();

	/* need to split the map into fixed-size cluster areas that will form the basis of our abstract graph building later */
	int csize = getClusterSize();
	for(int x=0; x<mapwidth; x+=csize)
		for(int y=0; y<mapheight; y+= csize)
		{	
			int cwidth=csize;
			if(x+cwidth > mapwidth)
				cwidth = mapwidth - x;
			int cheight=csize;
			if(y+cheight > mapheight)
				cheight = mapheight - y;
				
			AnnotatedCluster *ac = /*new AnnotatedCluster(x,y,cwidth,cheight);//*/acfactory->createCluster(x,y,cwidth,cheight);
			addCluster( ac ); // nb: also assigns a new id to cluster
			ac->addNodesToCluster(this);
		}
}


void AnnotatedClusterAbstraction::buildEntrances()
{
	for(int i=0; i<clusters.size(); i++)
	{
		AnnotatedCluster* ac = clusters[i];
		ac->buildEntrances(this);
	}
}

double AnnotatedClusterAbstraction::distance(path* p)
{
	double dist=0;
	
	if(!p)
		return dist;
	
	path* next = p->next;
	while(next)
	{
		dist+= this->h(p->n, next->n);
		p = next;
		next = p->next;
	}
	
	return dist;
}

// TODO: remove code duplication from this method
void AnnotatedClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(node* start, node* goal) 
	throw(NodeIsNullException, NodeHasNonZeroAbstractionLevelException)
{
	if(start == NULL || goal == NULL)
		throw NodeIsNullException();

	if(start->getLabelL(kAbstractionLevel) != 0 || goal->getLabelL(kAbstractionLevel) != 0)
		throw NodeHasNonZeroAbstractionLevelException();

	node *absstart, *absgoal;
	if(start->getLabelL(kParent) == -1) // not an entrance endpoint (and hence not in abstract graph)	
	{	
		absstart = dynamic_cast<node*>(start->clone());
		abstractions[1]->addNode(absstart);
		startid = absstart->getNum();
		AnnotatedCluster* startCluster = clusters[start->getParentCluster()];
		startCluster->addParent(absstart, this);
	}
	if(goal->getLabelL(kParent) == -1)
	{
		absgoal = dynamic_cast<node*>(goal->clone());
		abstractions[1]->addNode(absgoal);
		goalid = absgoal->getNum();
		AnnotatedCluster* goalCluster = clusters[goal->getParentCluster()];
		goalCluster->addParent(absgoal, this);
	}
}

/* Remove any nodes we added into the abstract graph to facilitate some search query. 
	NB:	startid/goalid are actually index positions of the node in the array stored by the graph class.
		When we remove start, our goalid is no longer an index to the goal node (HOG updates values when removing nodes) so we need to get it 
		again before we remove the goal
*/		
void AnnotatedClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph()
{
	graph* g = abstractions[1];
	node* start = NULL;
	node* goal = NULL;

	if(startid != -1)
		start = g->getNode(startid);
	if(goalid != -1)
		goal = g->getNode(goalid);
		
	if(start)
	{
		AnnotatedCluster* startCluster = clusters[start->getParentCluster()];
		startCluster->getParents().pop_back(); // always last one added
		g->removeNode(startid); 
		startid = -1;
		delete start;
	}

	if(goal)
	{
		AnnotatedCluster* goalCluster = clusters[goal->getParentCluster()];
		goalCluster->getParents().pop_back();
		g->removeNode(goal->getNum()); 
		goalid = -1;
		delete goal;
	}
}