/*
 * $Id: sample.cpp,v 1.23 2006/11/01 23:33:56 nathanst Exp $
 *
 *  sample.cpp
 *  hog
 *
 *  Created by Nathan Sturtevant on 5/31/05.
 *  Copyright 2005 Nathan Sturtevant, University of Alberta. All rights reserved.
 *
 * This file is part of HOG.
 *
 * HOG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * HOG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with HOG; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "common.h"
#include "sample.h"
#include "aStar.h"
#include "praStar.h"
#include "searchUnit.h"
#include "sharedAMapGroup.h"
#include "mapCliqueAbstraction.h"
#include "mapQuadTreeAbstraction.h"
#include "radiusAbstraction.h"
#include "mapFlatAbstraction.h"
#include "AnnotatedMapAbstraction.h"
#include "AnnotatedClusterAbstraction.h"
#include "AnnotatedClusterFactory.h"
#include "AnnotatedAStar.h"
#include "AnnotatedHierarchicalAStar.h"
#include "clusterAbstraction.h"
#include "ScenarioManager.h"
#include <cstdlib>

bool mouseTracking;
int px1, py1, px2, py2;
int absType = 3;
AHAScenarioManager scenariomgr;
AHAExperiment* nextExperiment;
int expnum=0;
bool runAHA=false;

/**
 * This function is called each time a unitSimulation is deallocated to
 * allow any necessary stat processing beforehand
 */

void processStats(statCollection *stat)
{
/*	std::stringstream ss;
	ss << "/Users/dharabor/src/hog/experiments/results";
	FILE *f = fopen(ss.str().c_str(), "a+");
	statValue val;
	std::vector<const char *> *owners = stat->getOwners();
	std::cout << "e\tid\ts\tc\tne\tnt\thne\thnt\tmem\thmem\ts&ct\ttst\thst\tsl\thsl\thdm\tdm"<<std::endl;
	for(int i=0;i<owners->size();i++)
	{
		const char* unitname = owners->at(i);
		stat->lookupStat("experimentId", unitname, val);
		int expId = val.lval;
		
		//cout <<val.lval<<",\t";
		//std::cout << unitname << ",\t";
		stat->lookupStat("agentSize", unitname, val);
		int agentSize=val.lval;
		//cout <<val.lval<<",\t";
		stat->lookupStat("agentCapabilities", unitname, val);
		int agentCaps = val.lval;
		
		//cout <<val.lval<<",\t";
		stat->lookupStat("totalNodesExpanded", unitname, val);
		int tne = val.lval;
		//cout <<val.lval<<",\t";
		stat->lookupStat("totalNodesTouched", unitname, val);
		int tnt = val.lval;
		
		//cout <<val.lval<<",\t";
		stat->lookupStat("hNodesExpanded", unitname, val);
		int hne = val.lval;
		//cout <<val.lval<<",\t";
		stat->lookupStat("hNodesTouched", unitname, val);
		int hnt = val.lval;
		
		//cout <<val.lval<<",\t";
		stat->lookupStat("peakMemoryUsage", unitname, val);
		int pmu = val.lval;
		
		//cout <<val.lval<<",\t";
		stat->lookupStat("hMemoryUsage", unitname, val);
		int hmu = val.lval;
		
		//cout <<val.lval<<",\t";
		stat->lookupStat("setupAndCleanupTime", unitname, val);
		double stc = val.fval;
		
		//cout <<setprecision(3) << val.fval<<",\t";
		stat->lookupStat("totalSearchTime", unitname, val);
		double tst = val.fval;
		
		//cout <<setprecision(3) << val.fval<<",\t";
		stat->lookupStat("hSearchTime", unitname, val);
		double hst = val.fval;
		
		//cout <<setprecision(3) << val.fval<<",\t";
		stat->lookupStat("solutionLength", unitname, val);
		int sl = val.lval;
		
		//cout << val.lval<<",\t";
		stat->lookupStat("hSolutionLength", unitname, val);
		int hsl = val.lval;
		
		//cout << val.lval<<",\t";
		stat->lookupStat("hDistance", unitname, val);
		double hdist = val.fval;
		
		//cout <<setprecision(3) << val.fval<<"\t";
		stat->lookupStat("distanceMoved", unitname, val);
		double dm = val.fval;
				
		//cout <<setprecision(3) << val.fval<<std::endl;
		fprintf(f, "%s,\t %i,\t %i,\t %i,\t %i,\t %i,\t %i,\t %i,\t %i,\t %i,\t %.2f,\t %.2f,\t %.2f,\t %i,\t %i,\t %.2f,\t %.2f,\t %s \n", 
					unitname, expId, agentSize, agentCaps, tne, tnt, hne, hnt, pmu, hmu, stc, tst, hst, sl, hsl, hdist, dm, mapname.c_str());
		

	}

	//fprintf(f,"\n");
	fflush(f);
	fclose(f);
	//stat->printStatsTable();
	
	*/
}

