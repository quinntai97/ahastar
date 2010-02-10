#ifndef EMPTYCLUSTER_H
#define EMPTYCLUSTER_H

#include "HPACluster.h"

class HPAClusterAbstraction;

class EmptyCluster : public HPACluster
{
	public:
		EmptyCluster(const int x, const int y) throw(std::invalid_argument);
		virtual ~EmptyCluster();

		virtual addNodesToCluster(HPAClusterAbstraction* hpamap);

	protected:
		virtual void extend();
};

#endif

