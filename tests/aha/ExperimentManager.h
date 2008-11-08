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

class node;

namespace ExpMgrUtil
{

	enum ExperimentKey
	{	
		/* experiments testing single step agent movements */
		kPathableMoveNorthLST, 
		kPathableMoveSouthLST,
		kPathableMoveEastLST,
		kPathableMoveWestLST,
		kPathableMoveSouthEastLST,
		kPathableMoveSouthWestLST,
		kPathableMoveNorthEastLST,
		kPathableMoveNorthWestLST,
		kNotPathableMoveToHardObstacleLST,
		kNotPathableMoveToSoftObstacleLST,
		kNotPathableMoveToNonAdjacentLocationLST,
		

		/* longer problems, pathable */
		kPathableToyProblemLST, 

		/* not pathable; hard obstacle */
		kNotPathableStartIsHardObstacleLST,
		kNotPathableGoalIsHardObstacleLST,
		kNotPathableHardObstacleBlocksGoal,

		/* not pathable; soft obstacle */
		kNotPathableStartIsSoftObstacleLST,
		kNotPathableGoalIsSoftObstacleLST,
		kNotPathableSoftObstacleBlocksGoal,

		/* not pathable; agent too big */
		kNotPathableStartClearanceTooSmallLST,
		kNotPathableGoalClearanceTooSmallLST,

		/* not pathable; invalid parameters */
		kNotPathableStartAndGoalIdenticalLST,
		kNotPathableInvalidCapability, 
		kNotPathableAgentSizeLessThanMin,
		
		kNoExperiment,

		kNotPathableWhenCorridorIsRestrictedToParentCluster,
		kPathableWithinCorridor

	};

	class TestExperiment
	{
		public:
			TestExperiment(ExperimentKey, int, int, int, int, int, int, int, int, double, std::string, bool);
			~TestExperiment();
			ExperimentKey key;
			int startx;
			int starty;
			int goalx;
			int goaly;
			int sterrain; 
			int gterrain;
			int caps; // agent capabilities; ie. which terrain types are traversable
			int size; // agent size
			double distance; // actual distance between the start and goal
			std::string mapname;
			bool pathable; // does a solution exist for this problem?
			
			node* getStartNode();
			node* getGoalNode();
			
		private:
			node *snode, *gnode;
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