/**
 * This function is used to allocate the unit simulated that you want to run.
 * Any parameters or other experimental setup can be done at this time.
 */
void createSimulation(unitSimulation * &unitSim)
{
//	Map* map = new Map("/Users/dharabor/src/ahastar/tests/testmaps/clustertest.map");
//	Map* map = new Map("/Users/dharabor/src/ahastar/maps/local/pacman.map");
//	Map* map = new Map("/Users/dharabor/src/ahastar/maps/local/adaptive-depth-10.map");
	Map* map = new Map(gDefaultMap);
	int CLUSTERSIZE=5;

	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(map, new AnnotatedAStar(), CLUSTERSIZE);
	AnnotatedClusterFactory* acf = new AnnotatedClusterFactory();
	aca->buildClusters(acf);
	delete acf;
	aca->buildEntrances();
	aca->setDrawClusters(true);
	aca->setDrawClearance(true);
	graph* absg = aca->getAbstractGraph(1);
	std::cout << "\n map: "<<gDefaultMap<< "absnodes: "<<absg->getNumNodes()<<" absedges: "<<absg->getNumEdges();

	edge_iterator ei = absg->getEdgeIter();
/*	edge* e = absg->edgeIterNext(ei);
	while(e)
	{
		node* f = absg->getNode(e->getFrom());
		node* t = absg->getNode(e->getTo());
		std::cout << "\n edge connects "<<f->getLabelL(kFirstData)<<","<<f->getLabelL(kFirstData+1)<< " and "<<t->getLabelL(kFirstData)<<","<<t->getLabelL(kFirstData+1);
		std::cout <<"(weight: "<<e->getWeight()<<" caps: "<<e->getCapability() << " clearance: "<<e->getClearance(e->getCapability())<<")";
		e = absg->edgeIterNext(ei);
	}
*/

	unitSim = new unitSimulation(aca);	
	unitSim->setCanCrossDiagonally(true);
	
	unitSim->setNextExperimentPtr(&runNextExperiment);
	runNextExperiment(unitSim);
}

/**
 * This function is called once after each [time-step and frame draw]
 * You can do any high level processing, drawing, etc in this function
 */
void frameCallback(unitSimulation *us)
{
	char tempStr[255];
	sprintf(tempStr, "Simulation time elapsed: %1.2f, Display Time: %1.2f",
					us->getSimulationTime(), us->getDisplayTime());
	submitTextToBuffer(tempStr);
	
	if ((mouseTracking) && (px1 != -1) && (px2 != -1) && (py1 != -1) && (py2 != -1))
	{
		glColor3f(1.0, 1.0, 1.0);
		GLdouble x1, y1, z1, rad;
		glBegin(GL_LINES);
		us->getMap()->getOpenGLCoord(px1, py1, x1, y1, z1, rad);
		glVertex3f(x1, y1, z1-rad);
		us->getMap()->getOpenGLCoord(px2, py2, x1, y1, z1, rad);
		glVertex3f(x1, y1, z1-rad);
		glEnd();
	}
}

