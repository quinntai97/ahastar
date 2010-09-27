#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <string>

class Heuristic;
class node;
class path;
class graph;

class DebugUtility
{
	public:
		DebugUtility(graph* g, Heuristic* h);
		virtual ~DebugUtility();
	
		void debugClosedNode(node* c, node* n, double c_to_n_cost, node* goal);
		void printNode(std::string msg, node* n, node* goal=0);
		void printPath(path* p);

	private:
		graph* g;
		Heuristic* heuristic;
};

#endif
