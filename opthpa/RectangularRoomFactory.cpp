#include "RectangularRoomFactory.h"
#include "RectangularRoom.h"
#include "EmptyClusterAbstraction.h"

RectangularRoomFactory::RectangularRoomFactory()
{
}

RectangularRoomFactory::~RectangularRoomFactory()
{
}

AbstractCluster* 
RectangularRoomFactory::createCluster(int x, int y, 
	GenericClusterAbstraction* map_)
{
	EmptyClusterAbstraction *map = dynamic_cast<EmptyClusterAbstraction*>(map_);
	if(map == 0)
		throw std::invalid_argument("EmptyClusterFactory: new cluster "
				"requires a map abstraction of type EmptyClusterAbstraction");

	return createCluster(x, y, map, false, false);
}

RectangularRoom*
RectangularRoomFactory::createCluster(int x, int y,
	EmptyClusterAbstraction* map, bool pr, bool bfr)
{
	return new RectangularRoom(x, y, map, pr, bfr);
}
