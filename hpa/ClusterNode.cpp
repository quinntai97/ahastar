#include "ClusterNode.h"

#include "Cluster.h"
#include "constants.h"

ClusterNode::ClusterNode(const ClusterNode* n) : node(n)
{
	this->parentCluster = n->parentCluster;
}

ClusterNode::ClusterNode(const char* name, Cluster* c) : node(name)
{
	this->parentCluster = c; 
	init();
}

ClusterNode::ClusterNode(const char* name) : node(name) 
{ 
	parentCluster = 0;
	init();
}

ClusterNode::~ClusterNode()
{
}

void ClusterNode::init()
{
	this->setLabelL(kParent, -1);
}

void ClusterNode::print(std::ostream& out)
{
	out << "node @ ("<<getLabelL(kFirstData)<<",";
	out << getLabelL(kFirstData+1)<<") cluster: ";
	if(parentCluster != 0)
		out << parentCluster->getId();
	else
		out << "NULL";
	out << std::endl;
}

void ClusterNode::reset()
{
	this->markEdge(0);
	this->backpointer = 0;
}

void ClusterNode::getParentClusterId()
{
	if(parentCluster)
		return parentCluster->getId();
	return -1;
}