/**
 * Allows you to install any keyboard handlers needed for program interaction.
 */
void initializeHandlers()
{
	installKeyboardHandler(myDisplayHandler, "Toggle Abstraction", "Toggle display of the ith level of the abstraction", kAnyModifier, '0', '9');
	installKeyboardHandler(myDisplayHandler, "Cycle Abs. Display", "Cycle which group abstraction is drawn", kNoModifier, '\t');
	installKeyboardHandler(myDisplayHandler, "Pause Simulation", "Pause simulation execution.", kNoModifier, 'p');
	installKeyboardHandler(myDisplayHandler, "Step Simulation", "If the simulation is paused, step forward .1 sec.", kNoModifier, 'o');
	installKeyboardHandler(myDisplayHandler, "Step History", "If the simulation is paused, step forward .1 sec in history", kAnyModifier, '}');
	installKeyboardHandler(myDisplayHandler, "Step History", "If the simulation is paused, step back .1 sec in history", kAnyModifier, '{');
	installKeyboardHandler(myDisplayHandler, "Step Abs Type", "Increase abstraction type", kAnyModifier, ']');
	installKeyboardHandler(myDisplayHandler, "Step Abs Type", "Decrease abstraction type", kAnyModifier, '[');

	installKeyboardHandler(myPathfindingKeyHandler, "Mapbuilding Unit", "Deploy unit that paths to a target, building a map as it travels", kNoModifier, 'd');
	installKeyboardHandler(myRandomUnitKeyHandler, "Add A* Unit", "Deploys a simple a* unit", kNoModifier, 'a');
	installKeyboardHandler(myRandomUnitKeyHandler, "Add simple Unit", "Deploys a randomly moving unit", kShiftDown, 'a');
	installKeyboardHandler(myRandomUnitKeyHandler, "Add simple Unit", "Deploys a right-hand-rule unit", kControlDown, 1);

	installCommandLineHandler(myCLHandler, "-map", "-map filename", "Selects the default map to be loaded.");
	installCommandLineHandler(myScenarioGeneratorCLHandler, "-genscenarios", "-genscenarios [.map filename] [number of scenarios] [clearance]", "Generates a scenario; a set of path problems on a given map");
	installCommandLineHandler(myExecuteScenarioCLHandler, "-scenario", "-scenario filename", "Execute all experiments in a given .scenario file");
	
	installMouseClickHandler(myClickHandler);
}

int myCLHandler(char *argument[], int maxNumArgs)
{
	if (maxNumArgs <= 1)
		return 0;
	strncpy(gDefaultMap, argument[1], 1024);
	return 2;
}

int myScenarioGeneratorCLHandler(char *argument[], int maxNumArgs)
{
	if (maxNumArgs < 4)
		return 0;	
	std::string map(argument[1]);
	std::string genscen(argument[0]);
	std::cout << "call: "<<genscen<<" "<<map<<" "<<argument[2] <<" "<<argument[3];
	
	AHAScenarioManager scenariomgr;
	int numScenarios = atoi(argument[2]);
	int minAgentSize = atoi(argument[3]);

	AnnotatedAStar* aastar = new AnnotatedAStar();
	AnnotatedMapAbstraction ama(new Map(map.c_str()), aastar);
	
	scenariomgr.generateExperiments(&ama, numScenarios, minAgentSize);
	std::cout << "\ngenerated: "<<scenariomgr.getNumExperiments()<< " experiments";

	string outfile = map + ".scenario"; 
	scenariomgr.writeScenarioFile(outfile.c_str());
	exit(-1);
}

