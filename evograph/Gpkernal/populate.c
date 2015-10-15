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

//#define DEBUG_SFS

/* Read in an individual from a file, skipping any text in between */
void mod_read_tree_recurse ( int space, ephem_const **eind, FILE *fil, int tree,
			     char *string, int skip )
{
  function *f;
  int i, j;
  ephem_const *ep;

  if( skip )
    {
      i = 0;
      /*Read up until we get the first parenthesies */
      while( i != '(' )
	{
	  if( feof(fil) )
	    {
	      fseek( fil, 0L, SEEK_SET );
	    }
	  i = fgetc(fil);
	}
      if( i != '(' )
	{
	  error( E_FATAL_ERROR, "Ran out of file space while reading in saved tree");
	}
    }
  /* read up until a nonwhitespace character in file.   the nonwhitespace
     character is saved in string[0]. */
  while ( isspace(string[0]=fgetc(fil)) );
  /* get the next character. */
  i = fgetc ( fil );
  if ( isspace(i) )
    /* if the next character is whitespace, then string[0] is a
       one-character function name.  null-terminate the string. */
    string[1] = 0;
  else
    {
      /** if the next character is not whitespace, then string[0]
	is either an open parenthesis or the first character of a
	multi-character function name. **/
	  
      /* push the next character back. */
      ungetc ( i, fil );
      /* read the function name.  skip over an open parenthesis, if there
	 is one. */
      fscanf ( fil, "%s ", string+(string[0]!='(') );
    }
#ifdef DEBUG_READTREE
  fprintf ( stderr, "function name is [%s]\n", string );
#endif

  /* look up the function name in this tree's function set.  if the
     function is an ERC terminal (the name is of the form "name:ERCindex"),
     then place the ERC address in ep. */
  f = get_function_by_name ( tree, string, &ep, eind );
  /* add an lnode to the tree. */
  gensp_next(space)->f = f;
     
  switch ( f->type )
    {
    case TERM_NORM:
    case TERM_ARG:
    case EVAL_TERM:
      break;
    case TERM_ERC:
      /* record the ERC address as the next lnode in the array. */
      gensp_next(space)->d = ep;
      break;
    case FUNC_DATA:
    case EVAL_DATA:
      /** recursively read child functions, no skip nodes needed. **/
      for ( i = 0; i < f->arity; ++i )
	mod_read_tree_recurse ( space, eind, fil, tree, string, 0 );
      break;
    case FUNC_EXPR:
    case EVAL_EXPR:
      /** recursively read child functions, recording skip values. **/
      for ( i = 0; i < f->arity; ++i )
	{
	  /* save an lnode for the skip value. */
	  j = gensp_next_int ( space );
	  /* read the child tree. */
	  mod_read_tree_recurse ( space, eind, fil, tree, string, 0);
	  /* figure out how big the child tree was, and save that
	     number in the skip node. */
	  gensp[space].data[j].s = gensp[space].used-j-1;
	}
      break;
    }
}


/* generate_random_population()
 *
 * fills a population with randomly generated members.
 */

