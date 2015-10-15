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

typedef struct
{
     int keep_trying;
     int num_times;  /* Number of times we should keep trying. If 0, infinity. --Sean */
     double internal;
     double external;
     double *tree;
     double treetotal;
     char *sname;
     sel_context *sc;
     int method;
     int mindepth, maxdepth;
} mutate_data;

/* operator_mutate_init()
 *
 * initialize a breedphase record for mutation.  much of this is cut and
 * paste from the crossover operator; see that for (a few) more comments.
 */

int operator_mutate_init ( char *options, breedphase *bp )
{
     int errors = 0;
     mutate_data *md;
     int i, j, k, m;
     double r;
     char **argv, **targv;
     int internalset = 0, externalset = 0;
     char *cp;

     md = (mutate_data *)MALLOC ( sizeof ( mutate_data ) );

     /* fill in the breedphase record. */
     bp->operatorID = OPERATOR_MUTATE; //HJJ
     bp->data = (void *)md;
     bp->operator_free = operator_mutate_free;
     bp->operator_start = operator_mutate_start;
     bp->operator_end = operator_mutate_end;
     bp->operator_operate = operator_mutate;

     /* default values for the mutation-specific data structure. */
     md->keep_trying = 0;
//     md->internal = 0.9;
//     md->external = 0.1;
     md->internal = 0.0;
     md->external = 0.0;
     md->tree = (double *)MALLOC ( tree_count * sizeof ( double ) );
     for ( j = 0; j < tree_count; ++j )
          md->tree[j] = 0.0;
     md->treetotal = 0.0;
     md->sname = NULL;
//     md->method = GENERATE_HALF_AND_HALF;
     md->method = GENERATE_FULL;
     md->mindepth = -1;
     md->maxdepth = -1;
     
     j = parse_o_rama ( options, &argv );

     for ( i = 0; i < j; ++i )
     {
          if ( strcmp ( "keep_trying", argv[i] ) == 0 )
          {
               md->keep_trying = translate_binary ( argv[++i] );
               if ( md->keep_trying == -1 )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"%s\" is not a valid setting for \"keep_trying\".",
                           argv[i] );
               }
          }
	  /* parse number of times to try */
	  else if (strcmp ("num_times", argv[1]) == 0)
	  {
	       md->num_times = translate_binary(argv[++i]);
	       if (md->num_times < 0 )
	       {
	           ++errors;
		   error ( E_ERROR, "mutation: \"%s\" is not a valid setting for \"num_times\".",
			  argv[i] );
	       }
	  }
          else if ( strcmp ( "internal", argv[i] ) == 0 )
          {
               internalset = 1;
               md->internal = strtod ( argv[++i], NULL );
               if ( md->internal < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"internal\" must be nonnegative." );
               }
          }
          else if ( strcmp ( "external", argv[i] ) == 0 )
          {
               externalset = 1;
               md->external = strtod ( argv[++i], NULL );
               if ( md->external < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"external\" must be nonnegative." );
               }
          }
          else if ( strcmp ( "select", argv[i] ) == 0 )
          {
               if ( !exists_select_method ( argv[++i] ) )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"%s\" is not a known selection method.",
                           argv[i] );
               }
               FREE ( md->sname );
               md->sname = (char *)MALLOC ( (strlen(argv[i])+1) * sizeof ( char ) );
               strcpy ( md->sname, argv[i] );
          }
          else if ( strcmp ( "method", argv[i] ) == 0 )
          {
               ++i;
               if ( strcmp ( argv[i], "half_and_half" ) == 0 )
                    md->method = GENERATE_HALF_AND_HALF;
               else if ( strcmp ( argv[i], "grow" ) == 0 )
                    md->method = GENERATE_GROW;
               else if ( strcmp ( argv[i], "full" ) == 0 )
                    md->method = GENERATE_FULL;
               else
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"%s\" is not a known generation method.",
                           argv[i] );
               }
          }
          else if ( strcmp ( "depth", argv[i] ) == 0 )
          {
               md->mindepth = strtol ( argv[++i], &cp, 10 );
               if ( *cp == 0 )
                    md->maxdepth = md->mindepth;
               else if ( *cp == '-' )
               {
                    md->maxdepth = strtol ( cp+1, &cp, 10 );
                    if ( *cp )
                    {
                         ++errors;
                         error ( E_ERROR, "mutation: malformed depth string \"%s\".",
                                argv[i] );
                    }
               }
               else
               {
                    ++errors;
                    error ( E_ERROR, "mutation: malformed depth string \"%s\".",
                           argv[i] );
               }
          }
          else if ( strcmp ( "tree", argv[i] ) == 0 )
          {
               k = parse_o_rama ( argv[++i], &targv );
               if ( k != tree_count )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: wrong number of tree fields: \"%s\".",
                           argv[i] );
               }
               else
               {
                    for ( m = 0; m < k; ++m )
                    {
                         md->tree[m] = strtod ( targv[m], &cp );
                         if ( *cp )
                         {
                              ++errors;
                              error ( E_ERROR, "mutation: \"%s\" is not a number.",
                                     targv[m] );
                         }
                    }
               }
               
               free_o_rama ( k, &targv );
          }
          else if ( strncmp ( "tree", argv[i], 4 ) == 0 )
          {
               k = strtol ( argv[i]+4, &cp, 10 );
               if ( *cp )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: unknown option \"%s\".",
                           argv[i] );
               }
               if ( k < 0 || k >= tree_count )
               {
                    ++errors;
                    error ( E_ERROR, "mutation: \"%s\" is out of range.",
                           argv[i] );
               }
               else
               {
                    md->tree[k] = strtod ( argv[++i], &cp );
                    if ( *cp )
                    {
                         ++errors;
                         error ( E_ERROR, "mutation: \"%s\" is not a number.",
                                argv[i] );
                    }
               }
          }
          else
          {
               ++errors;
               error ( E_ERROR, "mutation: unknown option \"%s\".",
                      argv[i] );
          }
     }
     
     free_o_rama ( j, &argv );
     
     if ( internalset && !externalset )
          md->external = 0.0;
     else if ( !internalset && externalset )
          md->internal = 0.0;
     
     if ( md->sname == NULL )
     {
          ++errors;
          error ( E_ERROR, "mutation: no selection method specified." );
     }

     if ( md->mindepth == -1 && md->maxdepth == -1 )
     {
          md->mindepth = 0;
          md->maxdepth = 4;
     }
     
     if ( md->mindepth < 0 || md->maxdepth < 0 ||
         md->maxdepth < md->mindepth )
     {
          ++errors;
          error ( E_ERROR, "mutation: bad depth range.\n" );
     }
     
     for ( j = 0; j < tree_count; ++j )
          md->treetotal += md->tree[j];
     if ( md->treetotal == 0.0 )
     {
          for ( j = 0; j < tree_count; ++j )
               md->tree[j] = 1.0;
          md->treetotal = tree_count;
     }
          
     r = 0.0;
     for ( j = 0; j < tree_count; ++j )
          r = (md->tree[j] += r);

   
     return errors;
}

