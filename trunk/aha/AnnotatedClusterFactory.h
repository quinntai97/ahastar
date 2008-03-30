/*
 *  AnnotatedClusterFactoryInterface.h
 *  hog
 *
 *  Created by Daniel Harabor on 29/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANNOTATEDCLUSTERFACTORY_H
#define ANNOTATEDCLUSTERFACTORY_H

class AnnotatedCluster;
class AnnotatedClusterAbstraction;

class IAnnotatedClusterFactory
{
	public:
		virtual AnnotatedCluster* createCluster(int xpos, int ypos, int width, int height) = 0;		
};

class AnnotatedClusterFactory : public IAnnotatedClusterFactory
{
	public:
		virtual AnnotatedCluster* createCluster(int xpos, int ypos, int width, int height);
	
	private:
		AnnotatedClusterAbstraction* aca;
};

#endif