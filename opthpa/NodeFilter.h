#ifndef NODEFILTER_H
#define NODEFILTER_H

class NodeFilter 
{

	public:
		NodeFilter() {}
		virtual ~NodeFilter() { }

		bool filter(node* n) = 0;
};

#endif