/* operator_mutate_free()
 *
 * free mutation stuff.
 */

void operator_mutate_free ( void *data )
{
     mutate_data * md;

     md = (mutate_data *)data;
     FREE ( md->sname );
     FREE ( md->tree );
     FREE ( md );
}

/* operator_mutate_start()
 *
 * get selection context for mutation operator.
 */

void operator_mutate_start ( population *oldpop, void *data )
{
     mutate_data * md;
     select_context_func_ptr select_con;

     md = (mutate_data *)data;
     select_con = get_select_context ( md->sname );
     md->sc = select_con ( SELECT_INIT, NULL, oldpop, md->sname );
}

/* operator_mutate_end()
 *
 * free selection context for mutation operator.
 */

void operator_mutate_end ( void *data )
{
     mutate_data * md;

     md = (mutate_data *)data;
     md->sc->context_method ( SELECT_CLEAN, md->sc, NULL, NULL );
}


/* operator_mutate()
 *
 * do the mutation.
 */

void operator_mutate ( population *oldpop, population *newpop,
                      void *data )
{
     int i;
     int ps;
     lnode *replace[2];
     int l, ns;
     int badtree;
     int repcount;
     mutate_data * md;
     int t;
     double r;
     int depth;
     int totalnodes;
     int p;
     int forceany;
     double total;
     int count=0;
     
     md = (mutate_data *)data;
     total = md->internal + md->external;

     /* choose a tree to mutate. */
     r = random_double() * md->treetotal;
     for ( t = 0; r >= md->tree[t]; ++t );

     /* select an individual to mutate. */
     p = md->sc->select_method ( md->sc ); 
     ps = tree_nodes ( oldpop->ind[p].tr[t].data );
     forceany = (ps==1||total==0.0);

     while(1)
     {
          count++;
	  
	  if ( forceany )
	  {
	       /* choose any point. */		///  modify by ksseo
		  while(1)	{				   //  avoid the case of l == 0, i.e. avoid root node
			l = random_int ( ps );
			if (l != 0) {
				replace[0] = get_subtree ( oldpop->ind[p].tr[t].data, l );
				break;
			}
		  }

	  }
	  else if ( total*random_double() < md->internal )	  /// Trouble!  not root, not terminal, So...
	  {
	       /* choose an internal point. */
		  while(1)	{				   //  avoid the case of l == 0
	        l = random_int ( tree_nodes_internal ( oldpop->ind[p].tr[t].data ) );
		    if (l != 0) {
				replace[0] = get_subtree_internal ( oldpop->ind[p].tr[t].data, l );
				break; 
			}
		  }
	  }
	  else
	  {
	       /* choose an external point. */
		  while(1)	{				   //  avoid the case of l == 0
	        l = random_int ( tree_nodes_external ( oldpop->ind[p].tr[t].data ) );
		    if (l != 0) {
				replace[0] = get_subtree_external ( oldpop->ind[p].tr[t].data, l );
				break; 
			}
		  }
	  }
	  
          gensp_reset ( 1 );
	  /* pick a value from the depth ramp. */
          depth = md->mindepth + random_int ( md->maxdepth - md->mindepth + 1 );
	  /* grow the tree. */
          switch ( md->method )
          {
             case GENERATE_GROW:
	       /* Look up the return type of the parent */
               generate_random_grow_tree ( 1, depth, fset+tree_map[t].fset,
					  replace[0]->f->return_type);
               break;
             case GENERATE_FULL:
               generate_random_full_tree ( 1, depth, fset+tree_map[t].fset,
					   replace[0]->f->return_type);
               break;
             case GENERATE_HALF_AND_HALF:
               if ( random_double() < 0.5 )
                    generate_random_grow_tree ( 1, depth, fset+tree_map[t].fset,
					   replace[0]->f->return_type);
               else
                    generate_random_full_tree ( 1, depth, fset+tree_map[t].fset ,
					  replace[0]->f->return_type);
               break;
          }

	  /* count the nodes in the new tree. */
          ns = ps - tree_nodes ( replace[0] ) + tree_nodes ( gensp[1].data );
          totalnodes = ns;

	  /* check the mutated tree against node count and/or size limits. */
          badtree = 0;
          if ( tree_map[t].nodelimit > -1 && ns > tree_map[t].nodelimit )
               badtree = 1;
          else if ( tree_map[t].depthlimit > -1 )
          {
               ns = tree_depth_to_subtree ( oldpop->ind[p].tr[t].data,
                                           replace[0] ) +
                    tree_depth ( gensp[1].data );
               if ( ns > tree_map[t].depthlimit )
                    badtree = 1;
          }

	  /* if tree is too big and keep_trying is set, then skip to the
	     stop and choose a new mutation point/mutant subtree. */
          if ( md->keep_trying && badtree &&
	      (md->num_times==0 ||
	      md->num_times > count) )
               continue;

	  /* check mutated tree against whole-individual node limits. */
          if ( ind_nodelimit > -1 )
          {
               for ( i = 0; i < tree_count; ++i )
                    if ( i != t )
                         totalnodes += oldpop->ind[p].tr[i].nodes;
               badtree |= (totalnodes > ind_nodelimit);
          }

	  /* if tree is too big and keep_trying is set, then skip to the
	     stop and choose a new mutation point/mutant subtree. */
          if ( md->keep_trying && badtree &&
	      (md->num_times==0 ||
	      md->num_times > count))
               continue;

          if ( badtree )
          {
	       /* tree too big but keep_trying not set, just reproduce
		  parent tree. */
               duplicate_individual ( (newpop->ind)+newpop->next,
                                     (oldpop->ind)+p );
				//HJJS  structure niching: mutation
			   //Assumption: we think any structure mutation will create a new structure
			   //for numeric mutation, we won't change the structure

			   // the follwing code applys for individual  operation with structure copy
   			   globaldata* g = get_globaldata();
			   CStruct* pSt;

			   int structID = newpop->ind[newpop->next].structID;
			   pSt = g->pStructHash->find(structID);

			   if(pSt==NULL){ //redundency code for secure safe!! can be removed!
					  pSt = new CStruct();
					  pSt->structID = ++(g->currMaxStructID);
					  pSt->age=1;
					  pSt->lastGen=g->current_generation+1;
					  pSt->nIndividuals=0;
					  pSt->nIndNextGen =1;
					  newpop->ind[newpop->next].structID = pSt->structID;
					  g->pStructHash->insert(pSt);
			   }
			   else{
				   if(pSt->lastGen != g->current_generation+1){//the first individual in this generation
					   pSt->age++;
					   pSt->lastGen = g->current_generation+1;
				   }
				   pSt->nIndNextGen++;//increase the ind no of this population
			   }
			   //HJJS end
          }
          else
          {
	       /* copy the parent tree to the offspring position. */
               duplicate_individual ( (newpop->ind)+newpop->next,
                                     (oldpop->ind)+p );
	       /* free the tree selected for mutation. */
               free_tree ( newpop->ind[newpop->next].tr+t );
               
               /* copy the selected tree, replacing the subtree at the
		  mutation point with the randomly generated tree. */
               replace[1] = gensp[1].data;
               copy_tree_replace_many ( 0, oldpop->ind[p].tr[t].data,
                                       replace, replace+1, 1, &repcount );
               if ( repcount != 1 )
               {
                    error ( E_FATAL_ERROR,
                           "botched mutation:  this can't happen." );
               }
	       /* copy the tree to the new individual. */
               gensp_dup_tree ( 0, newpop->ind[newpop->next].tr+t );
               newpop->ind[newpop->next].evald = EVAL_CACHE_INVALID;
               newpop->ind[newpop->next].flags = FLAG_NONE;

			   //HJJS  structure niching: mutation
			   //Assumption: we think any structure mutation will create a new structure
			   //for numeric mutation, we won't change the structure
   			   globaldata* g = get_globaldata();
			   if(replace[1]->f->return_type != OP_REAL){//structure change
						  //create new struct
						  newpop->ind[newpop->next].structID = ++ (g->currMaxStructID);
						  CStruct* pSt = new CStruct();
						  pSt->structID = newpop->ind[newpop->next].structID;
						  pSt->age=1;
						  pSt->lastGen=g->current_generation+1;
						  pSt->nIndividuals=0;
						  pSt->nIndNextGen=1;
						  g->pStructHash->insert(pSt);
			   }
			   else{//parameter change
				   int structID = newpop->ind[newpop->next].structID;
				   CStruct* pSt = g->pStructHash->find(structID);
				   if(/*oldpop->ind[p].structID==-1 &&*/ pSt==NULL){ //in case of duplicate selection of the same ind in old pop
					   	  pSt = new CStruct();
						  pSt->structID = ++(g->currMaxStructID);
						  newpop->ind[newpop->next].structID = pSt->structID;
						  pSt->age=1;
						  pSt->lastGen=g->current_generation+1;
						  pSt->nIndividuals=0;
						  pSt->nIndNextGen =1;
						  g->pStructHash->insert(pSt);
				   }
				   else{
					   if(pSt->lastGen != g->current_generation+1){//first ind of this structure in the next generations
						   pSt->age++;
						   pSt->lastGen = g->current_generation+1;
					   }
					   pSt->nIndNextGen++;//increase the ind number ofthis structure in  next population
				   }

			   }
			   //HJJS end
               
          }

          ++newpop->next;
          break;
     }
}



