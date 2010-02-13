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
		
		inline	void setSearchAlgorithm(AbstractClusterAStar* _alg) { alg = _alg; }
		inline AbstractClusterAStar* getSearchAlgorithm() { return alg; }
		inline int getVOrigin() { return starty; }
		inline int getHOrigin() { return startx; }
		inline int getWidth() { return width; }
		inline void setWidth(int _width) { width = _width; }
		inline int getHeight() { return height; }
		inline void setHeight(int _height) { height = _height; }
		inline int getId() { return clusterId; }
		inline int getClusterId() { return clusterId; }
		inline void setClusterId(const int newid) { clusterId = newid; }
		inline int getNumParents() { return parents.size(); }
		inline int getNumNodes() { return nodes.size(); }
		inline nodeTable* getNodes() { return &nodes; }
		inline nodeTable* getParents() { return &parents; }
		
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
