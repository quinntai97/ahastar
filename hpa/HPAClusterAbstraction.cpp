/*
 *  HPAClusterAbstraction.cpp
 *  hog
 *
 *  Created by dharabor on 10/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "HPAClusterAbstraction.h"
#include "HPACluster.h"
#include "HPAClusterFactory.h"
#include "ClusterNode.h"
#include "ClusterNodeFactory.h"
#include "ClusterAStar.h"

#include "map.h"
#include "NodeFactory.h"
#include "EdgeFactory.h"
#include "path.h"
#include <stdexcept>
#include <sstream>

const unsigned int DEFAULTCLUSTERSIZE = 10;

HPAClusterAbstraction::HPAClusterAbstraction(Map* m, HPAClusterFactory* _cf, 
	INodeFactory* _nf, IEdgeFactory* _ef, bool allowDiagonals_) :
		GenericClusterAbstraction(m, _cf, _nf, _ef, allowDiagonals_),
		clustersize(DEFAULTCLUSTERSIZE)
{	
	nodesExpanded = nodesTouched = nodesGenerated = 0;
	searchTime = 0;
}

HPAClusterAbstraction::~HPAClusterAbstraction()
{
}


void 
HPAClusterAbstraction::buildClusters()
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
				
			HPACluster *cluster = static_cast<HPACluster*>(
					getClusterFactory()->createCluster(x,y,this));
			cluster->setWidth(cwidth);
			cluster->setHeight(cheight);
			addCluster( cluster ); // nb: also assigns a new id to cluster
			cluster->buildCluster();
		}
}

/* Remove any nodes we added into the abstract graph to facilitate some search 
 * query. 
 * NB: nodes removed in reverse order to creation (i.e. goal, then start)
 */
void 
HPAClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph() throw(std::runtime_error)
{
	graph* g = abstractions[1];
	ClusterNode* start = dynamic_cast<ClusterNode*>(g->getNode(startid));
	ClusterNode* goal = dynamic_cast<ClusterNode*>(g->getNode(goalid));
	
	//	std::cout << " erasing goal...";
	if(goal)
	{		
		edge_iterator ei = goal->getEdgeIter();
		edge* e = goal->edgeIterNext(ei);
		while(e)
		{
			g->removeEdge(e);
			deletePathFromCache(e);
			delete e;
			ei = goal->getEdgeIter();
			e = goal->edgeIterNext(ei);
		}
		
		HPACluster* goalCluster = static_cast<HPACluster*>(
				this->getCluster(goal->getParentClusterId()));
		goalCluster->removeParent(goal->getUniqueID());
		g->removeNode(goal->getNum()); 

		goalid = -1;
		node* originalGoal = getNodeFromMap(goal->getLabelL(kFirstData), 
				goal->getLabelL(kFirstData+1));
		originalGoal->setLabelL(kParent, goalid);
		delete goal;
		goal = 0;
	}
	
	//	std::cout << "\n erasing start..";
	if(start)
	{		
		edge_iterator ei = start->getEdgeIter();
		edge* e = start->edgeIterNext(ei);
		while(e)
		{
			this->deletePathFromCache(e);
			g->removeEdge(e);
			delete e;
			ei = start->getEdgeIter();
			e = start->edgeIterNext(ei);
		}
		
		HPACluster* startCluster = static_cast<HPACluster*>(this->getCluster(
				start->getParentClusterId()));
		startCluster->removeParent(start->getUniqueID()); 
		g->removeNode(startid); 
		
		startid = -1;
		node* originalStart = getNodeFromMap(start->getLabelL(kFirstData), 
				start->getLabelL(kFirstData+1));
		originalStart->setLabelL(kParent, startid);
		delete start;
		start=0;
	}
}

