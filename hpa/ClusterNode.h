#ifndef CLUSTERNODE_H
#define CLUSTERNODE_H

// ClusterNode.h
//
// An extension the basic node class, ClusterNode objects
// can be associated with Cluster objects.
//
// @author: dharabor
// @created: 11/ 2008

#include "graph.h"
#include <ostream>

class Cluster;
class ClusterNode : public node
{
	public:
		ClusterNode(const char* name, Cluster*);
		ClusterNode(const char* name);
		ClusterNode(const ClusterNode* n);
		~ClusterNode(); 
		virtual graph_object* clone() const { return new ClusterNode(this); }
		virtual void reset();
		int getParentCluster() { return parentCluster; }
		void setParentCluster(Cluster* c) { parentCluster = c; }
		int getParentClusterId();

		void print(std::ostream& out);

	private:
		void init();
		Cluster* parentCluster; // bad, but convenient
};

#endif
