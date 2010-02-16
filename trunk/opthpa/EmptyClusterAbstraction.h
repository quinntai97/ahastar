/*	
 *	emptyClusterAbstaction.h
 *
 * 	Creates a spatial decomposition involving non-regular
 * 	clusters which are completely empty.
 * 	Uses a floodfill approach to create clusters.
 *
 * 	Because the clusters are empty we :w
 *
 *
 * 	@author: dharabor
 * 	@created: 10/02/2010
 */

#ifndef EMPTYCLUSTERABSTRACTION_H
#define EMPTYCLUSTERABSTRACTION_H

#include "HPAUtil.h"
#include "HPAClusterAbstraction.h"
#include "EmptyCluster.h"

class IHPAClusterFactory;
class INodeFactory;
class IEdgeFactory;
class Map;
class node;

class EmptyClusterAbstraction : public HPAClusterAbstraction
{
	#ifdef UNITTEST
		friend class EmptyClusterTest;
	#endif

	public:
		EmptyClusterAbstraction(Map* m, IHPAClusterFactory* cf, INodeFactory* nf, IEdgeFactory* ef) 
			throw(std::invalid_argument);
		virtual ~EmptyClusterAbstraction();

		virtual void buildClusters();
		virtual	EmptyCluster* clusterIterNext(cluster_iterator&) const;
		virtual EmptyCluster* getCluster(int cid);
		
		virtual void insertStartAndGoalNodesIntoAbstractGraph(node*, node*) 
			throw(std::invalid_argument);

		int manhattan(node* from, node* to);

	private:
		void connectSG(node* absNode);


};

#endif

