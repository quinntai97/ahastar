#include "EmptyClusterInsertionPolicy.h"

#include "EmptyCluster.h"
#include "EmptyClusterAbstraction.h"
#include "MacroNode.h"

EmptyClusterInsertionPolicy::EmptyClusterInsertionPolicy(
	EmptyClusterAbstraction* _map) : InsertionPolicy(), map(_map)
{
}

EmptyClusterInsertionPolicy::~EmptyClusterInsertionPolicy()
{
}


node* 
EmptyClusterInsertionPolicy::insert(node* _n) 
	throw(std::invalid_argument)
{
	ClusterNode* n = dynamic_cast<ClusterNode*>(_n);
	if(n == 0)
	return 0;

	if(n->getParentClusterId() == -1)
	return 0;

	if(map->getAllowDiagonals())
		connectSG(n);
	else
		cardinalConnectSG(n);
}

void 
EmptyClusterInsertionPolicy::remove(node* n)
	throw(std::runtime_error)
{
}

void 
EmptyClusterAbstraction::connectSG(MacroNode* absNode)
{
	EmptyCluster* nCluster = map->getCluster(absNode->getParentClusterId());
	graph* absg = map->getAbstractGraph(1);

	const int x = absNode->getLabelL(kFirstData);
	const int y = absNode->getLabelL(kFirstData+1);
	if(getVerbose())
			std::cout << "inserting node ("<<x<<", "<<y<<") "
				"into abstract graph"<<std::endl;

	if(getVerbose())
			std::cout << "connecting to nodes along the "
				"top of the cluster"<<std::endl;

	// connect to nodes along top perimeter of cluster
	int maxDiagSteps = y - nCluster->getVOrigin();
	int minx = (x-maxDiagSteps)<nCluster->getHOrigin()?
		nCluster->getHOrigin():(x-maxDiagSteps);
	int maxx = (x+maxDiagSteps)>(nCluster->getHOrigin()+nCluster->getWidth()-1)?
			(nCluster->getHOrigin()+nCluster->getWidth()-1):(x+maxDiagSteps);

	MacroNode* absNeighbour = 0;
	int ny = nCluster->getVOrigin();
	int nx = minx+1;
	for( ; nx <= maxx-1; nx++)
	{
		assert(map->getNodeFromMap(nx, ny));
		absNeighbour = static_cast<MacroNode*>(
						absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
		if(absNeighbour && &*absNeighbour != &*absNode)
				connectSGToNeighbour(absNode, absNeighbour);
	}

	// try to connect to nearest entrance along the top border not in the fan 
	// of absNode
	if(nCluster->getHeight() > 1 && nCluster->getWidth() > 1)
	{
		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInRow(minx-1, ny, map, false);
		else
			absNeighbour = nCluster->nextNodeInRow(minx, ny, map, false);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);

		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInRow(maxx+1, ny, map, true);
		else
			absNeighbour = nCluster->nextNodeInRow(maxx, ny, map, true);

		if(absNeighbour)
				connectSGToNeighbour(absNode, absNeighbour);
	}

	if(getVerbose())
			std::cout << "connecting to nodes along the bottom of "
				"the cluster"<<std::endl;

	// connect to nodes along the bottom perimeter of cluster
	maxDiagSteps = (nCluster->getVOrigin()+nCluster->getHeight()-1) - y;
	minx = (x-maxDiagSteps)<nCluster->getHOrigin()?
		nCluster->getHOrigin():(x-maxDiagSteps);
	maxx = (x+maxDiagSteps)>(nCluster->getHOrigin()+nCluster->getWidth()-1)?
			(nCluster->getHOrigin()+nCluster->getWidth()-1):(x+maxDiagSteps);

	nx = minx+1;
	ny = nCluster->getVOrigin()+nCluster->getHeight()-1;
	for( ; nx <= maxx-1; nx++)
	{
		assert(map->getNodeFromMap(nx, ny));
		absNeighbour = static_cast<MacroNode*>(
						absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
		if(absNeighbour && &*absNeighbour != &*absNode)
			connectSGToNeighbour(absNode, absNeighbour);
	}

	// try to connect to nearest entrance along the bottom border not in the 
	// fan of absNode
	if(nCluster->getHeight() > 1 && nCluster->getWidth() > 1)
	{
		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInRow(minx-1, ny, map, false);
		else
			absNeighbour = nCluster->nextNodeInRow(minx, ny, map, false);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);

		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInRow(maxx+1, ny, map, true);
		else
			absNeighbour = nCluster->nextNodeInRow(maxx, ny, map, true);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);

	}
	
	if(getVerbose())
			std::cout << "connecting to nodes along the left of the "
			"cluster"<<std::endl;

	// connect to nodes along the left perimeter of cluster
	maxDiagSteps = x - nCluster->getHOrigin();       
	int miny = (y-maxDiagSteps)<nCluster->getVOrigin()?
		nCluster->getVOrigin():(y-maxDiagSteps);
	int maxy = (y+maxDiagSteps)>(nCluster->getVOrigin()+nCluster->getHeight()-1)?
			(nCluster->getVOrigin()+nCluster->getHeight()-1):(y+maxDiagSteps);

	nx = nCluster->getHOrigin();
	ny = miny+1;
	for( ; ny <= maxy-1; ny++)
	{
		assert(map->getNodeFromMap(nx, ny));
		absNeighbour = static_cast<MacroNode*>(
						absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
		if(absNeighbour && &*absNeighbour != &*absNode)
			connectSGToNeighbour(absNode, absNeighbour);
	}

	// connect to nearest nodes outside fan area (if necessary)
	if(nCluster->getHeight() > 1 && nCluster->getWidth() > 1)
	{
		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInColumn(nx, miny-1, map, false);
		else
			absNeighbour = nCluster->nextNodeInColumn(nx, miny, map, false);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);

		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInColumn(nx, maxy+1, map, true);
		else
			absNeighbour = nCluster->nextNodeInColumn(nx, maxy, map, true);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);
	}

	if(getVerbose())
			std::cout << "connecting to nodes along the right of the "
			"cluster"<<std::endl;

	// connect to nodes along the right perimeter of cluster
	maxDiagSteps = (nCluster->getHOrigin()+nCluster->getWidth()-1) - x;
	miny = (y-maxDiagSteps)<nCluster->getVOrigin()?
		nCluster->getVOrigin():(y-maxDiagSteps);
	maxy = (y+maxDiagSteps)>(nCluster->getVOrigin()+nCluster->getHeight()-1)?
			(nCluster->getVOrigin()+nCluster->getHeight()-1):(y+maxDiagSteps);

	nx = nCluster->getHOrigin()+nCluster->getWidth()-1;
	ny = miny+1;
	for( ; ny <= maxy-1; ny++)
	{
		assert(map->getNodeFromMap(nx, ny));
		absNeighbour = static_cast<MacroNode*>(
						absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
		if(absNeighbour && &*absNeighbour != &*absNode)
			connectSGToNeighbour(absNode, absNeighbour);
	}

	// connect to nearest nodes outside fan area (if necessary)
	if(nCluster->getHeight() > 1 && nCluster->getWidth() > 1)
	{
		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInColumn(nx, miny-1, map, false);
		else
			absNeighbour = nCluster->nextNodeInColumn(nx, miny, map, false);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);

		if(maxDiagSteps == 0)
			absNeighbour = nCluster->nextNodeInColumn(nx, maxy+1, map, true);
		else
			absNeighbour = nCluster->nextNodeInColumn(nx, maxy, map, true);

		if(absNeighbour)
			connectSGToNeighbour(absNode, absNeighbour);
	}
}

