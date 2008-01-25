/*
 *  AnnotatedAStarTest.cpp
 *  hog
 *
 *  Created by Daniel Harabor on 14/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "AnnotatedMapAbstraction.h"
#include "AnnotatedAStar.h"
#include "AnnotatedAStarTest.h"
#include "AnnotatedAStarMock.h"
#include "TestConstants.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatedAStarTest );

void AnnotatedAStarTest::setUp()
{
	aastar = new AnnotatedAStar();
	ama = new AnnotatedMapAbstraction(new Map(maplocation.c_str()), new AnnotatedAStarMock());

}

//AnnotatedAStarTest::CreateSearchProblem

void AnnotatedAStarTest::tearDown()
{
	delete ama;
	delete aastar;
}

/*
NB: 
	Method generates pathfinding problems. different from test to test (which could be problematic).
	Inconsistencies between runs, failure to generate problems, no guarantees that all combinations of sizes/terrains are tested. 
	Using a fixed test dataset fixes some of these (if known good answers exist) but exhaustive coverage is also hard to do. 
	
	TODO: create test maps.
*/
void AnnotatedAStarTest::CheckPath()
{
	bool isok = false;
	graph *g = ama->getAbstractGraph(0);
	node *from, *to;
	path *p;
	int cnt=0;
	int maxrepeat=0;
	
	while(cnt<50) 
	{
		//try to generate a valid search problem; 50 tries max
		int agentsize, terrain;
		path *p;
		while(maxrepeat < 50)
		{
			agentsize = agentsizes[rand()%2];
			terrain = validterrains[rand()%3];
			
			from = g->getRandomNode();
			to = g->getRandomNode();
			
			/* check if the path is valid for the given agent size and terrain traversal capabilities */
			if((from->getTerrainType()&terrain) == terrain && (to->getTerrainType()&terrain) == terrain
				&& from->getClearance(from->getTerrainType()) >= agentsize && to->getClearance(to->getTerrainType()) >= agentsize)
				{
					AnnotatedAStar aastar;
					p = aastar.getPath(ama, from, to, terrain, agentsize);
					if(p) break;
				}

			maxrepeat++;
		}
		
		/* TEST1: did we generate a problem successfully? */
		CPPUNIT_ASSERT_MESSAGE("test map is rubbish. couldn't generate a pathable problem", p != 0);
				
		/* TEST2: each location along the solution path is OK for the specified capabilities */
		path* q;
		q = p;
		while(q->n)
		{
			node *n = q->n;
			CPPUNIT_ASSERT_MESSAGE("calculated path is invalid (invalid terrain)", n->getTerrainType() == terrain);
			CPPUNIT_ASSERT_MESSAGE("calculated path is invalid (insufficient clearance)", n->getClearance(terrain) >= agentsize);
		}
		
		delete p;
		cnt++;
	}

}