/*
 *  HPAUtil.h
 *  hog
 *
 *  Created by dharabor on 11/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HPAUTIL_H
#define HPAUTIL_H

#include <ext/hash_map>

class node;
class path;
class HPACluster;

namespace HPAUtil
{
	typedef __gnu_cxx::hash_map<int, path*> pathTable;
	typedef __gnu_cxx::hash_map<int, node*> nodeTable;
	typedef __gnu_cxx::hash_map<int, HPACluster*> clusterTable;
}

#endif