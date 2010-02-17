/*
 *  AnnotatedClusterAbstraction.cpp
 *  hog
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
#include "map.h"

#include "glUtil.h"
#ifdef OS_MAC
#include "GLUT/glut.h"
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <sstream>


AnnotatedClusterAbstraction::AnnotatedClusterAbstraction(Map* m, AbstractAnnotatedAStar* searchalg, int clustersize, ACAUtil::GraphQualityParameter qual)
	: AnnotatedMapAbstraction(m, searchalg), quality(qual)
{
	this->clustersize = clustersize;
	abstractions.push_back(new graph());	
	startid = goalid = -1;
	
	nodesExpanded = nodesTouched = peakMemory = 0;
	searchTime = 0;
	drawClusters = false;
	drawClearance=false;
}

AnnotatedClusterAbstraction::~AnnotatedClusterAbstraction()
{
	for(unsigned int i =0; i< clusters.size(); i++)
		delete clusters.at(i);
	clusters.erase(clusters.begin(), clusters.end());

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
	if(cid < 0 || ((unsigned)cid) >= clusters.size())
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
	for(unsigned int i=0; i<clusters.size(); i++)
	{
		AnnotatedCluster* ac = clusters[i];
		ac->buildEntrances(this);
	}
	

	
	if(quality == ACAUtil::kLowQualityAbstraction)
	{	
		/* find & mark all dominated edges */
		graph* absg = this->getAbstractGraph(1);
		edge_iterator ei1, ei2;
		edge *first, *second;
		ei1 = absg->getEdgeIter();
		first = absg->edgeIterNext(ei1);
		while(first)
		{
			ei2 = ei1;
			second = absg->edgeIterNext(ei2);
			while(second)
			{
				findAndMarkDominatedTransition(first, second);
				second = absg->edgeIterNext(ei2);
			}
			first = absg->edgeIterNext(ei1);
		}
		
		/* delete all dominated edges */
		removeDominatedEdgesAndEndpoints();
	
	}

}

