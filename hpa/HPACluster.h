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

#include <stdexcept>
#include <map>

class node;
class searchAlgorithm;
class HPAClusterAbstraction;
class AbstractClusterAStar;

typedef std::map<int, node*> nodeTable;
class HPACluster
{
	#ifdef UNITTEST
		friend class HPAClusterAbstractionTest; 
		friend class HPAClusterTest; 
	#endif
	
	public:
		HPACluster(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* alg);
		HPACluster(const int x, const int y, const int _width, const int _height);
		virtual ~HPACluster();

		virtual void buildEntrances(HPAClusterAbstraction*);
		virtual void addParent(node *, HPAClusterAbstraction*) throw(std::invalid_argument);
		virtual void removeParent(int nodeId);
		virtual void addNodesToCluster(HPAClusterAbstraction*) throw(std::invalid_argument);
		
		void setSearchAlgorithm(AbstractClusterAStar* _alg) { alg = _alg; }
		AbstractClusterAStar* getSearchAlgorithm() { return alg; }
		int getVOrigin() { return starty; }
		int getHOrigin() { return startx; }
		int getWidth() { return width; }
		int getHeight() { return height; }
		int getId() { return clusterId; }
		int getClusterId() { return clusterId; }
		void setClusterId(const int newid) { clusterId = newid; }
		int getNumParents() { return nodes.size(); }
		nodeTable* getNodes() { return &nodes; }
//		node* getParent(int parentId) { return nodes[parentId]; }

	protected:
		void addNode(node* mynode) throw(std::invalid_argument);
	
	private:
		void init(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* _alg);
		void insertParent(node*, HPAClusterAbstraction*);
			
		int clusterId;
		int startx, starty, width, height;
		nodeTable nodes;
		nodeTable parents; // transition points
		AbstractClusterAStar* alg;
		
		static unsigned int uniqueClusterIdCnt;
};

#endif