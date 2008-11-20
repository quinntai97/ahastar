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
#include "CapabilityUnit.h"
#include "statCollection.h"
#include <cstdlib>
#include <sstream>

bool mouseTracking;
int px1, py1, px2, py2;
int absType = 3;
AHAScenarioManager scenariomgr;
AHAExperiment* nextExperiment;
int expnum=0;
bool runAHA=false;
bool scenario=false;
bool hog_gui=true;
bool highquality=true;
int CLUSTERSIZE=10;

/**
 * This function is called each time a unitSimulation is deallocated to
 * allow any necessary stat processing beforehand
 */

void processStats(statCollection *stat)
{
	if(stat->getNumStats() == 0)
		return;
	
	std::string unitname;
	if(runAHA) // aha is next; we just finished aa* experiment
		unitname = "AAStar";		
	else
		unitname = "AHAStar";
	
	processStats(stat, unitname.c_str());
	stat->clearAllStats();
}

void processStats(statCollection* stat, const char* unitname)
{
	//stat->printStatsTable();
	std::ostringstream ss;
	ss << "results";
	if(highquality)
		ss << "_highquality";
	else
		ss << "_lowquality";

	ss << "_csize"<<CLUSTERSIZE;
	
	statValue val;
	
	int ne, nt, pm, absne, absnt, abspm, insne, insnt, inspm;
	double st, absst, insst, pathdist;
	int expId = expnum;
	if(strcmp(unitname, "AHAStar") == 0 && hog_gui)
		expId--;

	ss << "_"<<unitname;
	//std::cout << "exp: "<<expId<<" ";
	//std::cout << unitname;
	FILE *f = fopen(ss.str().c_str(), "a+");

	ne = nt = pm = absne = absnt = abspm = insne = insnt = inspm = 0;
	st = absst = insst = pathdist = 0;
	bool exists;

	fprintf(f, "%i,\t", expId);

	fprintf(f, "%s,\t", unitname);

	exists = stat->lookupStat("agentSize", unitname, val);
	assert(exists);
	int agentSize=val.lval;
	fprintf(f, "%i,\t", agentSize);

	
	exists = stat->lookupStat("agentCapability", unitname, val);
	assert(exists);
	int agentCaps = val.lval;
	fprintf(f, "%i,\t", agentCaps);
	
	exists = stat->lookupStat("nodesExpanded", unitname, val);
	assert(exists);
	ne = val.lval;
	fprintf(f, "%i,\t", ne);

	
	exists = stat->lookupStat("nodesTouched", unitname, val);
	assert(exists);
	nt = val.lval;
	fprintf(f, "%i,\t", nt);

	exists = stat->lookupStat("peakMemory", unitname, val);
	assert(exists);
	pm = val.lval;
	fprintf(f, "%i,\t", pm);

	exists = stat->lookupStat("searchTime", unitname, val);
	assert(exists);
	st = val.fval;
	fprintf(f, "%.8f,\t", st);
	
	if(strcmp(unitname, "AHAStar") == 0)
	{
		exists = stat->lookupStat("insNodesExpanded", unitname, val);
		assert(exists);
		insne = val.lval;
		fprintf(f, "%i,\t", insne);

		exists = stat->lookupStat("insNodesTouched", unitname, val);
		assert(exists);
		insnt = val.lval;
		fprintf(f, "%i,\t", insnt);
		
		exists = stat->lookupStat("insPeakMemory", unitname, val);
		assert(exists);
		inspm = val.lval;
		fprintf(f, "%i,\t", inspm);
				
		exists = stat->lookupStat("insSearchTime", unitname, val);
		assert(exists);
		insst = val.fval;
		fprintf(f, "%.8f,\t", insst);

	}
	
	//stat->lookupStat("distanceMoved", unitname, val);
	//std::cout << "dist: "<<val.fval<<std::endl;
	if(!hog_gui)
	{
		exists = stat->lookupStat("distanceMoved", unitname, val);
		assert(exists);
		pathdist = val.fval;
	}
	fprintf(f, "%.3f,\t", pathdist);	
	
	fprintf(f, "%s\n", gDefaultMap);	

	fflush(f);
	fclose(f);
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
	//map->scale(100, 100);

	AnnotatedClusterAbstraction* aca;
	if(highquality)
		aca = new AnnotatedClusterAbstraction(map, new AnnotatedAStar(), CLUSTERSIZE);
	else
		aca = new AnnotatedClusterAbstraction(map, new AnnotatedAStar(), CLUSTERSIZE, ACAUtil::kLowQualityAbstraction);
				
	AnnotatedClusterFactory* acf = new AnnotatedClusterFactory();
	aca->buildClusters(acf);
	delete acf;
	aca->buildEntrances();
	aca->setDrawClusters(true);
	//aca->setDrawClearance(true);
	graph* absg = aca->getAbstractGraph(1);
	graph* g = aca->getAbstractGraph(0);
	
	std::ostringstream ss;
	ss << "results_graphsize";
	if(highquality)
		ss << "_highquality";
	else
		ss << "_lowquality";

	ss << "_csize"<<CLUSTERSIZE;
	
	FILE *f = fopen(ss.str().c_str(), "a+");

	fprintf(f, "%i,\t%i,\t", g->getNumNodes(), g->getNumEdges());
	fprintf(f, "%i,\t%i,\t", absg->getNumNodes(), absg->getNumEdges());
	fprintf(f, "%s\n", gDefaultMap);
	fflush(f);
	fclose(f);
	
	std::cout << "\noriginal map: nodes: "<<g->getNumNodes()<<" edges: "<<g->getNumEdges();
	std::cout << "\n map: "<<gDefaultMap<< "absnodes: "<<absg->getNumNodes()<<" absedges: "<<absg->getNumEdges();
	edge_iterator ei = absg->getEdgeIter();
	 
	// debugging
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
	
	if(hog_gui)
	{
		unitSim = new unitSimulation(aca);	
		unitSim->setCanCrossDiagonally(true);
		if(scenario)
		{
			unitSim->setNextExperimentPtr(&runNextExperiment);
			runNextExperiment(unitSim);
		}
	}
	else
	{
		gogoGadgetNOGUIScenario(aca);
	}
}

