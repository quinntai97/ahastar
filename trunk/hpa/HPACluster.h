/*
 *  HPACluster.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPACLUSTER_H
#define HPACLUSTER_H

#include "HPAUtil.h"
#include <stdexcept>

class node;
class HPAClusterAbstraction;

class HPACluster
{
	public:
		HPACluster(const int x, const int y, const int _width, const int _height);

		virtual void addParent(node *, HPAClusterAbstraction*);
		virtual void addNodesToCluster(HPAClusterAbstraction*) throw(std::invalid_argument);
				
		int getVOrigin() { return starty; }
		int getHOrigin() { return startx; }
		int getWidth() { return width; }
		int getHeight() { return height; }
		int getId() { return clusterId; }
		int getClusterId() { return clusterId; }
		void setClusterId(const int newid) { clusterId = newid; }

	protected:
		void addNode(node* mynode) throw(std::invalid_argument);
	
	private:	
		int clusterId;
		int startx, starty, width, height;
		HPAUtil::nodeTable nodes;
		
		static unsigned int uniqueClusterIdCnt;
};

#endif