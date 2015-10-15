/*  EvoGraph
 *  Copyright (C) 2003
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *	Jianjun Hu		hujianju@msu.edu
 *	Homepage: www.egr.msu.edu/~hujianju
 *	Genetic Algorithm Research and Application Group
 *  Department of Computer Science
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 */


#ifndef _APP_H
#define _APP_H

#include <conio.h>
#include <vector>
#include "..\graphlib\UndirectedGraph.h"



class app_data: public UndirectedGraph
{
public:
	//APPCODE HJJ
	//Global information
	int writeHead;		  //used for generating new edge/vertex identifiers.
	int iniNumWriteHead;//initial modifiable site ID in the embryo.
	int maxX;						//maximum value of X coordinate 
	int maxY;						//maximum value of Y coordinate
	UndirectedGraph* pClientGraph; //Client distribution map of the wireless internet service
	int watchInterval;	//every watchInterval generations we save the evolved graph
	int nGraphOutputed;	//how many evolved graphs have been saved.
	double maxServiceRadius; //the maximum distance a wireless access point can cover
	//APPCODE END

	//functions	
	~app_data(){
	};

	app_data(){
		UndirectedGraph::UndirectedGraph(1);
			//global write head information
			//APPCODE
			writeHead =0;				
			iniNumWriteHead= 1;	
			maxX = 1000;				
			maxY =1000;
			watchInterval=5;
			nGraphOutputed=0;
			maxServiceRadius = 200;
	};
};

extern app_data ad;

/*extern globaldata g;*/
void embryo_graph();//set up initial bondgraph
extern app_data *get_appdata(void);

#endif
