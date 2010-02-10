#include "EmptyCluster.h"
#include "HPAClusterAbstraction.h"

EmptyCluster::EmptyCluster(const int x, const int y)
	: HPACluster(x, y, 0, 0);
{

}

EmptyCluster::~EmptyCluster()
{

}

EmptyCluster::addNodesToCluster(HPAClusterAbstraction* aMap)
{
	extend(aMap);

	// add nodes along left border
	int x = this->getHOrigin();
	for(int y=this->getVOrigin(); y<this->getVOrigin()+this->getWidth(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}

	// add nodes along right border
	int x = this->getHOrigin()+this->getWidth()-1;
	for(int y=this->getVOrigin(); y<this->getVOrigin()+this->getHeight(); y++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
	
	// add nodes along top border
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
	
	// add nodes along bottom border
	int y = this->getVOrigin()+this->getHeight()-1;
	for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(n)
			addNode(n);
	}
}

EmptyCluster::extend(HPAClusterAbstraction* aMap)
{
	Map* themap = aMap->getMap();
	int hsize=0;
	int y = this->getVOrigin();
	for(int x=this->getHOrigin(); x<themap->getMapWidth(); x++)
	{
		node* n = aMap->getNodeFromMap(x,y);
		if(!n)
			break;
		hsize++;
	}
	
	this->setWidth(hsize);
	int vsize=0;
	for( ; y<themap->getMapHeight(); y++)
	{
		bool rowok=true;
		for(int x=this->getHOrigin(); x<this->getHOrigin()+this->getWidth(); x++)
		{
			node* n = aMap->getNodeFromMap(x,y);
			if(!n)
			{
				rowok=false;
			}
		}
		if(!rowok)
			break;
		vsize++;
	}
}
