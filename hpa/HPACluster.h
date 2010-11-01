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

#include <map>
#include <iostream>
#include <stdexcept>

class node;
class searchAlgorithm;
class HPAClusterAbstraction;
class AbstractClusterAStar;

typedef std::map<int, node*> nodeTable;
const int MAX_SINGLE_TRANSITION_ENTRANCE_SIZE = 6;

class HPACluster : public Cluster
{
	#ifdef UNITTEST
		friend class HPAClusterAbstractionTest; 
		friend class HPAClusterTest; 
	#endif
	
	public:
		HPACluster(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* alg) throw(std::invalid_argument);
		HPACluster(const int x, const int y, const int _width, const int _height) throw(std::invalid_argument);
		virtual ~HPACluster();

		virtual void buildEntrances(HPAClusterAbstraction* hpamap) 
			throw(std::invalid_argument);
		virtual void addParent(node *, HPAClusterAbstraction*) 
			throw(std::invalid_argument);
		virtual void removeParent(int nodeId);
		virtual void addNodesToCluster(HPAClusterAbstraction*) 
			throw(std::invalid_argument);
		
		inline	void setSearchAlgorithm(AbstractClusterAStar* _alg) { alg = _alg; }
		inline AbstractClusterAStar* getSearchAlgorithm() { return alg; }

		inline void setVOrigin(int starty_) { starty = starty_; }
		inline void setHOrigin(int startx_) { startx = startx_; }
		inline int getVOrigin() { return starty; }
		inline int getHOrigin() { return startx; }
		inline int getWidth() { return width; }
		inline void setWidth(int _width) { width = _width; }
		inline int getHeight() { return height; }
		inline void setHeight(int _height) { height = _height; }

		inline void setAllowDiagonals(bool value) { allowDiagonals = value; }
		inline bool getAllowDiagonals() { return allowDiagonals; }

		virtual void openGLDraw() { }

	protected:
		virtual void addNode(node* mynode) throw(std::invalid_argument);
		virtual void addTransitionPoint(node* from, node* to, 
				HPAClusterAbstraction* hpamap, double edgeweight = 0);
		virtual void connectParent(node*, HPAClusterAbstraction*);

		virtual void buildHorizontalEntrances(HPAClusterAbstraction* hpamap);
		virtual void buildVerticalEntrances(HPAClusterAbstraction* hpamap);
		virtual void buildDiagonalEntrances(HPAClusterAbstraction* hpamap);

		virtual void processHorizontalEntrance(HPAClusterAbstraction* hpamap,
						int x, int y, int length);
		virtual void processVerticalEntrance(HPAClusterAbstraction* hpamap,
						int x, int y, int length);

		virtual	int findVerticalEntranceLength(int x, int y, 
				HPAClusterAbstraction* hpamap);
		virtual	int findHorizontalEntranceLength(int x, int y, 
				HPAClusterAbstraction* hpamap);
	
	private:
		void init(const int x, const int y, const int _width, const int _height, AbstractClusterAStar* _alg) throw(std::invalid_argument);
		void insertNodeIntoAbstractGraph(node* n);

		int startx, starty, width, height;
		AbstractClusterAStar* alg;
		bool allowDiagonals;
		
};

#endif