void generate_random_population ( population *p, int *mindepth,
				  int *maxdepth, int *method, FILE **dataum )
{
  int i, j, k, m;
  int attempts;
  int totalattempts = 0;
  int depth;
  int attempts_generation;
  tree *temp;
  int totalnodes = 0;
  int flag;
  char buf[2048];
  int ignore_limits;

  /* how many consecutive rejected trees we will tolerate before
     giving up. */
  attempts_generation = atoi ( get_parameter ( "init.random_attempts" ) );
  if ( attempts_generation <= 0 )
    error ( E_FATAL_ERROR,
	    "\"init.random_attempts\" must be positive." );

  ignore_limits = (get_parameter("init.ignore_limits")!=NULL);

  temp = (tree *)MALLOC ( tree_count * sizeof ( tree ) );
     
  k = 0;
  attempts = attempts_generation;
  while ( k < p->size )
    {
      /* total nodes in the individual being generated. */
      totalnodes = 0;
          
      for ( j = 0; j < tree_count; ++j )
	{
          
	  if ( attempts <= 0 )
	    error ( E_FATAL_ERROR,
		    "The last %d trees generated have been bad.  Giving up.",
		    attempts_generation );
               
	  --attempts;
	  ++totalattempts;

	  /* pick a depth on the depth ramp. */
	  depth = mindepth[j] + random_int ( maxdepth[j] - mindepth[j] + 1 );

	  /* clear a generation space. */
	  gensp_reset ( 0 );

	  /** generate the tree. **/
	  switch ( method[j] )
	    {
	    case GENERATE_FULL:
	      generate_random_full_tree ( 0, depth, fset+tree_map[j].fset,
					  tree_map[j].return_type);
	      break;
	    case GENERATE_GROW:
	      generate_random_grow_tree ( 0, depth, fset+tree_map[j].fset,
					  tree_map[j].return_type);
	      break;
	    case GENERATE_HALF_AND_HALF:
	      if ( random_double() < 0.5  )
		generate_random_full_tree ( 0, depth, fset+tree_map[j].fset,
					    tree_map[j].return_type);
	      else
		generate_random_grow_tree ( 0, depth, fset+tree_map[j].fset,
					    tree_map[j].return_type);
	      break;

	    case LOAD_FILE:
	      gensp_reset(0);
	      mod_read_tree_recurse( 0, NULL, dataum[j], j, buf, 1);
	      /* need to do something here about limit checking */
	      break;
	    }

	      /** throw away the tree if it's too big. **/
	      
	      /*printf("Attempted Tree: ");
		print_tree(gensp[0].data,stdout);*/
	      
	      /* first check the node limits. */
	      flag = 0;
	      m = tree_nodes ( gensp[0].data );


	  if (method[j]!=LOAD_FILE)
	    {
	      if ( tree_map[j].nodelimit > -1 && m > tree_map[j].nodelimit )
		{
		  --j;
		  continue;
		}
	      
	      /* now change the depth limits. */
	      if ( tree_map[j].depthlimit > -1 && tree_depth ( gensp[0].data ) > tree_map[j].depthlimit )
		{
		  --j;
		  continue;
		}
	      
	      /* count total nodes in the individual being created. */
	    }
	  totalnodes += m;
	  gensp_dup_tree ( 0, temp+j );
	}

      if (!ignore_limits)
	{
	  
	  /* is the individual over the total node limit (if one is set)? */
	  if ( ind_nodelimit > -1 && totalnodes > ind_nodelimit )
	    {
#ifdef DEBUG
	      printf ( "overall node limit violated.\n" );
#endif
	      /* yes, so delete it and try again. */
	      for ( j = 0; j < tree_count; ++j )
		free_tree ( temp+j );
	      continue;
	    }
	  
	  /* throw away the individual if it's a duplicate. */
	  for ( i = 0; i < k; ++i )
	    {
	      flag = 0;
	      for ( j = 0; j < tree_count && !flag; ++j )
		{
		  if ( temp[j].size == p->ind[i].tr[j].size )
		    {
		      if ( memcmp ( temp[j].data, p->ind[i].tr[j].data,
				    temp[j].size * sizeof ( lnode ) ) )
			{
			  flag = 1;
			}
		    }
		  else
		    flag = 1;
		}
	      if ( !flag )
		break;
	    }
	  if ( i < k )
	    {
#ifdef DEBUG
	      printf ( "duplicate individual: (same as %d)\n", i );
	      for ( j = 0; j < tree_count; ++j )
		{
		  printf ( "   tree %d: ", j );
		  print_tree ( temp[j].data, stdout );
		}
#endif
	      /* individual is a duplicate, throw it away. */
	      --attempts;
	      for ( j = 0; j < tree_count; ++j )
		free_tree ( temp+j );
	      continue;
	    }
	}

      /** we now have a good individual to put in the population. */

      /* copy the tree array. */
      memcpy ( p->ind[k].tr, temp, tree_count * sizeof ( tree ) );

/*
      for ( j = 0; j < tree_count; ++j )
	{
	 printf("%d %d ",k,j); print_tree (p->ind[k].tr[j].data,stdout);
	}
*/
      /* reference ERCs. */
      for ( j = 0; j < tree_count; ++j )
	reference_ephem_constants ( p->ind[k].tr[j].data, 1 );
          
#ifdef DUMP_POPULATION
      printf ( "individual %5d:\n", k );
      print_individual ( p->ind+k, stdout );
#endif
      /* mark individual as unevaluated. */
      p->ind[k].evald = EVAL_CACHE_INVALID;
      p->ind[k].flags = FLAG_NONE;

	  //HJJS structure Niching: generate_random_population

	  //We assume for random generated populations, all individuals belong to 
	  //different structures. Anyhow, this assumption is not very critical to
	  //the effect of structure niching.
	  globaldata* g = get_globaldata();
	  p->ind[k].structID = ++ (g->currMaxStructID);
	  CStruct * pSt = new CStruct();
	  pSt->structID = p->ind[k].structID;
	  pSt->age=1;
	  pSt->lastGen=g->current_generation;
	  pSt->nIndividuals=1;
	  pSt->nIndNextGen=1;
	  g->pStructHash->insert(pSt);
#ifdef DEBUG_SFS
	  printf(" insert at generate random pop\n");
#endif 
	  //HJJS end


      attempts = attempts_generation;
      ++k;
          
    }

  FREE ( temp );
     
  oprintf ( OUT_SYS, 10,
	    "    %d trees were generated to fill the population of %d (%d trees).\n",
	    totalattempts, p->size, p->size * tree_count );

}

