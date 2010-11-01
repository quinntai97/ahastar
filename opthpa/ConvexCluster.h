#ifndef CONVEXCLUSTER_H
#define CONVEXCLUSTER_H

// ConvexCluster.h
//
// ConvexCluster generalises EmptyCluster by allowing clusters which can have a
// non-rectangular shape (but which must respect grid convexity).
//
// A cluster C respects grid convexity if for all nodes x and y:
//		d(x, y) = h(x, y)
//
//	Where x and y are nodes belonging to C, d(x, y) is the length of the optimal
//	path connecting them and h(x, y) is a value returned by an admissible heuristic 
//	function (usually Manhattan Distance or Octile Distance).
//
//	@author: dharabor
//	@created: 28/10/2010


class ConvexCluster : public Cluster
{
	public:
		ConvexCluster();
		virtual ~ConvexCluster();

		virtual void buildCluster();
		virtual void buildEntrances();
		virtual void connectParent(node*) 
			throw(std::invalid_argument);

};

#endif