void gogoGadgetNOGUIScenario(AnnotatedClusterAbstraction* aca)
{
	AnnotatedAStar aastar;
	AnnotatedHierarchicalAStar ahastar;
	statCollection stats;
	
	for(int i=0; i< scenariomgr.getNumExperiments(); i++)
	{
		expnum = i;
		nextExperiment = (AHAExperiment*)scenariomgr.getNthExperiment(i);
		aastar.setCapability(nextExperiment->getCapability());
		aastar.setClearance(nextExperiment->getAgentsize());
		node* from = aca->getNodeFromMap(nextExperiment->getStartX(), nextExperiment->getStartY());
		node* to = aca->getNodeFromMap(nextExperiment->getGoalX(), nextExperiment->getGoalY());
		
		path* p = aastar.getPath(aca, from, to);
		double distanceTravelled = aca->distance(p);
		stats.addStat("distanceMoved", aastar.getName(), distanceTravelled);
		aastar.logFinalStats(&stats);
		processStats(&stats, aastar.getName());
		stats.clearAllStats();
		delete p;
		
		if(aastar.getClearance() == 2)
		{
			aastar.setClearance(1);
			p = aastar.getPath(aca, from, to);
			distanceTravelled = aca->distance(p);
			stats.addStat("distanceMoved", aastar.getName(), distanceTravelled);
			aastar.logFinalStats(&stats);
			processStats(&stats, aastar.getName());
			stats.clearAllStats();
			delete p;
		}

		
		ahastar.setCapability(nextExperiment->getCapability());
		ahastar.setClearance(nextExperiment->getAgentsize());
		p = ahastar.getPath(aca, from, to);
		distanceTravelled = aca->distance(p);
		stats.addStat("distanceMoved", ahastar.getName(), distanceTravelled);
		ahastar.logFinalStats(&stats);
		processStats(&stats);
		stats.clearAllStats();
		delete p;

		if(ahastar.getClearance() == 2)
		{
			ahastar.setClearance(1);
			p = ahastar.getPath(aca, from, to);
			distanceTravelled = aca->distance(p);
			stats.addStat("distanceMoved", ahastar.getName(), distanceTravelled);
			ahastar.logFinalStats(&stats);
			processStats(&stats, ahastar.getName());
			stats.clearAllStats();
			delete p;
		}
	}
	
	delete aca;
	exit(0);
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
	installCommandLineHandler(myGUICLHandler, "-gui", "-gui enable/disable", "Run the app without a pretty interface (used in conjunction with -scenario). Defaults to enable if not specified or if a non-valid argument is given ");	
	installCommandLineHandler(myQualityCLHandler, "-quality", "-quality high/low", "Type of cluster abstraction to create (high results in better quality solutions but costs more memory & takes longer to set up). Default = high");	
	installCommandLineHandler(myClustersizeCLHandler, "-clustersize", "-clustersize [num]", "Size of clusters to split up map into. Larger clusters are faster to create but less accurate. Default = 10.");		
	
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
	if(maxNumArgs < 1)
		return 0;
	
	std::cout << "\n -scenario call: "<<argument[1];
	std::string infile(argument[1]);
	scenariomgr.loadScenarioFile(infile.c_str());	
	strncpy(gDefaultMap, scenariomgr.getNthExperiment(0)->getMapName(), 1024);
	
	scenario=true;
	return 2;
}

