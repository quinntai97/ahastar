#include "ConvexCluster.h"
#include "EmptyClusterAbstraction.h"

ConvexCluster::ConvexCluster(int x, int y, EmptyClusterAbstraction* map) 
	: EmptyCluster(x, y, map)
{

}

ConvexCluster::~ConvexCluster()
{
}

void ConvexCluster::buildCluster()
{
}

void ConvexCluster::openGLDraw()
{
}