int myExecuteScenarioCLHandler(char *argument[], int maxNumArgs)
{	
	if(maxNumArgs < 2)
		return 0;
		
	std::string infile(argument[1]);
	scenariomgr.loadScenarioFile(infile.c_str());	
	strncpy(gDefaultMap, scenariomgr.getNthExperiment(0)->getMapName(), 1024);
	
	std::cout << "\n executing scenario file at " << gDefaultMap<<". hold on to your hats, this could get hairy!";
	return 2;
}

void myDisplayHandler(unitSimulation *unitSim, tKeyboardModifier mod, char key)
{
	switch (key)
	{
		case '\t': unitSim->cyclemapAbstractionDisplay(); break;
		case 'p': unitSim->setSimulationPaused(!unitSim->getSimulationPaused()); break;
		case 'o':
			if (unitSim->getSimulationPaused())
			{
				unitSim->setSimulationPaused(false);
				unitSim->advanceTime(.1);
				unitSim->setSimulationPaused(true);
			}
			break;
		case ']': absType = (absType+1)%3; break;
		case '[': absType = (absType+4)%3; break;
		case '{': unitSim->setSimulationPaused(true); unitSim->offsetDisplayTime(-0.5); break;
		case '}': unitSim->offsetDisplayTime(0.5); break;
		default:
			if (unitSim->getMapAbstractionDisplay())
				unitSim->getMapAbstractionDisplay()->toggleDrawAbstraction(((mod == kControlDown)?10:0)+(key-'0'));
			break;
	}
}

void myRandomUnitKeyHandler(unitSimulation *unitSim, tKeyboardModifier mod, char)
{
	unitSim->clearAllUnits();

	/*
	Map* map = new Map("/Users/dharabor/src/ahastar/maps/local/demo.map");///annotatedcluster.map");
	AnnotatedClusterAbstraction* aca = new AnnotatedClusterAbstraction(map, new AnnotatedAStar(), 5);
	AnnotatedClusterFactory* acf = new AnnotatedClusterFactory();
	aca->buildClusters(acf);
	aca->buildEntrances();
	delete aca;
	delete acf;
	*/
//	AnnotatedMapAbstraction* ama = new AnnotatedMapAbstraction(map, new AnnotatedAStar());
//	delete ama;

	int x1, y1, x2, y2;
	unit *u, *targ;
	AbstractAnnotatedAStar* aastar;
	
	std::cout << "\n absnodes: "<<unitSim->getMapAbstraction()->getAbstractGraph(1)->getNumNodes()<< " edges: "<<unitSim->getMapAbstraction()->getAbstractGraph(1)->getNumEdges();
	std::cout << " cachesize: "<<((AnnotatedClusterAbstraction*)unitSim->getMapAbstraction())->getPathCacheSize();

	int clearance=2;
	int capability=kGround;

	unitSim->getRandomLocation(x1, y1);
	unitSim->getRandomLocation(x2, y2);
	
//	x2 = 1; y2 = 5;
//	x1=16; y1=8;
	std::cout << "\n deploying unit to "<<x2<<","<<y2<<" with target at "<<x1<<","<<y1;
	
	unitSim->addUnit(targ = new unit(x1, y1));

	switch (mod)
	{
		case kShiftDown: 
			aastar = new AnnotatedHierarchicalAStar();
			aastar->setClearance(clearance);
			aastar->setCapability(capability);
			unitSim->addUnit(u=new searchUnit(x2, y2, targ, aastar)); 
			break;
		default:
			aastar = new AnnotatedAStar();
			aastar->setClearance(clearance);
			aastar->setCapability(capability);
			unitSim->addUnit(u=new searchUnit(x2, y2, targ, aastar)); 
			break;
	}
	u->setSpeed(0.25);
//	u->setSpeed(0.000001);
	//unitSim->setmapAbstractionDisplay(1);
}