//HJJHFC
// Function name	: mutate_individual, used for HFCOMPEL model
// designed by		: Jianjun Hu  04/12/01
// Description	    : mutate a individual 
// Return type		: void 
// Argument         : multipop* mpop
// Argument         : int subpop
// Argument         : int ind
void mutate_individual(multipop* mpop, int subpop, int ind)
{

    char pnamebuf[100];
    int i;
	breedphase* bp;
	int numphases;
	int operatorID;

    bp = (breedphase *)MALLOC ( mpop->size * sizeof ( breedphase) );
    sprintf ( pnamebuf, "subpop[%d].", subpop+1 );//assume always mutation is phase 2 HJJM
    bp = initialize_one_breeding ( pnamebuf );

	numphases = bp[0].operatorID;
	for(i=0;i<=numphases;++i){
		if(bp[i].operator_start == operator_mutate_start){
			bp[i].operator_start(mpop->pop[subpop],bp[i].data);
			operatorID = i;//HJJHFC  BUG found! ichanged later!
			break;
		}
	}


	//do mutation here
	//copy from operatro_mutate (mpop->pop[subpop],bp[i].data);
	void * data = bp[i].data;
	population* oldpop = mpop->pop[subpop];
	population* newpop = mpop->pop[subpop];

	
     int ps;
     lnode *replace[2];
     int l, ns;
     int badtree;
     int repcount;
     mutate_data * md;
     int t;
     double r;
     int depth;
     int totalnodes;
     int p;
     int forceany;
     double total;
     int count=0;
     
     md = (mutate_data *)data;
     total = md->internal + md->external;

     /* choose a tree to mutate. */
     r = random_double() * md->treetotal;
     for ( t = 0; r >= md->tree[t]; ++t );

     /* select an individual to mutate. */
     p = ind; 
     ps = tree_nodes ( oldpop->ind[p].tr[t].data );
     forceany = (ps==1||total==0.0);

     while(1)
     {
          count++;
	  
	  if ( forceany )
	  {
	       /* choose any point. */		///  modify by ksseo
		  while(1)	{				   //  avoid the case of l == 0, i.e. avoid root node
			l = random_int ( ps );
			if (l != 0) {
				replace[0] = get_subtree ( oldpop->ind[p].tr[t].data, l );
				break;
			}
		  }

	  }
	  else if ( total*random_double() < md->internal )	  /// Trouble!  not root, not terminal, So...
	  {
	       /* choose an internal point. */
		  while(1)	{				   //  avoid the case of l == 0
	        l = random_int ( tree_nodes_internal ( oldpop->ind[p].tr[t].data ) );
		    if (l != 0) {
				replace[0] = get_subtree_internal ( oldpop->ind[p].tr[t].data, l );
				break; 
			}
		  }
	  }
	  else
	  {
	       /* choose an external point. */
		  while(1)	{				   //  avoid the case of l == 0
	        l = random_int ( tree_nodes_external ( oldpop->ind[p].tr[t].data ) );
		    if (l != 0) {
				replace[0] = get_subtree_external ( oldpop->ind[p].tr[t].data, l );
				break; 
			}
		  }
	  }
	  
          gensp_reset ( 1 );
	  /* pick a value from the depth ramp. */
          depth = md->mindepth + random_int ( md->maxdepth - md->mindepth + 1 );
	  /* grow the tree. */
          switch ( md->method )
          {
             case GENERATE_GROW:
	       /* Look up the return type of the parent */
               generate_random_grow_tree ( 1, depth, fset+tree_map[t].fset,
					  replace[0]->f->return_type);
               break;
             case GENERATE_FULL:
               generate_random_full_tree ( 1, depth, fset+tree_map[t].fset,
					   replace[0]->f->return_type);
               break;
             case GENERATE_HALF_AND_HALF:
               if ( random_double() < 0.5 )
                    generate_random_grow_tree ( 1, depth, fset+tree_map[t].fset,
					   replace[0]->f->return_type);
               else
                    generate_random_full_tree ( 1, depth, fset+tree_map[t].fset ,
					  replace[0]->f->return_type);
               break;
          }

	  /* count the nodes in the new tree. */
          ns = ps - tree_nodes ( replace[0] ) + tree_nodes ( gensp[1].data );
          totalnodes = ns;

	  /* check the mutated tree against node count and/or size limits. */
          badtree = 0;
          if ( tree_map[t].nodelimit > -1 && ns > tree_map[t].nodelimit )
               badtree = 1;
          else if ( tree_map[t].depthlimit > -1 )
          {
               ns = tree_depth_to_subtree ( oldpop->ind[p].tr[t].data,
                                           replace[0] ) +
                    tree_depth ( gensp[1].data );
               if ( ns > tree_map[t].depthlimit )
                    badtree = 1;
          }

	  /* if tree is too big and keep_trying is set, then skip to the
	     stop and choose a new mutation point/mutant subtree. */
          if ( md->keep_trying && badtree &&
	      (md->num_times==0 ||
	      md->num_times > count) )
               continue;

	  /* check mutated tree against whole-individual node limits. */
          if ( ind_nodelimit > -1 )
          {
               for ( i = 0; i < tree_count; ++i )
                    if ( i != t )
                         totalnodes += oldpop->ind[p].tr[i].nodes;
               badtree |= (totalnodes > ind_nodelimit);
          }

	  /* if tree is too big and keep_trying is set, then skip to the
	     stop and choose a new mutation point/mutant subtree. */
          if ( md->keep_trying && badtree &&
	      (md->num_times==0 ||
	      md->num_times > count))
               continue;

          if ( badtree )
          {

			   continue;//HJJ
          }
          else
          {


               /* copy the selected tree, replacing the subtree at the
		  mutation point with the randomly generated tree. */

               replace[1] = gensp[1].data;
               copy_tree_replace_many ( 0, oldpop->ind[p].tr[t].data,
                                       replace, replace+1, 1, &repcount );
               if ( repcount != 1 )
               {
                    error ( E_FATAL_ERROR,
                           "botched mutation:  this can't happen." );
               }
								reference_ephem_constants ( newpop->ind[ind].tr[t].data, -1 );
								free_tree ( newpop->ind[ind].tr+t );

								/* copy the tree to the new individual. */
								gensp_dup_tree ( 0, newpop->ind[ind].tr+t );
								reference_ephem_constants ( newpop->ind[ind].tr[t].data, +1 );

								newpop->ind[ind].evald = EVAL_CACHE_INVALID;
								newpop->ind[ind].flags = FLAG_NONE;

								app_eval_fitness ( &(mpop->pop[subpop]->ind[ind]) );
               
          }
          break;
     }//end while(1)

		bp[operatorID].operator_end(bp[operatorID].data);

	free_one_breeding(bp);
	//ERC reference



}

