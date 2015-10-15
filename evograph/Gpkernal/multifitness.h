/*  lil-gp Genetic Programming System, version 1.0, 11 July 1995
 *  Copyright (C) 1995  Michigan State University
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
 *  Douglas Zongker       (zongker@isl.cps.msu.edu)
 *  Dr. Bill Punch        (punch@isl.cps.msu.edu)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	Implementing multi-fitness function for multipopulation
 *	Jianjun								(hujianju@cse.msu.edu)
 *
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *  
 */



#ifndef __GPENHANCE_H__
#define __GPENHANCE_H__

#include "lilgp.h"


//Used to implement multiple fitness function for different sub-populations
struct CMultiFitness{
	CMultiFitness(){ 
		app_eval_fitness = NULL;
		index = 0;
	};
	CMultiFitness(void (*fitness)(individual *ind), int i){ 
		app_eval_fitness = fitness;
		index = i;
	};
	operator =(CMultiFitness func);

	~CMultiFitness(){};
public:
	void (*app_eval_fitness)(individual *ind);//which fitness function
	int index;//which subpopulation
};














#endif //__GPENHANCE_H__