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
const int MAX_SINGLE_TRANSITION_ENTRANCE_SIZE = 6;

class HPACluster
{
	#ifdef UNITTEST
		friend class HPAClusterAbstractionTest; 
		friend class HPAClusterTest; 
	#endif
	
	public:
		HPACluster(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* alg) throw(std::invalid_argument);
		HPACluster(const int x, const int y, const int _width, const int _height) throw(std::invalid_argument);
		virtual ~HPACluster();

		virtual void buildEntrances(HPAClusterAbstraction* hpamap) throw(std::invalid_argument);
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
		int getNumParents() { return parents.size(); }
		int getNumNodes() { return nodes.size(); }
		nodeTable* getNodes() { return &nodes; }
		nodeTable* getParents() { return &parents; }
		
		void printParents();

	protected:
		virtual void addNode(node* mynode) throw(std::invalid_argument);
		virtual void addTransitionPoint(node* from, node* to, HPAClusterAbstraction* hpamap);
	
	private:
		void init(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* _alg) throw(std::invalid_argument);
		void insertNodeIntoAbstractGraph(node* n);
		void connectParent(node*, HPAClusterAbstraction*);
		virtual void buildHorizontalEntrances(HPAClusterAbstraction* hpamap);
		virtual void buildVerticalEntrances(HPAClusterAbstraction* hpamap);

		int findVerticalEntranceLength(int x, int y, HPAClusterAbstraction* hpamap);
		int findHorizontalEntranceLength(int x, int y, HPAClusterAbstraction* hpamap);
		
			
		int clusterId;
		int startx, starty, width, height;
		nodeTable nodes;
		nodeTable parents; // transition points
		AbstractClusterAStar* alg;
		
		static unsigned int uniqueClusterIdCnt;
};

#endif