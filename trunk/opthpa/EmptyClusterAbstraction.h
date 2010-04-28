/*	
 *	EmptyClusterAbstaction.h
 *
 *  Decomposes a map into non-regular clusters which are obstacle free. 
 *  This decomposition allows each cluster to be traversed optimally 
 *  by only expanding nodes along the perimeter of the cluster area 
 *  (and never any inside).
 *
 *  See [Harabor & Botea 2010]
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

		//virtual double h(node* from, node* to);
		int getNumMacro();

	private:
		void connectSG(node* absNode);
		void computeClearance(int** clearance);


};

#endif
