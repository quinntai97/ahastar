#ifndef RECTANGULARROOMFACTORY_H 
#define RECTANGULARROOMFACTORY_H

#include "IClusterFactory.h"
#include "RectangularRoom.h"

class RectangularRoomFactory : public IClusterFactory 
{
	public:
		RectangularRoomFactory();
		virtual ~RectangularRoomFactory();
		virtual AbstractCluster* createCluster(int xpos, int ypos,
				GenericClusterAbstraction* map);
		RectangularRoom* createCluster(int x, int y,
				EmptyClusterAbstraction* map, bool pr, bool bfr);
};

#endif

