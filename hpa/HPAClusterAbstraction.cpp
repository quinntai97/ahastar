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

#include "searchAlgorithm.h"
#include "map.h"
#include "NodeFactory.h"
#include "EdgeFactory.h"
#include "path.h"


// TODO: throw an exception if anything of the parameter pointers are NULL.
// TODO add search algorithm parameter
HPAClusterAbstraction::HPAClusterAbstraction(Map* m, searchAlgorithm* _alg, IHPAClusterFactory* _cf, INodeFactory* _nf, IEdgeFactory* _ef, unsigned int _clustersize) 
	: mapAbstraction(m), alg(_alg), cf(_cf), nf(_nf), ef(_ef), clustersize(_clustersize)
{	
	abstractions.push_back(getMapGraph(this->getMap(), nf, ef)); 
	abstractions.push_back(new graph());	
	startid = goalid = -1;
	drawClusters=true;
	
	nodesExpanded = nodesTouched = peakMemory = 0;
	searchTime = 0;
}

HPAClusterAbstraction::~HPAClusterAbstraction()
{
	delete ef;
	delete cf;
	delete nf;
	delete alg;
	
	for(HPAUtil::pathTable::iterator it = pathCache.begin(); it != pathCache.end(); it++)
		delete (*it).second;
	pathCache.erase(pathCache.begin(), pathCache.end());

	for(HPAUtil::clusterTable::iterator it = clusters.begin(); it != clusters.end(); it++)
		delete (*it).second;
	clusters.erase(clusters.begin(), clusters.end());
}

HPACluster* HPAClusterAbstraction::getCluster(int cid)
{		
	return clusters[cid];
}

void HPAClusterAbstraction::addCluster(HPACluster* cluster) 
{ 
	clusters[cluster->getClusterId()] = cluster;
} 

void HPAClusterAbstraction::buildClusters()
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
				
			HPACluster *cluster = cf->createCluster(x,y,cwidth,cheight);
			addCluster( cluster ); // nb: also assigns a new id to cluster
			cluster->addNodesToCluster(this);
		}
}

void HPAClusterAbstraction::buildEntrances()
{
}

/* paths are cached in the direction of the edge (from, to) */
void HPAClusterAbstraction::addPathToCache(edge* e, path* p)
{
	if(e == NULL || p == NULL)
		return;
	
	pathCache[e->getUniqueID()] = p;
}

/* Cache for known paths. Stores one path for each abstract edge. */
path* HPAClusterAbstraction::getPathFromCache(edge* e)
{
	if(e == NULL)
		return 0;

	path* p = pathCache[e->getUniqueID()];
	return p;	
}

HPACluster* HPAClusterAbstraction::clusterIterNext(cluster_iterator &iter) const
{
	if (iter != clusters.end())
	{
		HPACluster* c = (*iter).second;
		iter++;
		return c;
	}
  return 0;
}

void HPAClusterAbstraction::openGLDraw()
{
/*	Map* map = this->getMap();
	graph* g1 = abstractions[1];
		
	GLdouble xx, yy, zz,rr;
	
	//draw transitions 
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
		for(int cindex = 0; cindex<clusters.size(); cindex++)
		{
			glColor3f (0.6F, 0.9F, 0.4F);
			HPACluster* ac = clusters[cindex];
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
		HPAMapAbstraction::openGLDraw();
	*/
}