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


#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "..\GPKernal\lilgp.h"

//Evolving graph----------------------------------------Operator Type
DATATYPE f_Tree ( int tree, farg *args, int arity );	//OP_ROOT
DATATYPE f_cutV(int tree, farg *args, int arity);			//OP_NODE
DATATYPE f_attachV(int tree, farg *args, int arity);	//OP_NODE
DATATYPE f_splitVL(int tree, farg *args, int arity);		//OP_NODE
DATATYPE f_endV ( int tree, farg *args, int arity );	//OP_NODE

DATATYPE f_endE ( int tree, farg *args, int arity );	//OP_EDGE
DATATYPE f_insertV(int tree, farg *args, int arity);	//OP_EDGE
DATATYPE f_cutE(int tree, farg *args, int arity);			//OP_EDGE
DATATYPE f_parallelE(int tree, farg *args, int arity);//OP_EDGE
DATATYPE f_splitV(int tree, farg *args, int arity);//OP_EDGE
DATATYPE f_loop_start(int tree, farg *args, int arity);
DATATYPE f_loop_end(int tree, farg *args, int arity);

																											
/*
DATATYPE f_ins_Selfloop ( int tree, farg *args, int arity );
DATATYPE f_ins_Neighborloop ( int tree, farg *args, int arity );
*/


//arithmetric
DATATYPE f_nop ( int tree, farg *args, int arity );
DATATYPE f_add ( int tree, farg *args, int arity );
DATATYPE f_sub ( int tree, farg *args, int arity );
DATATYPE f_mul ( int tree, farg *args, int arity );
DATATYPE f_div ( int tree, farg *args, int arity );
DATATYPE f_sin ( int tree, farg *args, int arity );
DATATYPE f_cos ( int tree, farg *args, int arity );
DATATYPE f_exp ( int tree, farg *args, int arity );
DATATYPE f_rlog( int tree, farg *args, int arity );


//erc 
void f_erc_gen ( DATATYPE *r );
char *f_erc_print ( DATATYPE d);
double calculate_erc(double erc);



#endif
