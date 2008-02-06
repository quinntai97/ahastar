/*
 *  ExperimentManager.h
 *  hog
	
	Wrapper class around a related set of functionality for dealing with TestExperiment classes (ie. test cases).
	These are in turn used by the test fixtures to verify correct behaviour of search algorithms (AnnotatedAStar, AHA*)
 
 *  Created by Daniel Harabor on 18/01/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EXPERIMENTMANAGER_H
#define EXPERIMENTMANAGER_H

#include <ext/hash_map>

namespace ExpMgrUtil
{
	enum ExperimentKey
	{	
		/* pathable problems; experiments for large agents are supersets of small agents so we can avoid duplication by testing only large cases */
		kPathableMoveNorthLST, 
		kPathableMoveSouthLST,
		kPathableMoveEastLST,
		kPathableMoveWestLST,
		kPathableMoveSouthEastLST,
		kPathableMoveSouthWestLST,
		kPathableMoveNorthEastLST,
		kPathableMoveNorthWestLST,
		kPathableToyProblemLST, 

		/* not pathable; hard obstacle */
		kNotPathableStartIsHardObstacleLST,
		kNotPathableGoalIsHardObstacleLST,
		kNotPathableHardObstacleBlocksGoal,

		/* not pathable; soft obstacle */
		kNotPathableStartIsSoftObstacleLST,
		kNotPathableGoalIsSoftObstacleLST,
		kNotPathableSoftObstacleBlocksGoal,

		/* not pathable; invalid parameters */
		kNotPathableStartPositionOutOfBoundsLST,
		kNotPathableGoalPositionOutOfBoundsLST,
		kNotPathableStartPositionOverlapsMapEdgeLST,
		kNotPathableGoalPositionOverlapsMapEdgeLST,
		kNotPathableInvalidTerrain,
		kNotPathableAgentSizeLessThanMin,
		kNotPathableAgentSizeGreaterThanMax,
		
		kNoExperiment

	};

	class TestExperiment
	{
		public:
			TestExperiment(ExperimentKey, int, int, int, int, int, int, double, std::string, bool);
			ExperimentKey key;
			int startx;
			int starty;
			int goalx;
			int goaly;
			int terrain; // agent capabilities; ie. which terrain types are traversable
			int size; // agent size
			double distance; // actual distance between the start and goal
			std::string mapname;
			bool pathable; // does a solution exist for this problem?
	}; 
	
/*	struct TestExperiment
	{
		int startx;
		int starty;
		int goalx;
		int goaly;
		int terrain; // agent capabilities; ie. which terrain types are traversable
		int size; // agent size
		double distance; // actual distance between the start and goal
		char *mapname;
		bool pathable; // does a solution exist for this problem?
	};
	
	struct ExpCompare {
		bool operator()( const TestExperiment exp1, const TestExperiment exp2) {
			if(exp1.distance == exp2.distance)
				return true;
			return false;
		}
	}; 
*/
	
	typedef __gnu_cxx::hash_map<int, TestExperiment*> ExperimentCollection;
}

class ExperimentManager
{
	public:
		ExperimentManager();
		~ExperimentManager();
		ExpMgrUtil::TestExperiment* getExperiment(ExpMgrUtil::ExperimentKey);
		ExpMgrUtil::ExperimentKey getLastExperimentKey();
		
	private:
		void generateExperiments();
		
		ExpMgrUtil::ExperimentKey last;
		static ExpMgrUtil::ExperimentCollection* tests;
		static int refcount;

};



#endif