void 
EmptyClusterAbstraction::cardinalConnectSG(MacroNode* absNode)
{
        graph* absg = map->getAbstractGraph(1);
        EmptyCluster* nodeCluster = map->getCluster(absNode->getParentClusterId());

        int x = absNode->getLabelL(kFirstData);
        int y = absNode->getLabelL(kFirstData+1);

        if(getVerbose())
                std::cout << "inserting node ("<<x<<", "<<y<<") into abstract "
				"graph"<<std::endl;

        // connect to neighbour above
        int ny = nodeCluster->getVOrigin();
        int nx = x;
        MacroNode* absNeighbour = static_cast<MacroNode*>(
                        absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
        if(absNeighbour == 0 || absNeighbour->getNum() == absNode->getNum())
        {
			absNeighbour = nodeCluster->nextNodeInRow(nx+1, ny, map, true);
			if(absNeighbour)
				connectSGToNeighbour(absNode, absNeighbour);

			absNeighbour =  nodeCluster->nextNodeInRow(nx-1, ny, map, false);
			if(absNeighbour)
				connectSGToNeighbour(absNode, absNeighbour);
        }
        else
                connectSGToNeighbour(absNode, absNeighbour);

        // connect to neighbour below
        ny = nodeCluster->getVOrigin()+nodeCluster->getHeight()-1;
        nx = x;
        absNeighbour = static_cast<MacroNode*>(
                        absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
        if(absNeighbour == 0 || absNeighbour->getNum() == absNode->getNum())
        {
                absNeighbour = nodeCluster->nextNodeInRow(nx+1, ny, map, true);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);

                absNeighbour =  nodeCluster->nextNodeInRow(nx-1, ny, map, false);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);
        }
        else
                connectSGToNeighbour(absNode, absNeighbour);
                

        // connect to neighbour to the left
        ny = y; 
        nx = nodeCluster->getHOrigin();
        absNeighbour = static_cast<MacroNode*>(
                        absg->getNode(
						map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
        if(absNeighbour == 0 || absNeighbour->getNum() == absNode->getNum())
        {
                absNeighbour = nodeCluster->nextNodeInColumn(nx, ny+1, map, true);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);

                absNeighbour =  nodeCluster->nextNodeInColumn(nx, ny-1, map, false);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);
        }
        else
                connectSGToNeighbour(absNode, absNeighbour);


        // connect to neighbour to the right
        ny = y; 
        nx = nodeCluster->getHOrigin()+nodeCluster->getWidth()-1;
        absNeighbour = static_cast<MacroNode*>(
                        absg->getNode(map->getNodeFromMap(nx, ny)->getLabelL(kParent)));
        if(absNeighbour == 0 || absNeighbour->getNum() == absNode->getNum())
        {
                absNeighbour = nodeCluster->nextNodeInColumn(nx, ny+1, map, true);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);

                absNeighbour =  nodeCluster->nextNodeInColumn(nx, ny-1, map, false);
                if(absNeighbour)
                        connectSGToNeighbour(absNode, absNeighbour);
        }
        else
                connectSGToNeighbour(absNode, absNeighbour);
}

void 
EmptyClusterAbstraction::connectSGToNeighbour(
		MacroNode* absNode, MacroNode* absNeighbour)
{
//	assert(map->getNodeFromMap(absNode->getLabelL(kFirstData),
//			absNode->getLabelL(kFirstData+1))->getLabelL(kParent) != -1);
//	assert(map->getNodeFromMap(absNeighbour->getLabelL(kFirstData), 
//			absNeighbour->getLabelL(kFirstData+1))->getLabelL(kParent) != -1);

	if(absNode->getUniqueID() == absNeighbour->getUniqueID())
		return;

	graph* absg = map->getAbstractGraph(1);

	assert((int)absNeighbour->getNum() < absg->getNumNodes());
	assert((int)absNode->getNum() < absg->getNumNodes());

	MacroEdge* e = new MacroEdge(absNode->getNum(), absNeighbour->getNum(), 
			h(absNode, absNeighbour));
	absg->addEdge(e);

	if(getVerbose())
	{
		std::cout << "absNeighbour ("<<absNeighbour->getLabelL(kFirstData)<<", "
			<<absNeighbour->getLabelL(kFirstData+1)<<") weight: "
			<<e->getWeight() <<std::endl;
	}
}
	