void myPathfindingKeyHandler(unitSimulation *unitSim, tKeyboardModifier mod, char)
{	
	for (int x = 0; x < ((mod==kShiftDown)?(50):(1)); x++)
	{
		if (unitSim->getUnitGroup(1) == 0)
		{
			unitSim->addUnitGroup(new sharedAMapGroup(unitSim));
			unitSim->setmapAbstractionDisplay(2);
		}
		int xx1, yy1, xx2, yy2;
		unitSim->getRandomLocation(xx1, yy1);
		unitSim->getRandomLocation(xx2, yy2);
		
		unit *u, *u2 = new unit(xx2, yy2, 0);
		
		praStar *pra = new praStar(); pra->setPartialPathLimit(4);
		//aStar *pra = new aStar();
		
		unitSim->addUnit(u2);
		u = new searchUnit(xx1, yy1, u2, pra);
		// just set the group of the unit, and it will share a map with those
		// units.
		unitSim->getUnitGroup(1)->addUnit(u);
		unitSim->addUnit(u);
		u->setSpeed(0.5); // time to go 1 distance						
	}
}

bool myClickHandler(unitSimulation *unitSim, int, int, point3d loc, tButtonType button, tMouseEventType mType)
{
	mouseTracking = false;
	if (button == kRightButton)
	{
		switch (mType)
		{
			case kMouseDown:
				unitSim->getMap()->getPointFromCoordinate(loc, px1, py1);
				//printf("Mouse down at (%d, %d)\n", px1, py1);
				break;
			case kMouseDrag:
				mouseTracking = true;
				unitSim->getMap()->getPointFromCoordinate(loc, px2, py2);
				//printf("Mouse tracking at (%d, %d)\n", px2, py2);
				break;
			case kMouseUp:
			{
				if ((px1 == -1) || (px2 == -1))
					break;
				unitSim->getMap()->getPointFromCoordinate(loc, px2, py2);
				//printf("Mouse up at (%d, %d)\n", px2, py2);
				unit *u, *u2 = new unit(px2, py2, 0);
				//praStar *pra = new praStar(); pra->setPartialPathLimit(4);
				aStar *pra = new aStar();
				unitSim->addUnit(u2);
				u = new searchUnit(px1, py1, u2, pra);
				unitSim->addUnit(u);
				u->setSpeed(0.5); // time to go 1 distance						
			}
			break;
		}
		return true;
	}
	return false;
}

void runNextExperiment(unitSimulation *unitSim)
{
	if(expnum == scenariomgr.getNumExperiments()) 
	{
		processStats(unitSim->getStats());
		delete unitSim;
		exit(0);
	}

	AHAExperiment* nextExperiment = dynamic_cast<AHAExperiment*>(scenariomgr.getNthExperiment(expnum));
	
	searchUnit* nextUnit;
	unit* nextTarget = new unit(nextExperiment->getGoalX(), nextExperiment->getGoalY());
	if(runAHA)
	{
		AnnotatedHierarchicalAStar* ahastar = new AnnotatedHierarchicalAStar();
		ahastar->setCapability(nextExperiment->getCapability());
		ahastar->setClearance(nextExperiment->getAgentsize());			
		nextUnit = new searchUnit(nextExperiment->getStartX(), nextExperiment->getStartY(), nextTarget, ahastar); 
		nextUnit->setColor(1,0.98,0.8);
		nextTarget->setColor(1,0.98,0.8);
		expnum++;
		runAHA=false;
	}
	else
	{
		AnnotatedAStar* aastar = new AnnotatedAStar();
		aastar->setCapability(nextExperiment->getCapability());
		aastar->setClearance(nextExperiment->getAgentsize());			
		nextUnit = new searchUnit(nextExperiment->getStartX(), nextExperiment->getStartY(), nextTarget, aastar); 
		nextUnit->setColor(1,1,0);
		nextTarget->setColor(1,1,0);
		runAHA=true;
	}
	
//	nextUnit->setSpeed(0.000001);
	nextUnit->setSpeed(0.25);
	unitSim->clearAllUnits();
	unitSim->addUnit(nextTarget);
	unitSim->addUnit(nextUnit);
	
}