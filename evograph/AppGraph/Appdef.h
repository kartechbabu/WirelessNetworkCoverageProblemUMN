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

	#ifndef _APPDEF_H
	#define _APPDEF_H


	//classified by its operation objects

	#define OP_ROOT 0 // root function f_Tree

	#define OP_EDGE 1 // functions operating on edge

	#define OP_NODE 2 // functions operating on node

	#define OP_REAL 3 // functions operating on real number and return real number

	#define OP_BOOL 4 // Boolean funcitons 

	#define OP_IGNORE -1// any functions  This will get ignored anyway */

	//max arguments of a GP function
	#define MAXARGS    6 //APPCODE

	//	typedef  double DATATYPE;
	#define DATATYPE double


	/* Define NUMTYPES to be the number of types passed and returned in your
		 GP program.  You should distinguish your types from 0 to NUMTYPES-1 */
	#define NUMTYPES   5 /* Void (what move etc. do) and bool */



#endif