double AnnotatedClusterAbstraction::distance(path* p)
{

	//return graphAbstraction::distance(p);
	double dist=0;
	
	if(!p)
		return dist;
		
	
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
	throw(NodeIsNullException, NodeHasNonZeroAbstractionLevelException, NodeIsHardObstacleException)
{
	if(start == NULL || goal == NULL)
		throw NodeIsNullException();

	if(start->getClearance(start->getTerrainType()) == 0)
		throw NodeIsHardObstacleException(start, NULL);

	if(goal->getClearance(goal->getTerrainType()) == 0)
		throw NodeIsHardObstacleException(goal, NULL);
		
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

/* paths are cached in the direction of the edge (from, to) */
void AnnotatedClusterAbstraction::addPathToCache(edge* e, path* p)
{
	if(e == NULL || p == NULL)
		return;
	
	pathCache[e->getUniqueID()] = p;
}

/* Cache for known paths. Stores one path for each abstract edge. */
path* AnnotatedClusterAbstraction::getPathFromCache(edge* e)
{
	if(e == NULL)
		return 0;

	path* p = pathCache[e->getUniqueID()];
	return p;	
}

/* method in mapAbstraction makes some.. interesting.. decisions about abstraction hierarchies. doesn't work for us */
double AnnotatedClusterAbstraction::h(node* a, node*b) throw(NodeIsNullException)
{

	if(a == NULL || b == NULL) 
		throw NodeIsNullException();
	int x1 = a->getLabelL(kFirstData);
	int x2 = b->getLabelL(kFirstData);
	int y1 = a->getLabelL(kFirstData+1);
	int y2 = b->getLabelL(kFirstData+1);
	
	double answer = 0.0;
	const double root2m1 = ROOT_TWO-1;//sqrt(2.0)-1;
		if (fabs(x1-x2) < fabs(y1-y2))
			answer = root2m1*fabs(x1-x2)+fabs(y1-y2);
	else
		answer = root2m1*fabs(y1-y2)+fabs(x1-x2);
	return answer;
}

void AnnotatedClusterAbstraction::findAndMarkDominatedTransition(edge* first, edge* second)
{
	if(first == 0 || second == 0)
		return;
	
	int e1Capability = first->getCapability();
	int e1Clearance = first->getClearance(e1Capability);
	int e2Capability = second->getCapability();
	int e2Clearance = second->getClearance(e2Capability);
	
	edge *dominant, *dominated;
	dominant = dominated = 0;
	
	/* a dominant transition is one traversable by a capability involving fewer terrains and larger larger clearance */
	if(first->getClearance(e2Capability) >= e2Clearance)
	{
			dominant = first;
			dominated = second;
	}
	else
	{
		if(second->getClearance(e1Capability) >= e1Clearance)
		{
				dominant = second;
				dominated = first;
		}
	}
	
	if(dominant == 0) // transitions incompatible; no dominance relationship exists
		return;
	
	graph* g = this->getAbstractGraph(1);
	node* dmc1 = g->getNode(dominant->getFrom()); // dominant endpoint in cluster 1
	node* dmc2 = g->getNode(dominant->getTo()); // dominant endpoint in cluster 2
	node *dtc1, *dtc2; // dominated endpoints
	
	dtc1 = g->getNode(dominated->getFrom());
	dtc2 = g->getNode(dominated->getTo());
	
	if(dmc1 == 0 || dmc2 == 0 || dtc1 == 0 || dtc2 == 0) 
		return; // not all endpoints in graph. TODO: maybe throw exception here? 
	
	if(dmc1->getParentCluster() == dmc2->getParentCluster())
		return;
	if(dtc1->getParentCluster() == dtc2->getParentCluster())
		return;
	
	if(dmc1->getParentCluster() != dtc1->getParentCluster())
	{
		node* tmp = dtc1;
		dtc1 = dtc2;
		dtc2 = tmp;
		if(dmc1->getParentCluster() != dtc1->getParentCluster()) // no dominance relationship; transitions do not begin in same entrance
			return;
		if(dmc2->getParentCluster() != dtc2->getParentCluster())
			return;
	}
	if(dmc2->getParentCluster() != dtc2->getParentCluster())
	{
		node* tmp = dtc2;
		dtc2 = dtc1;
		dtc1 = tmp;
		if(dmc2->getParentCluster() != dtc2->getParentCluster())
			return;
		if(dmc1->getParentCluster() != dtc1->getParentCluster())
			return;
	}
	
	/* circuit must exist between the endpoints of the two edges. further, each edge must be traversable by any agent able
		to traverse the dominated edge. Otherwise, no dominance relationship exists */
	int MAX_INT = 2147483647;
	edge* intra1 = dmc1->findAnnotatedEdge(dtc1, dominated->getCapability(), dominated->getClearance(dominated->getCapability()), MAX_INT);
	edge* intra2 = dmc2->findAnnotatedEdge(dtc2, dominated->getCapability(), dominated->getClearance(dominated->getCapability()), MAX_INT);
	if( intra1 && intra2 )
	{	
		if(!dominated->getMarked())
			dominated->setMarked(true);
	}
}

void AnnotatedClusterAbstraction::removeDominatedEdgesAndEndpoints()
{
		std::vector<edge*> deleteQueue; 
		graph* absg = this->getAbstractGraph(1);

		edge_iterator ei = absg->getEdgeIter();
		edge* e = absg->edgeIterNext(ei);
		while(e)
		{
			if(e->getMarked())
				deleteQueue.push_back(e);
			e = absg->edgeIterNext(ei);
		}
		
		if(deleteQueue.size() == 0)
			return;
			
		while(deleteQueue.size() > 0)
		{
			edge* target = deleteQueue.back();
			node* n1 = absg->getNode(target->getFrom());
			node* n2 = absg->getNode(target->getTo());
			
			absg->removeEdge(target);
			delete target;
			
			/* if endpoints of dominated transition are not required by other inter-edges, we can delete them too */
			if(!hasMoreInterEdges(n1, absg)) 
			{
				removeDominatedNodeFromParentCluster(n1); 
				removeDominatedNodeFromGraph(n1, absg);			
			}
			if(!hasMoreInterEdges(n2, absg))
			{
				removeDominatedNodeFromParentCluster(n2); 
				removeDominatedNodeFromGraph(n2, absg);
			}
			deleteQueue.pop_back();
		}
		
		repair_kParent_Labels();
		//repairAbstractNodeCollectionInClusters();
}

void AnnotatedClusterAbstraction::removeDominatedNodeFromParentCluster(node* n)
{
	AnnotatedCluster* ac = this->getCluster(n->getParentCluster());
	std::vector<node*> *nodes = &(ac->getParents());
	for(unsigned int i=0; i<nodes->size(); i++)
	{
		node* tmp = nodes->at(i);
		if(tmp == n)
		{
			nodes->erase(nodes->begin()+i);
			return;
		}
	}
}

void AnnotatedClusterAbstraction::removeDominatedNodeFromGraph(node* n, graph* absg)
{
	edge_iterator ei = n->getEdgeIter();
	edge* e = n->edgeIterNext(ei);
	while(e)
	{
		absg->removeEdge(e);
		delete e;
		e = n->edgeIterNext(--ei);
	}
	absg->removeNode(n->getNum());
	node* lowlevelnode = this->getNodeFromMap(n->getLabelL(kFirstData), n->getLabelL(kFirstData+1));
	lowlevelnode->setLabelL(kParent, -1); // no longer abstracted by anything
	delete n;	
}

/* when endpoints of dominated edges are removed, the kParent labels of the nodes in the low-level graph are rendered incorrect
	(they store index positions of abstract nodes, which change as a result of the deletion). 
	This method repairs the problem.
*/
void AnnotatedClusterAbstraction::repair_kParent_Labels()
{
	graph* absg = this->getAbstractGraph(1);
	node_iterator ni = absg->getNodeIter();
	node* absn, *n;
	absn = absg->nodeIterNext(ni);
	while(absn)
	{	
		n = this->getNodeFromMap(absn->getLabelL(kFirstData), absn->getLabelL(kFirstData+1));
		n->setLabelL(kParent, absn->getNum());
		absn = absg->nodeIterNext(ni);
	}
}

/* repair by rebuilding abstract node collectio; re-add nodes from the abstract graph to the cluster marked as their parent.
	Easier than iterating through each cluster's nodes and working out what's OK vs not.
*/
void AnnotatedClusterAbstraction::repairAbstractNodeCollectionInClusters()
{	
	/* blow away the current collection of abstract nodes */
	int totalClusters = this->getNumClusters();
	for(int i=0; i<totalClusters; i++)
	{
		AnnotatedCluster* ac = this->getCluster(i);
		ac->getParents().clear();
	}
	
	graph* absg = this->getAbstractGraph(1);
	node_iterator ni = absg->getNodeIter();
	node* absn = absg->nodeIterNext(ni);
	while(absn)
	{
		AnnotatedCluster *ac = getCluster(absn->getParentCluster());
		ac->getParents().push_back(absn);
		absn = absg->nodeIterNext(ni);
	}
}

bool AnnotatedClusterAbstraction::hasMoreInterEdges(node* n, graph* absg)
{
	int nParentCluster = n->getParentCluster();
	edge_iterator ei = n->getEdgeIter();
	edge* e = n->edgeIterNext(ei);
	while(e)
	{
		node* to = absg->getNode(e->getTo());
		node* from = absg->getNode(e->getFrom());
		if(to->getParentCluster() != nParentCluster || from->getParentCluster() != nParentCluster)
			return true;
			
		e = n->edgeIterNext(ei);
	}
	
	return false;
}

void AnnotatedClusterAbstraction::openGLDraw()
{
	Map* map = this->getMap();
	graph* g1 = abstractions[1];
		
	GLdouble xx, yy, zz,rr;
	
	/* draw transitions */
	edge_iterator edgeIter; 
	edgeIter = g1->getEdgeIter();
	edge* e = g1->edgeIterNext(edgeIter);
	while(e)
	{
		node *n1, *n2; 		
		//double x1, x2, y1, y2;

		path* thepath = pathCache[e->getUniqueID()];
		if(thepath)
		{
			//node* efrom = abstractions[1]->getNode(e->getFrom()); 
			//node* eto = abstractions[1]->getNode(e->getTo());
			
			glLineWidth(1.0f);
			while(thepath->next)
			{
				n1 = thepath->n;
				n2 = thepath->next->n;

				glColor3f (0.7F, 0.5F, 0.5F);
				
				glBegin(GL_LINES);
				
				map->getOpenGLCoord(n1->getLabelL(kFirstData), n1->getLabelL(kFirstData+1), xx, yy, zz, rr);
				glVertex3f(xx, yy, zz-rr*0.5);
				map->getOpenGLCoord(n2->getLabelL(kFirstData), n2->getLabelL(kFirstData+1), xx, yy, zz, rr);
				glVertex3f(xx, yy, zz-rr*0.5);
				
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
		
		map->getOpenGLCoord(absn->getLabelL(kFirstData), absn->getLabelL(kFirstData+1), xx, yy, zz, rr);
		
		glColor3f (0.6F, 0.4F, 0.4F);
		glBegin(GL_QUADS);
		glVertex3f(xx-0.01, yy-0.01, zz-rr*0.5);
		glVertex3f(xx+0.01, yy-0.01, zz-rr*0.5);
		glVertex3f(xx+0.01, yy+0.01, zz-rr*0.5);
		glVertex3f(xx-0.01, yy+0.01, zz-rr*0.5);		
		glEnd();
		
		absn = g1->nodeIterNext(nodeIter);	
	}

	if(drawClusters)
	{
		glLineWidth(2.0f);
		for(unsigned int cindex = 0; cindex<clusters.size(); cindex++)
		{
			glColor3f (0.6F, 0.9F, 0.4F);
			AnnotatedCluster* ac = clusters[cindex];
			glBegin(GL_LINE_STRIP);
			map->getOpenGLCoord(ac->getHOrig(), ac->getVOrig(), xx, yy, zz, rr);
			glVertex3f(xx, yy, zz-rr*0.5);

			map->getOpenGLCoord(ac->getHOrig()+ac->getWidth()-1, ac->getVOrig(), xx, yy, zz, rr);
			glVertex3f(xx, yy, zz-rr*0.5);

			map->getOpenGLCoord(ac->getHOrig()+ac->getWidth()-1, ac->getVOrig()+ac->getHeight()-1, xx, yy, zz, rr);
			glVertex3f(xx, yy, zz-rr*0.5);

			map->getOpenGLCoord(ac->getHOrig(), ac->getVOrig()+ac->getHeight()-1, xx, yy, zz, rr);
			glVertex3f(xx, yy, zz-rr*0.5);

			map->getOpenGLCoord(ac->getHOrig(), ac->getVOrig(), xx, yy, zz, rr);
			glVertex3f(xx, yy, zz-rr*0.5);

			glEnd();
		}
	}

	glLineWidth(1.0f);
	
	if(drawClearance)
		AnnotatedMapAbstraction::openGLDraw();
}

