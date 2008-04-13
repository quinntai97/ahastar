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

#include "glUtil.h"
#ifdef OS_MAC
#include "GLUT/glut.h"
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <sstream>


AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize)
	: AnnotatedMapAbstraction(m, searchalg)
{
	this->clustersize = clustersize;
	abstractions.push_back(new graph());	
	startid = goalid = -1;
	
	nodesExpanded = nodesTouched = peakMemory = 0;
	searchTime = 0;
}

AnnotatedClusterAbstraction::~AnnotatedClusterAbstraction()
{
	for(int i =0; i< clusters.size(); i++)
		delete clusters.at(i);
	clusters.erase(clusters.begin(), clusters.end());

//	delete (*pathCache.begin()).second;
	for(ACAUtil::pathTable::iterator it = pathCache.begin(); it != pathCache.end(); it++)
		delete (*it).second;
	pathCache.erase(pathCache.begin(), pathCache.end());
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

	//return graphAbstraction::distance(p);
	double dist=0;
	
	if(!p)
		return dist;
		
	graph *g = abstractions[p->n->getLabelL(kAbstractionLevel)];
	
	path* next = p->next;
	while(next)
	{
		dist+= h(p->n, next->n);
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

	nodesExpanded = nodesTouched = peakMemory = 0;
	searchTime = 0;

	node *absstart, *absgoal;
	if(start->getLabelL(kParent) == -1) // not an entrance endpoint (and hence not in abstract graph)	
	{	
		absstart = dynamic_cast<node*>(start->clone());
		absstart->setLabelL(kAbstractionLevel, start->getLabelL(kAbstractionLevel)+1);
		abstractions[1]->addNode(absstart);
		startid = absstart->getNum();
		start->setLabelL(kParent, startid); // reflect new parent
		AnnotatedCluster* startCluster = clusters[start->getParentCluster()];
		startCluster->addParent(absstart, this);
	}
	if(goal->getLabelL(kParent) == -1)
	{
		absgoal = dynamic_cast<node*>(goal->clone());
		absgoal->setLabelL(kAbstractionLevel, goal->getLabelL(kAbstractionLevel)+1);
		abstractions[1]->addNode(absgoal);
		goalid = absgoal->getNum();
		goal->setLabelL(kParent, goalid);
		AnnotatedCluster* goalCluster = clusters[goal->getParentCluster()];
		goalCluster->addParent(absgoal, this);
	}
}

/* Remove any nodes we added into the abstract graph to facilitate some search query. 
	NB:	startid/goalid are actually index positions of the node in the array stored by the graph class.
		When we remove start, our goalid is no longer an index to the goal node (HOG updates values when removing nodes) so we need to get it 
		again before we remove the goal
		
	TODO: merge common code
*/		
void AnnotatedClusterAbstraction::removeStartAndGoalNodesFromAbstractGraph()
{
	graph* g = abstractions[1];
	node* start = NULL;
	node* goal = NULL;

	assert(startid == -1 || (startid == g->getNumNodes()-1) || (startid == g->getNumNodes()-2)); // should be the last 2 nodes added
	assert(goalid == -1 || (goalid == g->getNumNodes()-1) || (goalid == g->getNumNodes()-2));


	if(startid != -1)
		start = g->getNode(startid);
	if(goalid != -1)
		goal = g->getNode(goalid);
		
	//	std::cout << "\n erasing start..";
	if(start)
	{		
		edge_iterator ei = start->getEdgeIter();
		edge* e = start->edgeIterNext(ei);
		while(e)
		{
			g->removeEdge(e);
			delete pathCache[e->getUniqueID()];
			pathCache.erase(e->getUniqueID());
			delete e;
			ei = start->getEdgeIter();
			e = start->edgeIterNext(ei);
		}
		
		g->removeNode(startid); 
		AnnotatedCluster* startCluster = clusters[start->getParentCluster()];
		startCluster->getParents().pop_back(); // always last one added
		
		startid = -1;
		node* originalStart = getNodeFromMap(start->getLabelL(kFirstData), start->getLabelL(kFirstData+1));
		originalStart->setLabelL(kParent, startid);
		delete start;
	}

	//	std::cout << " erasing goal...";
	if(goal)
	{		
		edge_iterator ei = goal->getEdgeIter();
		edge* e = goal->edgeIterNext(ei);
		while(e)
		{
			g->removeEdge(e);
			delete pathCache[e->getUniqueID()];
			pathCache.erase(e->getUniqueID());
			delete e;
			ei = goal->getEdgeIter();
			e = goal->edgeIterNext(ei);
		}
		
		g->removeNode(goal->getNum()); 
		AnnotatedCluster* goalCluster = clusters[goal->getParentCluster()];
		goalCluster->getParents().pop_back();

		goalid = -1;
		node* originalGoal = getNodeFromMap(goal->getLabelL(kFirstData), goal->getLabelL(kFirstData+1));
		originalGoal->setLabelL(kParent, startid);
		delete goal;
	}
}

void AnnotatedClusterAbstraction::addPathToCache(edge* e, path* p)
{
	if(e == NULL || p == NULL)
		return;
	
	pathCache[e->getUniqueID()] = p;
}

path* AnnotatedClusterAbstraction::getPathFromCache(edge* e)
{
	if(e == NULL)
		return 0;

	return pathCache[e->getUniqueID()];
}

void AnnotatedClusterAbstraction::openGLDraw()
{
	Map* map = this->getMap();
	graph* g1 = abstractions[1];
		
	/* draw transitions */
	edge_iterator edgeIter; 
	edgeIter = g1->getEdgeIter();
	edge* e = g1->edgeIterNext(edgeIter);
	while(e)
	{
		node *n1, *n2; 		
		double x1, x2, y1, y2;

		path* thepath = pathCache[e->getUniqueID()];
		if(thepath)
		{
			node* efrom = abstractions[1]->getNode(e->getFrom()); 
			node* eto = abstractions[1]->getNode(e->getTo());
			
			glLineWidth(1.0f);
			while(thepath->next)
			{
				n1 = thepath->n;
				n2 = thepath->next->n;

				x1 = n1->getLabelF(kXCoordinate); x2 = n2->getLabelF(kXCoordinate);
				y1 = n1->getLabelF(kYCoordinate); y2 = n2->getLabelF(kYCoordinate);
				
				glColor3f (0.7F, 0.5F, 0.5F);
				glBegin(GL_LINES);
				glVertex3f(x1, y1, -0.010);
				glVertex3f(x2, y2, -0.010);
				glEnd();
				
				thepath = thepath->next;
			}
		}
		e = g1->edgeIterNext(edgeIter);							
	}

	node_iterator nodeIter;
	nodeIter = g1->getNodeIter();
	node* absn = g1->nodeIterNext(nodeIter);
	while(absn)
	{
		glLineWidth(3.0f);
		
		node* n = getNodeFromMap(absn->getLabelL(kFirstData), absn->getLabelL(kFirstData+1));
		GLdouble x1 = n->getLabelF(kXCoordinate);
		GLdouble y1 = n->getLabelF(kYCoordinate);
		
//		std::cout <<"\n node @ "<<x1<<","<<y1;
		
		glColor3f (0.6F, 0.4F, 0.4F);
		glBegin(GL_QUADS);
		glVertex3f(x1-0.01, y1-0.01, -0.010);
		glVertex3f(x1+0.01, y1-0.01, -0.010);
		glVertex3f(x1+0.01, y1+0.01, -0.010);
		glVertex3f(x1-0.01, y1+0.01, -0.010);		
		glEnd();
		
		absn = g1->nodeIterNext(nodeIter);	
	}

	glLineWidth(1.0f);
}