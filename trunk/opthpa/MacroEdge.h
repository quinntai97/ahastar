#ifndef MACROEDGE_H
#define MACROEDGE_H

/* MacroEdge.h
 *
 *  Wraps HOG's edge class. Provides a way to distinguish
 *  between edges added by HOG and edges added by OHA*.
 *
 *  @author: dharabor
 *  @created: 27/04/2010
 */
 

#include "graph.h"

class MacroEdge : public edge
{
	public:
		MacroEdge(unsigned int from, unsigned int to, double weight);
		MacroEdge(MacroEdge& e);
		~MacroEdge(); 
};

#endif

