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
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *  
 */


#include "lilgp.h"
#include <math.h>

#define _E_ 2.718281828459

/* This version of the boltzman distribution will not work with
   checkpoint files--we'll have to add the globals below into the global
   section of the checkpoint file later... -- Sean */



int boltzman_initialized = 0;     /* This SHOULD start out 0!!! */     
double boltzman_hi;
double boltzman_low;
double boltzman_step; 
double boltzman_t;             /* Current Temperature */

sel_context *select_boltzman_context (int op, 
				      sel_context *sc, 
				      population *p,
				      char *string )
{
     interval_data *id;
     int i, j = 0;
     double n;          /* Our New Boltzman Value */

     switch ( op )
     {
        case SELECT_INIT:
          sc = (sel_context *)MALLOC ( sizeof ( sel_context ) );
          sc->p = p;
          sc->select_method =  select_interval; //HJJ C++ explicit type conversion

          sc->context_method = select_boltzman_context;

	  /* the interval_data structure (used with select_interval()) is
	     essentially a list of interval widths and indices for each
	     individual. */
	  
          id = (interval_data *)MALLOC ( sizeof ( interval_data ) );

          id->ri = (reverse_index *)MALLOC ( (p->size+1) *
                                           sizeof ( reverse_index ) );
          id->total = 0.0;
          id->count = p->size;
          id->ri[j].fitness = 0.0;
          id->ri[j].index = -1;
          ++j;
          
	  if (!boltzman_initialized)
		  {
		  char* param;

		  boltzman_initialized=1;
		  param=get_parameter("boltzman_hi");
		  if (param==NULL)
			  {
			  param=get_parameter("max_generations");
			  if (param==NULL) param="51.0";  /* Default */
			  }
		  boltzman_hi=atof(param);

		  param=get_parameter("boltzman_low");
		  if (param==NULL) param="0.0"; /* Default */
		  boltzman_low=atof(param);

		  param=get_parameter("boltzman_step");
		  if (param==NULL) param="1.0"; /* Default */
		  boltzman_step=atof(param);

		  boltzman_t=boltzman_hi;
		  }

          for ( i = 0; i < p->size; ++i )
          {
	
	  n = pow(_E_,(p->ind[i].a_fitness / boltzman_t));   
	                  /* Boltzman distribution modification */

	  boltzman_t -= boltzman_step;
	  if (boltzman_t < boltzman_low) boltzman_t=boltzman_low;
	  
	  id->total += n;
	  id->ri[j].fitness = id->total;
	  id->ri[j].index = i;
	  ++j;
          }

	  /* We don't need to divide everything by the total to
	     normalize the boltzman results--we're done as it is. */

          sc->data = (void *)id;
          return sc;
          break;

        case SELECT_CLEAN:
          id = (interval_data *)(sc->data);
          FREE ( id->ri );
          
          FREE ( sc->data );
          FREE ( sc );
          return NULL;
          break;
     }

     return NULL;
}
