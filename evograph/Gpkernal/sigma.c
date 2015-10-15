#include "lilgp.h"
#include <math.h>


sel_context *select_sigma_context (int op, 
				      sel_context *sc, 
				      population *p,
				      char *string )
{
     interval_data *id;
     int i, j = 0;
     double average;
     double std;
     double n;
  
     switch ( op )
     {
        case SELECT_INIT:
          sc = (sel_context *)MALLOC ( sizeof ( sel_context ) );
          sc->p = p;

          sc->select_method =  select_interval;
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
          
          /* Compute average */

	  average=0;
	  for (i=0;i<p->size;++i)
		  {
		  average += p->ind[i].a_fitness;
		  }
	  average/=p->size;

	  /* Compute Standard Deviation */
	  if (p->size<=1)
	      std=0;
	  else
	      {
	      std=0;
	      for(i=0;i<p->size;i++)
		      {
		      std += (p->ind[i].a_fitness - average) * 
			  (p->ind[i].a_fitness - average);
		      }
	      std/=(p->size-1);
	      std=sqrt(std);
	      }
	  

	  for (i=0; i< p->size; ++i)
		  {
		  if (std==0)
			  {
			  n=1.0;
			  }
		  else
			  {
			  n= 1 + (p->ind[i].a_fitness - average)/(2 * std);
			  }

		  id->total += n;
		  id->ri[j].fitness = id->total;
		  id->ri[j].index = i;
		  ++j;
		  }

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