int myGUICLHandler(char *argument[], int maxNumArgs)
{
	std::string value(argument[1]);
	if(strcmp(argument[1], "disable") == 0)
		hog_gui=false;

	return 2;
}

int myQualityCLHandler(char *argument[], int maxNumArgs)
{
	std::string value(argument[1]);
	if(strcmp(argument[1], "high") == 0)
	{
		highquality=true;
		return 2;
	}
	else 
		if(strcmp(argument[1], "low") == 0)
		{
			highquality=false;
			return 2;
		}
		
	return 0;
}

int myClustersizeCLHandler(char *argument[], int maxNumArgs)
{
	std::string value(argument[1]);
	int val = atoi(value.c_str());
	if(val>0)
	{
		CLUSTERSIZE=val;
		return 2;
	}

	return 0;
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
			unitSim->addUnit(u=new CapabilityUnit(x2, y2, targ, aastar)); 
			u->setColor(1,0.98,0.8);
			targ->setColor(1,0.98,0.8);
			break;
		default:
			aastar = new AnnotatedAStar();
			aastar->setClearance(clearance);
			aastar->setCapability(capability);
			unitSim->addUnit(u=new CapabilityUnit(x2, y2, targ, aastar)); 
			u->setColor(1,1,0);
			targ->setColor(1,1,0);
			break;
	}
	u->setSpeed(0.12);
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

	processStats(unitSim->getStats());
	AHAExperiment* nextExperiment = dynamic_cast<AHAExperiment*>(scenariomgr.getNthExperiment(expnum));
	
	searchUnit* nextUnit;
	unit* nextTarget = new unit(nextExperiment->getGoalX(), nextExperiment->getGoalY());
	if(runAHA)
	{
		AnnotatedHierarchicalAStar* ahastar = new AnnotatedHierarchicalAStar();
		ahastar->setCapability(nextExperiment->getCapability());
		ahastar->setClearance(nextExperiment->getAgentsize());			
		nextUnit = new CapabilityUnit(nextExperiment->getStartX(), nextExperiment->getStartY(), nextTarget, ahastar); 
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
		nextUnit = new CapabilityUnit(nextExperiment->getStartX(), nextExperiment->getStartY(), nextTarget, aastar); 
		nextUnit->setColor(1,1,0);
		nextTarget->setColor(1,1,0);
		runAHA=true;
	}
	
	nextUnit->setSpeed(0.05);
	unitSim->clearAllUnits();
	unitSim->addUnit(nextTarget);
	unitSim->addUnit(nextUnit);
	
}

void runSimulationNoGUI()
{
	std::cout << "\nok, no gui";
}
