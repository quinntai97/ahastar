/*
 *  TestHelper.h
 *  hog
 *
 *  Created by Daniel Harabor on 8/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TESTHELPER_H
#define TESTHELPER_H

#include "map.h"
#include "AHAConstants.h"
#include <string>

using namespace std;

/* consts etc */
const string maplocation = "/Users/dharabor/src/hog/maps/local/demo.map"; // testmap
const string tslocation = "test.scenario"; // test scenario file

/* helper methods */
string* getMessage(stringstream *ss);

#endif