/* allocate_population()
 *
 * allocates a population structure with the given size.
 */

population *allocate_population ( int size )
{
  int i;
  population *p = (population *)MALLOC ( sizeof ( population ) );

  p->size = size;
  p->next = 0;
  /* allocate the array of individuals. */
  p->ind = (individual *)MALLOC ( size * sizeof ( individual ) );

  for ( i = 0; i < size; ++i )
    {
      /* each individual has a block of tree structures, allocate
	 those here. */
      p->ind[i].tr = (tree *)MALLOC ( tree_count * sizeof ( tree ) );
      p->ind[i].evald = EVAL_CACHE_INVALID;
      p->ind[i].flags = FLAG_NONE;
    }

  return p;
}

/* free_multi_population()
 *
 * frees the populations in a multipop structure.
 */

void free_multi_population ( multipop *mp )
{
  int i;
  for ( i = 0; i < mp->size; ++i )
    free_population ( mp->pop[i] );
  FREE ( mp->pop );
  FREE ( mp );
}

/* free_population()
 *
 * frees a population and all the individuals in it.
 */

void free_population ( population *p )
{


	int i, j;
  for ( i = 0; i < p->size; ++i )
    {
      for ( j = 0; j < tree_count; ++j )
	{
	  /* dereference ERCs. */
	  reference_ephem_constants ( p->ind[i].tr[j].data, -1 );
	  free_tree ( &(p->ind[i].tr[j]) );
	}
      FREE ( p->ind[i].tr );
    }
  

  FREE ( p->ind );
  FREE ( p );
}

/* initial_multi_population()
 *
 * randomly fills a multipop strcture with individuals.
 */

