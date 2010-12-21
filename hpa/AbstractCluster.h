#ifndef ABSTRACTCLUSTER_H
#define ABSTRACTCLUSTER_H

// AbstractCluster.h
//
// A generic base class for implementing different kinds of clusters.
//
// A few methods have to be implemented for a concrete implementation:
// 	- buildCluster: assigns nodes to the cluster.
// 	- buildEntrances: identifies and creates transitions to adjacent clusters. 
// 	- connectParent: creates intra-edges that connect a node with the set of
// 					 abstract nodes associated with the cluster. 
//
// @author: dharabor
// @created: 28/10/2010

#include "ClusterNode.h"
#include "HPAUtil.h"

#include <stdexcept>
#include <map>

class GenericClusterAbstraction;
class AbstractCluster
{
	public:
		AbstractCluster(const int x, const int y, 
				GenericClusterAbstraction* map);
		virtual ~AbstractCluster();

		// implement these
		virtual void buildCluster() = 0;
		virtual void buildEntrances() = 0;
		virtual void connectParent(node*) 
			throw(std::invalid_argument) = 0;
		virtual void openGLDraw() = 0;
	
		// methods for managing nodes associated with the cluster
		virtual void addNode(node* n) 
			throw(std::invalid_argument);
		virtual void addParent(node*) 
			throw(std::invalid_argument);
		virtual void removeParent(int nodeId);
		virtual void addTransition(node* from, node* to, double edgeweight)
			throw(std::invalid_argument);

		// getters and setters
		inline int getId() { return clusterId; }
		inline void setId(const int newid) { clusterId = newid; }
		inline int getNumParents() { return parents.size(); }
		inline int getNumNodes() { return nodes.size(); }
		inline HPAUtil::nodeTable* getNodes() { return &nodes; }
		inline HPAUtil::nodeTable* getParents() { return &parents; }
		inline void setVerbose(bool _v) { verbose = _v; }
		inline bool getVerbose() { return verbose; }
		inline void setVOrigin(int starty_) { starty = starty_; }
		inline void setHOrigin(int startx_) { startx = startx_; }
		inline int getVOrigin() { return starty; }
		inline int getHOrigin() { return startx; }

		// debugging and display functions
		void printParents();
		virtual void print(std::ostream& out);

		GenericClusterAbstraction* getMap() const { return map; }

	protected:
		int clusterId;
		bool verbose;
		HPAUtil::nodeTable nodes;
		HPAUtil::nodeTable parents; // transition points
		GenericClusterAbstraction* map;

	private:
		static unsigned int uniqueClusterIdCnt;
		int startx, starty;
};

#endif