void 
HPAClusterAbstraction::insertStartAndGoalNodesIntoAbstractGraph(node* _start, 
		node* _goal) throw(std::invalid_argument)
{
	ClusterNode* start = static_cast<ClusterNode*>(_start);
	ClusterNode* goal = static_cast<ClusterNode*>(_goal);
	
	if(start == NULL || goal == NULL)
		throw std::invalid_argument("insertion error: null start or goal");
		
	if(start->getLabelL(kAbstractionLevel) != 0 || goal->getLabelL(kAbstractionLevel) != 0)
		throw std::invalid_argument("insertion error: start or goal is an abstract node (only non-abstract nodes can be inserted into the abstract graph)");

	nodesExpanded = nodesTouched = nodesGenerated = 0;
	searchTime = 0;

	if(start->getLabelL(kParent) == -1) // only add nodes that don't already exist in the abstract graph
	{	
		ClusterNode* absstart;
		HPACluster* startCluster = static_cast<HPACluster*>(
				this->getCluster(start->getParentClusterId()));
	
		absstart = static_cast<ClusterNode*>(start->clone());
		absstart->setLabelL(kAbstractionLevel, start->getLabelL(kAbstractionLevel)+1);
		abstractions[1]->addNode(absstart);
		startid = absstart->getNum();
		startCluster->addParent(absstart);
		start->setLabelL(kParent, startid);
		this->startid = startid;
		int numnodes = abstractions[1]->getNumNodes();
		assert(startid+1 == numnodes);

		nodesExpanded = startCluster->getSearchAlgorithm()->getNodesExpanded();
		nodesTouched = startCluster->getSearchAlgorithm()->getNodesTouched();
		nodesGenerated = startCluster->getSearchAlgorithm()->getNodesGenerated();
		searchTime = startCluster->getSearchAlgorithm()->getSearchTime();
	}
	if(goal->getLabelL(kParent) == -1)
	{
		ClusterNode* absgoal;
		HPACluster* goalCluster =  static_cast<HPACluster*>(
				this->getCluster(goal->getParentClusterId()));

		absgoal = static_cast<ClusterNode*>(goal->clone());
		absgoal->setLabelL(kAbstractionLevel, goal->getLabelL(kAbstractionLevel)+1);
		abstractions[1]->addNode(absgoal);
		goalid = absgoal->getNum();
		goalCluster->addParent(absgoal);
		goal->setLabelL(kParent, goalid);
		this->goalid = goalid;
		
		nodesExpanded+= goalCluster->getSearchAlgorithm()->getNodesExpanded();
		nodesTouched+= goalCluster->getSearchAlgorithm()->getNodesTouched();
		nodesGenerated+= goalCluster->getSearchAlgorithm()->getNodesGenerated();
		searchTime+= goalCluster->getSearchAlgorithm()->getSearchTime();
	}
}

void 
HPAClusterAbstraction::print(std::ostream& out)
{
	GenericClusterAbstraction::print(out);
	out << std::endl << " Cluster Size: "<<getClusterSize()<<std::endl;
}


// returns heuristic lowerbound on the distance between two tiles.
// if diagonal moves are allowed this is the octile distance.
// if diagonal moves are not allowed, this is the manhattan distance.
double 
HPAClusterAbstraction::h(node* a, node* b)
{
	if(a == NULL || b == NULL) 
		throw std::invalid_argument("null node");
	
	double answer = 0.0;
	if(getAllowDiagonals())
	{
		int x1 = a->getLabelL(kFirstData);
		int x2 = b->getLabelL(kFirstData);
		int y1 = a->getLabelL(kFirstData+1);
		int y2 = b->getLabelL(kFirstData+1);
		
		const double root2m1 = ROOT_TWO-1;//sqrt(2.0)-1;
		if (fabs(x1-x2) < fabs(y1-y2))
			answer = root2m1*fabs(x1-x2)+fabs(y1-y2);
		else
			answer = root2m1*fabs(y1-y2)+fabs(x1-x2);
	}
	else
	{
		int ax = a->getLabelL(kFirstData);
		int ay = a->getLabelL(kFirstData+1);
		int bx = b->getLabelL(kFirstData);
		int by = b->getLabelL(kFirstData+1);
		//std::cout << "from: "<<ax<<","<<ay<<") to: ("<<bx<<","<<by<<") ";

		int deltax = ax - bx;
		if(deltax < 0) deltax *=-1;

		int deltay = ay - by;
		if(deltay < 0) deltay *=-1;

		//std::cout << "deltax: "<<deltax<<" deltay: "<<deltay<<std::endl;
		answer = deltax + deltay;
	}

	return answer;
}

void 
HPAClusterAbstraction::verifyClusters()
{
//	cluster_iterator iter = getClusterIter();
//	HPACluster* cluster = clusterIterNext(iter);
//	while(cluster)
//	{
//		cluster->verifyCluster();
//		cluster = clusterIterNext(iter);
//	}
}