multipop *initial_multi_population ( void )
{
  char *param;
  char temp_name[256];
  char **tree_replace;
  multipop *mpop;
  int i;

  int *mindepth, *maxdepth, *method;
  char *cp;
  char pnamebuf[100];
  FILE **tree_data;
//  struct stat tree_data_info;
  FILE *fp;

  oputs ( OUT_SYS, 10, "creating initial population(s):\n" );

  mpop = (multipop *)MALLOC ( sizeof ( multipop ) );

  /* how many subpops are we supposed to have? */
  param = get_parameter ( "multiple.subpops" );
  mpop->size = atoi ( param );
  if ( mpop->size <= 0 )
    error ( E_FATAL_ERROR,
	    "\"%s\" is not a valid value for \"multiple.subpops\".",
	    param );

  /* allocate that many population pointers. */
  mpop->pop = (population **)MALLOC ( sizeof ( population* ) * mpop->size );

  /* read the depth ramp and generation method(s), which can be
     different for each tree. */
  tree_replace = (char **)MALLOC(sizeof(char *)*tree_count);
  tree_data = (FILE **)MALLOC( sizeof(FILE *)*tree_count);

  for( i = 0; i < tree_count; i++ )
    {
      sprintf( temp_name, "tree-replace[%d]", i );
      tree_replace[i] = get_parameter( temp_name );
    }

  for( i = 0; i < tree_count; i++ )
    {
      if( tree_replace[i] != NULL )
	 {
	  //tree_data[i] = fopen( tree_replace[i], "r" );//HJJ
	  fp = tree_data[i] = fopen( tree_replace[i], "r" );//HJJ
	  if( fp == NULL )
	    {
	      /* Error Handler */
	      error( E_FATAL_ERROR, "Unable to open file %s\n", tree_replace[i] );
	    }
	     
	} else {
	  tree_data[i] = NULL;
	}
    }

  mindepth = (int *)MALLOC ( tree_count * sizeof ( int ) );
  maxdepth = (int *)MALLOC ( tree_count * sizeof ( int ) );
  method = (int *)MALLOC ( tree_count * sizeof ( int ) );

  for ( i = 0; i < tree_count; ++i )
    {
      mindepth[i] = -1;
      maxdepth[i] = -1;

      /** read the depth ramp. **/
      sprintf ( pnamebuf, "init.tree[%d].depth", i );
      param = get_parameter ( pnamebuf );
      if ( !param )
	param = get_parameter ( "init.depth" );
      if ( param == NULL )
	error ( E_FATAL_ERROR, "\"init.tree[%d].depth\" must be specified.", i );

      /** parse the depth ramp ("min-max" or just "val"). **/
      mindepth[i] = strtol ( param, &cp, 10 );
      if ( *cp )
	{
	  if ( *cp == '-' )
	    {
	      maxdepth[i] = strtol ( cp+1, &cp, 10 );
	    }
	  if ( *cp )
	    {
	      error ( E_FATAL_ERROR, "\"init.tree[%d].depth\" is malformed.",
		      i );
	    }
	}
      if ( maxdepth[i] == -1 )
	maxdepth[i] = mindepth[i];

      if ( mindepth[i] > maxdepth[i] )
	error ( E_FATAL_ERROR, "\"init.tree[%d].depth\" is malformed.", i );

      /** read the method. **/
      sprintf ( pnamebuf, "init.tree[%d].method", i );
      param = get_parameter ( pnamebuf );
      if ( !param )
	param = get_parameter ( "init.method" );

      if ( param == NULL )
	error ( E_FATAL_ERROR, "\"init.tree[%d].method\" must be set.",
		i );

      if ( strcmp ( param, "half_and_half" ) == 0 )
	method[i] = GENERATE_HALF_AND_HALF;
      else if ( strcmp ( param, "full" ) == 0 )
	method[i] = GENERATE_FULL;
      else if ( strcmp ( param, "grow" ) == 0 )
	method[i] = GENERATE_GROW;
      else if ( strcmp ( param, "load" ) == 0 )
	method[i]= LOAD_FILE;
      else
	error ( E_FATAL_ERROR, "\"init.tree[%d].method\": \"%s\" is not a known generation method.",
		i, param );
    }

  /* generate each population. */
  for ( i = 0; i < mpop->size; ++i )
    mpop->pop[i] = initial_population (i, mindepth, maxdepth, method, tree_data );

  for( i = 0; i < tree_count; i++ )
    {
      if( tree_data[i] != NULL )
	fclose(tree_data[i]);     
    }

  FREE ( mindepth );
  FREE ( maxdepth );
  FREE ( method );
  FREE ( tree_replace );
  FREE ( tree_data );

  oputs ( OUT_SYS, 10, "    initial population(s) complete.\n" );
          
  return mpop;
}

/* initial_population()
 *
 * creates a population structure and fills it with randomly
 * generated individuals.
 */

population *initial_population (int which_subpop, int *mindepth, int *maxdepth, int *method, FILE **dataum )
{
	population *pop;
	int pop_size;
	char *param;
 
	/* get the default population size and allocate. */
 
	param = get_parameter ( "pop_size" );
	if ( param == NULL )
	error ( E_FATAL_ERROR,
		"no value specified for \"pop_size\"." );
	pop_size = atoi ( param );

//HJJB************************************************************************
//support different sub population size 
	//Get the pop_size of this subpopulation 
	char buffer[30];
	if(which_subpop<10)
		sprintf(buffer,"subpop[%d].pop_size",which_subpop+1);
	else if(which_subpop<100)
		sprintf(buffer,"subpop[%2d].pop_size",which_subpop+1);
	else
		sprintf(buffer,"subpop[%3d].pop_size",which_subpop+1);
	param = get_parameter ( buffer);//HJJM  
	if(param != NULL){ // specified popsize for this subpop, then use it, else use default popsize
		pop_size = atoi(param);
	}
//HJJE************************************************************************



	pop = allocate_population ( pop_size );

	/* get the generation method and create the random population. */

	generate_random_population ( pop, mindepth, maxdepth, method, dataum );
 
	return pop;
}
     
