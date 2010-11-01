#ifndef CLUSTER_H
#define CLUSTER_H

// Cluster.h
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

class Cluster
{
	public:
		Cluster(graphAbstraction* map);
		virtual ~Cluster();

		// implement these
		virtual void buildCluster() = 0;
		virtual void buildEntrances() = 0;
		virtual void connectParent(node*) 
			throw(std::invalid_argument) = 0;

		// getters and setters
		inline int getId() { return clusterId; }
		inline void setId(const int newid) { clusterId = newid; }
		inline int getNumParents() { return parents.size(); }
		inline int getNumNodes() { return nodes.size(); }
		inline nodeTable* getNodes() { return &nodes; }
		inline nodeTable* getParents() { return &parents; }
		inline void setVerbose(bool _v) { verbose = _v; }
		inline bool getVerbose() { return verbose; }

		// debugging and display functions
		void printParents();
		virtual void print(std::ostream& out);
		virtual void openGLDraw() { }

	protected:
		// methods for managing nodes associated with the cluster
		void addNode(node* n) 
			throw(std::invalid_argument);
		void addParent(node*) 
			throw(std::invalid_argument);
		void removeParent(int nodeId);
		void addTransition(node* from, node* to, double edgeweight)
			throw(std::invalid_argument);

		int clusterId;
		bool verbose;
		nodeTable nodes;
		nodeTable parents; // transition points
		graphAbstraction* map;

	private:
		static unsigned int uniqueClusterIdCnt;
};

#endif

