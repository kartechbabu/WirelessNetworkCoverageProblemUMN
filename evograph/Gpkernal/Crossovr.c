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
 ******************************************************************************************
 *	Additional features added by Jianjun Hu  2001-10-11
 *	1. Structur Fitness Sharing (SFS) GECCO2002 paper
 *			Ideas: Each structure can be taken as a peak in the parameter space
 *			as the case of fitness sharing in GA. We use number of individuals of the 
 *			same structure as the number of individuals in the same cluster. 
 *			Hash table technique is used here to speed up the access to structure
 *			information. we can use more sophisticated methods to control the evolution
 *			of structure and parameter such as AGE concept of structure, individuals. 
 *			
 *			How to use SFS:
 *				Just add the following switch in the input file
 *					bUseSFS = 1
 *
 *
 *	2. Probablistic Structure Parameter alternation in evolution
 *			Standard GP doesn't differentiate non-numeric nodes and numeric nodes.
 *			Since there is more numeric nodes in a structure usually, numeric nodes grow
 *			with out control. And especially, some numeric subtree grow much more quickly
 *			than others.
 *			Probablistic structure and parameter alternation technique control the selection
 *			of crossover and mutation nodes according to a prabability.
 *
 *			There are two types of structure parameter alternation:
 *			a). structure and parameter each evolve for several generations.
 *				eg. only structure node crossover or mutation for 4 generations. then
 *				comes 8 generations of parameter node crossover and mutation. 
 *				
 *				how to use:
 *					in input file set the following switch:
 *					StructParamEvolution = 1
 *			b) Probablistic control of structure and parameter node selection for crossover
 *			   and mutation. At each time before selecting node for crossover and mutation,
 *			   a dice is thrown to decide (according to a probability) if we do numeric node 
 *			   operation or non-numeric node operation
 *			
 *				How to use:
 *				in input file set the following switch:
 *				    StructParamEvolution = 1
 *					StructParamEvolution.probabilistic = 1
 */


#include "lilgp.h"
#include <vector>
//select the parameter of parent1 with equal probablity, 
//not proportional to no of its numeric node under the paramter
#define PARAMETER_EQUAL_PROB_P1 
//#define PARAMETER_EQUAL_PROB_P2


typedef struct
{
     int keep_trying;
     int num_times;  /* Number of times we should keep trying. If 0, infinity. --Sean */
     double internal;
     double external;
     double *tree;       /* probability that a given tree
			    will be selected for crossover. */
     double *treecumul;  /* running sum of "tree" field. */
     double treetotal;   /* total of all tree fields. */
     double *func;       /* probability that a given function
			    set will be selected for crossover. */
     char *sname;
     sel_context *sc;
     char *sname2;
     sel_context *sc2;
} crossover_data;

/* operator_crossver_init()
 *
 * called to parse crossover options and initialize one record
 * of a breedphase table appropriately.
 */

int operator_crossover_init ( char *options, breedphase *bp )
{
     int errors = 0;
     crossover_data *cd;
     int i, j, k, m;
     double r;
     char **argv, **targv;
     int internalset = 0, externalset = 0;
     char *cp;

     cd = (crossover_data *)MALLOC ( sizeof ( crossover_data ) );


     /* place values into the breedphase table record. */
     bp->operatorID = OPERATOR_CROSSOVER;  //HJJ  not use reserved word operator
     bp->data = (void *)cd;
     bp->operator_free = operator_crossover_free;
     bp->operator_start = operator_crossover_start;
     bp->operator_end = operator_crossover_end;
     bp->operator_operate = operator_crossover;

     /* default values for all the crossover options. */
//     cd->keep_trying = 0;
     cd->keep_trying = 1;
     cd->num_times = 10;//HJJM
     cd->internal = 0.9;//HJJM
     cd->external = 0.1;//HJJM
//     cd->internal = 0.0;
//     cd->external = 0.0;
     cd->tree = (double *)MALLOC ( tree_count * sizeof ( double ) );
     cd->treecumul = (double *)MALLOC ( tree_count * sizeof ( double ) );
     for ( j = 0; j < tree_count; ++j )
          cd->tree[j] = 0.0;
     cd->treetotal = 0.0;
     cd->func = (double *)MALLOC ( fset_count * sizeof ( double ) );
     cd->sname = NULL;
     cd->sname2 = NULL;

     /* break the options string into an argv-style array of strings. */
     j = parse_o_rama ( options, &argv );

     for ( i = 0; i < j; ++i )
     {
	  /* parse "keep_trying" option. */
          if ( strcmp ( "keep_trying", argv[i] ) == 0 )
          {
	       /* translate a string into a binary value.  returns -1 if
		  the string is not one of the valid strings meaning
		  yes or no. */
               cd->keep_trying = translate_binary ( argv[++i] );
               if ( cd->keep_trying == -1 )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"%s\" is not a valid setting for \"keep_trying\".",
                           argv[i] );
               }
          }
	  /* parse number of times to try */
	  else if (strcmp ("num_times", argv[1]) == 0)
	  {
	       cd->num_times = translate_binary(argv[++i]);
	       if (cd->num_times < 0 )
	       {
	           ++errors;
		   error ( E_ERROR, "crossover: \"%s\" is not a valid setting for \"num_times\".",
			  argv[i] );
	       }
	  }
	  /* parse "internal" option. */
          else if ( strcmp ( "internal", argv[i] ) == 0 )
          {
               internalset = 1;
               cd->internal = strtod ( argv[++i], NULL );
               if ( cd->internal < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"internal\" must be nonnegative." );
               }
          }
	  /* parse "external" option. */
          else if ( strcmp ( "external", argv[i] ) == 0 )
          {
               externalset = 1;
               cd->external = strtod ( argv[++i], NULL );
               if ( cd->external < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"external\" must be nonnegative." );
               }
          }
	  /* parse "select" option. */
          else if ( strcmp ( "select", argv[i] ) == 0 )
          {
               if ( !exists_select_method ( argv[++i] ) )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"%s\" is not a known selection method.",
                           argv[i] );
               }
               FREE ( cd->sname );
               cd->sname = (char *)MALLOC ( (strlen(argv[i])+1) * sizeof ( char ) );
               strcpy ( cd->sname, argv[i] );
               if ( cd->sname2 == NULL )
                    cd->sname2 = cd->sname;
          }
	  /* parse "select2" option. */
          else if ( strcmp ( "select2", argv[i] ) == 0 )
          {
               if ( !exists_select_method ( argv[++i] ) )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"%s\" is not a known selection method.",
                           argv[i] );
               }
               if ( cd->sname2 && cd->sname != cd->sname2 )
                    FREE ( cd->sname2 );
               cd->sname2 = (char *)MALLOC ( (strlen(argv[i])+1) * sizeof ( char ) );
               strcpy ( cd->sname2, argv[i] );
          }
	  /* parse "tree" option. */
          else if ( strcmp ( "tree", argv[i] ) == 0 )
          {
               k = parse_o_rama ( argv[++i], &targv );
               if ( k != tree_count )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: wrong number of tree fields: \"%s\".",
                           argv[i] );
               }
               else
               {
                    for ( m = 0; m < k; ++m )
                    {
                         cd->tree[m] = strtod ( targv[m], &cp );
                         if ( *cp )
                         {
                              ++errors;
                              error ( E_ERROR, "crossover: \"%s\" is not a number.",
                                     targv[m] );
                         }
                    }
               }
               
               free_o_rama ( k, &targv );
          }
	  /* parse "tree#" option. */
          else if ( strncmp ( "tree", argv[i], 4 ) == 0 )
          {
               k = strtol ( argv[i]+4, &cp, 10 );
               if ( *cp )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: unknown option \"%s\".",
                           argv[i] );
               }
               if ( k < 0 || k >= tree_count )
               {
                    ++errors;
                    error ( E_ERROR, "crossover: \"%s\" is out of range.",
                           argv[i] );
               }
               else
               {
                    cd->tree[k] = strtod ( argv[++i], &cp );
                    if ( *cp )
                    {
                         ++errors;
                         error ( E_ERROR, "crossover: \"%s\" is not a number.",
                                argv[i] );
                    }
               }
          }
          else
          {
               ++errors;
               error ( E_ERROR, "crossover: unknown option \"%s\".",
                      argv[i] );
          }
     }
     
     free_o_rama ( j, &argv );
     
     if ( internalset && !externalset )
          cd->external = 0.0;
     else if ( !internalset && externalset )
          cd->internal = 0.0;
     
     if ( cd->sname == NULL )
     {
          ++errors;
          error ( E_ERROR, "crossover: no selection method specified." );
     }

     /** compute "func" array from the "tree" array. **/
     
     for ( j = 0; j < tree_count; ++j )
          cd->treetotal += cd->tree[j];
     if ( cd->treetotal == 0.0 )
     {
          for ( j = 0; j < tree_count; ++j )
               cd->tree[j] = 1.0;
          cd->treetotal = tree_count;
     }
          
     for ( j = 0; j < fset_count; ++j )
          cd->func[j] = 0.0;
     for ( j = 0; j < tree_count; ++j )
          cd->func[tree_map[j].fset] += cd->tree[j];
     
     r = 0.0;
     for ( j = 0; j < fset_count; ++j )
          r = (cd->func[j] += r);
   
     return errors;
}

/* operator_crossover_free()
 *
 * free the crossover-specific data structure.
 */

void operator_crossover_free ( void *data )
{
     crossover_data * cd;

     cd = (crossover_data *)data;

     FREE ( cd->sname );
     if ( cd->sname != cd->sname2 )
          FREE ( cd->sname2 );
     FREE ( cd->tree );
     FREE ( cd->treecumul );
     FREE ( cd->func );
     FREE ( cd );
}

/* operator_crossover_start()
 *
 * called at the start of the breeding process each generation.
 * initializes the selection contexts for this phase.
 */

void operator_crossover_start ( population *oldpop, void *data )
{
     crossover_data * cd;
     select_context_func_ptr select_con;

     cd = (crossover_data *)data;
     
     select_con = get_select_context ( cd->sname );
     cd->sc = select_con ( SELECT_INIT, NULL, oldpop, cd->sname );

     /* if there is a separate selection method specified for the
	second parent... */
     if ( cd->sname2 != cd->sname )
     {
	  /* ...then initialize it too. */
          select_con = get_select_context ( cd->sname2 );
          cd->sc2 = select_con ( SELECT_INIT, NULL, oldpop, cd->sname2 );
     }
     else
	  /* ...otherwise use the first context. */
          cd->sc2 = cd->sc;
}

/* operator_crossover_end()
 *
 * called when breeding is finished each generation.  frees up selection
 * contexts for this phase.
 */

void operator_crossover_end ( void *data )
{
     crossover_data * cd;

     cd = (crossover_data *)data;

     cd->sc->context_method ( SELECT_CLEAN, cd->sc, NULL, NULL );
     if ( cd->sname != cd->sname2 )
          cd->sc2->context_method ( SELECT_CLEAN, cd->sc2, NULL, NULL );
}

/* operator_crossover()
 *
 * performs the crossover, inserting one or both offspring into the
 * new population.
 */

void operator_crossover ( population *oldpop, population *newpop,
                         void *data )
{
	 globaldata* g = get_globaldata();//get the lists

// we should use struct evo stage control for each population and
	 // at each stage, we use probability control the percent of each 
	 // structure or parameter crossover opeation


#ifdef TIME_STATISTICS
	CrossoverTimes++;
	clock_t startT, endT;
	double durationT;
	if(CrossoverTimes%2000==0){
	printf("Crossover operation executed for  %d  times\n", CrossoverTimes);
	startT = clock();
	}
#endif

     crossover_data * cd;
     int p1, p2;
     int ps1, ps2;
     int l1, l2;
     lnode *st[3];
     int sts1, sts2;
     int ns1, ns2;
     int badtree1, badtree2;
     double total;
     int forceany1, forceany2;
     int repcount;
     int f, t1, t2, j;
     double r, r2;
     int totalnodes1, totalnodes2;
     int i;
     int count=0;     /* Number of attempts */


    /* get the crossover-specific data structure. */
     cd = (crossover_data *)data;
     total = cd->internal + cd->external;

     /* choose a function set. */
     r = random_double() * cd->treetotal;
     for ( f = 0; r >= cd->func[f]; ++f );

     /* fill in the "treecumul" array, zeroing all trees which
	don't use the selected function set. */
     r = 0.0;
     t1 = 0;
     for ( j = 0; j < tree_count; ++j )
     {
          if ( tree_map[j].fset == f )
               r = (cd->treecumul[j] = r + cd->tree[j]);
          else
               cd->treecumul[j] = r;
     }

     /* select the first and second trees. */
     r2 = random_double() * r;
     for ( t1 = 0; r2 >= cd->treecumul[t1]; ++t1 );
     r2 = random_double() * r;
     for ( t2 = 0; r2 >= cd->treecumul[t2]; ++t2 );

     /* choose two parents */
     p1 = cd->sc->select_method ( cd->sc );
     ps1 = oldpop->ind[p1].tr[t1].nodes;
     forceany1 = (ps1==1||total==0.0);
     /* 1. if the tree only has one node, we obviously can't do fucntion point 
	 crossoveror. 2. Or we don't want to distinguish internal or external nodes
	 ( that's total = cd->internal + cd->external=0)
	Then we use anypoint instead. */
     
     p2 = cd->sc2->select_method ( cd->sc2 );
     ps2 = oldpop->ind[p2].tr[t2].nodes;
     forceany2 = (ps2==1||total==0.0);






     int reselect_times=0;
     while(1)
     {
				reselect_times++;
				badtree1=0;   /* Moved up here so they don't overwrite my claims --strong typing */
				badtree2=0;

				if(!g->bUseStructParamEvoAlternation){
		//don't use stucture & parameter evolution stage alternation feature
		/* choose two crossover points */

          if ( forceany1 )
          {
	       /* choose any point. */			///  modified by ksseo
			 while(1)	{				   //  avoid the case of l == 0, i.e. avoid root node
               l1 = random_int ( ps1 );
			   if (l1 != 0) {
                  st[1] = get_subtree ( oldpop->ind[p1].tr[t1].data, l1 );
				  break;
			   }
			 }
	      /* print_tree(st[1],stdout);*/
          }
          else if ( total*random_double() < cd->internal )   /// Trouble!  not root, not terminal, So...
          {
								 /* choose an internal point. */
								int icount = 0;					/// 
								while(1)	{				   //  avoid the case of l == 0, i.e. avoid root node
									 l1 = random_int ( tree_nodes_internal (oldpop->ind[p1].tr[t1].data) );
									 if (l1 != 0) {
										st[1] = get_subtree_internal ( oldpop->ind[p1].tr[t1].data, l1 );
										break;
									 }
									 icount++;
									 if (icount >cd->num_times) {				///
										l1 = random_int ( tree_nodes_external (oldpop->ind[p1].tr[t1].data) );
										st[1] = get_subtree_external ( oldpop->ind[p1].tr[t1].data, l1 );
										break;
									 }
								}
          }  
	       /*print_tree(st[1],stdout);*/
          else{
								/* choose an external point. */
								while(1)	{				   //  avoid the case of l == 0, i.e. avoid root node
										l1 = random_int ( tree_nodes_external (oldpop->ind[p1].tr[t1].data) );
										st[1] = get_subtree_external ( oldpop->ind[p1].tr[t1].data, l1 );
										break;
								}
	       /*print_tree(st[1],stdout);*/
          }
	}//	END of if(!g->bUseStructParamEvoAlternation){


	//use Structure and Parameter alternation evolution feature
	else{
		
		//parameter and structure evolution stage exchange

		//use probabilistic structure and parameter alternation
		//else alternate structure and parameter evolution stage every 10 gen (exchange_gen)
		if(g->bProbabilisticSPA){
		//decide the evolution stage probabilistically
			if(random_double()<g->probStructEvoStage)
				g->IsStructEvoStage = true;
			else
				g->IsStructEvoStage = false;
		}

		vector<lnode*> NumericNodelist1;
		vector<lnode*> NonNumericNodelist1;
		get_tree_nodes_numeric(oldpop->ind[p1].tr[t1].data, ps1-1,&NumericNodelist1, &NonNumericNodelist1);
		// Now is the structure evolution stage
		if(g->IsStructEvoStage){
		/* choose two crossover points */
       if ( forceany1 )
        {
							 /* choose any point. */			///  modified by ksseo
							if( NonNumericNodelist1.size()==0){//no structure node, then choose any node
									 l1 = random_int ( NumericNodelist1.size());
									 st[1] = NumericNodelist1[l1];
							}
							else{
									 l1 = random_int ( NonNumericNodelist1.size());
									 st[1] = NonNumericNodelist1[l1];
							}
							/* print_tree(st[1],stdout);*/
       }
       else if ( total*random_double() < cd->internal )   /// Trouble!  not root, not terminal, So...
       {
							if( NonNumericNodelist1.size()==0){//no structure node, then choose any node
									 l1 = random_int ( NumericNodelist1.size());
									 st[1] = NumericNodelist1[l1];
									 //for numeric node, it's meaningless to distinguish internal or external
									 //so we don't treat here
							}
							else{
								 int icount = 0;					/// 
								 while(1){				   //  avoid the case of l == 0, i.e. avoid root node
										 l1 = random_int ( NonNumericNodelist1.size());
										 st[1] = NonNumericNodelist1[l1];
									 /* choose an internal point. */
										 if (st[1]->f->arity != 0) {//this is internal structure node, we get it
											break;
										 }
										 icount++;
										 if (icount >cd->num_times) {
											break;					// I can't get the internal node after num_times try, so just use external node
										 }
								 }  
							}
							 /*print_tree(st[1],stdout);*/
         }//--------------------------------------------------------------------------
         else //choose external structure node
        {
							if( NonNumericNodelist1.size()==0){
							//no structure node, then choose any numeric node
									 l1 = random_int ( NumericNodelist1.size());
									 st[1] = NumericNodelist1[l1]; 
									 //for numeric node, it's meaningless to distinguish internal or external
							}
							else{
								 int icount = 0;					/// 
								 while(1){				   //  avoid the case of l == 0, i.e. avoid root node
										 l1 = random_int ( NonNumericNodelist1.size());
										 st[1] = NonNumericNodelist1[l1];
									 /* choose an internal point. */
										 if (st[1]->f->arity == 0) {//this is external structure node, we get it
											break;
										 }
										 icount++;
										 if (icount >cd->num_times) {
											break;// I can't get the internal node after num_times try, so just use external node
										 }
								 }  
							}
							 /*print_tree(st[1],stdout);*/
		}//end choose external structure node
	}//end the structure evolution stage

	//Now is the parameter stage
	else{
				//see the tree before numeric crossover
			  if( NumericNodelist1.size()==0){//no numeric node, then choose any structure node
					   l1 = random_int ( NonNumericNodelist1.size());
					   st[1] = NonNumericNodelist1[l1];
					   //for numeric node, it's meaningless to distinguish internal or external
					   //so we don't treat here
			  }
			  else{
					vector<lnode*> ParamNodeList1;
					//get the parametric subtree nodes
					get_subtree_parameter_list(oldpop->ind[p1].tr[t1].data, &ParamNodeList1);
#ifdef PARAMETER_EQUAL_PROB_P1
						//Method1: equal chances of selection for each parameter!
						int whichParam = random_int(ParamNodeList1.size());
						lnode* ln = ParamNodeList1[whichParam];
						int whichNode = random_int(tree_nodes(ln));
						st[1] = get_subtree(ln, whichNode);
#else
						//Method2: Uniform choice of numeric node to all numeric nodes
					  l1 = random_int ( NumericNodelist1.size());
					  st[1] = NumericNodelist1[l1];
#endif
			  }
	}
}//END of use param structure evoluation stage alternation

#ifdef NO_ENDNODE_CROSSOVER
	  //what if we don't crossover endn, endr, endnr HJJM 2-12-01
	  if((st[1]->f->code==f_endn ||st[1]->f->code==f_endb||st[1]->f->code==f_endnr)&&
		  reselect_times<cd->num_times) {//endn HJJQ
		  if(ps1==1) return;// parent 1 has only one node
		  else
			continue;
	  }
#endif

	  //Root node check
	  if((st[1]->f->return_type==OP_ROOT)&&
		  reselect_times<cd->num_times) {//endn HJJQ
		  if(ps1==1) return;// parent 1 has only one node
		  else
  	    //print_tree(oldpop->ind[p1].tr[t1].data,stdout);
			continue;
	  }

//now select crossover point 2		  
		  //statistics of crossover type node, for debug  HJJD
		  switch(st[1]->f->return_type){
		  case OP_ROOT:
			  g->SelTimesRoot++;
			  break;
		  case OP_EDGE:
			  g->SelTimesEdge++;
			  break;
		  case OP_NODE:
			  g->SelTimesNode++;
			  break;
		  case OP_REAL:
			  g->SelTimesReal++;
			  break;
		  };
		
		int node1_type = st[1]->f->return_type;//HJJM  
		if(node1_type==OP_ROOT){
			continue;	// Is the root node, then reselect 
						// Here dead loop wouldn't happen, because no tree consists of only one Root node!
		}

		vector<lnode*> Nodelist, NodelistIn, NodelistEx;
		//if the st[1] is endn, endb, endnr, then the following function will not get endn, endb, endnr
		//node in the parent2
		get_tree_nodes_type(oldpop->ind[p2].tr[t2].data, ps2-1, st[1], &Nodelist);
		get_tree_nodes_type_internal(oldpop->ind[p2].tr[t2].data, ps2-1, st[1], &NodelistIn);
		get_tree_nodes_type_external(oldpop->ind[p2].tr[t2].data, ps2-1, st[1], &NodelistEx);

		if(Nodelist.size()==0) { //I can't find paired node in parent 2
			if(reselect_times < cd->num_times){
				continue; //if no paired point in parent 1 then reselect parent1
			}//these two tree seems to nothave comment type nodes, so just stop the crossover!
			else{
				//we just can't paired nodes, then we just mutate parent1 and 2 and copy to new
				//population. HJJM
				break;
			}

		}

//***********************************************************************                                
	if(st[1]->f->return_type == OP_REAL){

#ifdef PARAMETER_EQUAL_PROB_P2
			//Method1: all paramters in parent 2 have equal probability to be changed
			vector<lnode*> ParamNodeList2;
			get_subtree_parameter_list(oldpop->ind[p2].tr[t2].data, &ParamNodeList2);
			int whichParam = random_int(ParamNodeList2.size());
			lnode* ln = ParamNodeList2[whichParam];
			int whichNode = random_int(tree_nodes(ln));
			st[2] = get_subtree(ln, whichNode);

#else
			//Method2: all paramters in parent 2' probability to be changed is proportion to how mnay
			//nodes in its numeric expression tree.
			int whichNode = random_int(Nodelist.size());
			st[2] = Nodelist[whichNode];

#endif //PARAMETER_EQUAL_PROB_P2

	}
	else{ //st1 is not a numeric node
		/* choose two crossover points */
          if ( forceany2 ){
						 /* choose any point on second parent. */
							l2 = random_int ( Nodelist.size());
							st[2] = Nodelist[l2];//Make sure get the same type with ps1
						 /*print_tree(st[2],stdout);*/
          }
          else if ( total*random_double() < cd->internal ){
							 /* choose internal point. */
							if(NodelistIn.size()>0){// there are some good points in Internal points
								l2 = random_int ( NodelistIn.size() );
								st[2] = NodelistIn[l2];
							}
							else{//only in external points
								l2 = random_int ( NodelistEx.size() );
								st[2] = NodelistEx[l2];
							}
          }
          else{ // choose extern points
							 /* choose external point. */
							if(NodelistEx.size()>0){// there are some good points in external points
									l2 = random_int ( NodelistEx.size() );
									st[2] = NodelistEx[l2];
							}
							else{//only in internal points
											l2 = random_int ( NodelistIn.size() );
								st[2] = NodelistIn[l2];
							}
          }
					/*print_tree(st[2],stdout);*/
	}

			//Just can't get two compatible crossover point! //3-8-02  
		//without the following code, everything can be messed up!
		if(st[1]->f->return_type!= st[2]->f->return_type){
			if(cd->num_times==0 ||reselect_times < cd->num_times) continue;
			else break;
		}

	  /** validate the second offspring with regards to the first
	      offspring.  **/
	  //Now the two st[1], st[2] are sure the same return type


	  /* if we're supposed to keep trying, skip up and choose new crossover
	     points. */
	  if ( cd->keep_trying && badtree2){
							if (cd->num_times==0 ||reselect_times < cd->num_times ){//HJJM
									#ifdef DEBUG_CONTINUE
									printf("continue  endb endn endnr\n");
									print_tree ( oldpop->ind[p1].tr[t1].data, stdout );//HJJD
									print_tree ( oldpop->ind[p2].tr[t2].data, stdout );
									#endif 
							continue;
						}
						else{	//if always find endn, then mutate the parents and end crossover
							//HJJADD
							break;
						}
	  }

//*******************************************************************
//now the type is paired
	  /* count the nodes in the selected subtrees. */
          sts1 = tree_nodes ( st[1] );
          sts2 = tree_nodes ( st[2] );

	  /* calculate the sizes of the offspring. */
          ns1 = ps1 - sts1 + sts2;
          ns2 = ps2 - sts2 + sts1;

          totalnodes1 = ns1;
          totalnodes2 = ns2;

	  /** validate the first offspring against the tree node and depth
	    limits; set "badtree1" if any are violated. **/
	  
          /*badtree1 = 0;  moved to the while() loop */
          if ( tree_map[t1].nodelimit > -1 && ns1 > tree_map[t1].nodelimit )
               badtree1 |= 1;
          else if ( tree_map[t1].depthlimit > -1 )
          {
               ns1 = tree_depth_to_subtree ( oldpop->ind[p1].tr[t1].data, st[1] ) +
                     tree_depth ( st[2] );
               if ( ns1 > tree_map[t1].depthlimit )
                    badtree1 |= 1;
          }

	  /* if we're supposed to keep trying, skip up and choose new crossover
	     points. */
        if ( cd->keep_trying && badtree1){
			  if(cd->num_times==0 ||reselect_times<cd->num_times){
				  continue;
			  }
			  else{//HJJADD
 				  break;
			  }
		}

	  /** validate the second offspring against the tree node and depth
	    limits; set "badtree2" if any are violated. **/
	  
          /* badtree2 = 0;  moved to the while() loop */
          if ( tree_map[t2].nodelimit > -1 && ns2 > tree_map[t2].nodelimit )
               badtree2 |= 1;
          else if ( tree_map[t2].depthlimit > -1 )
          {
               ns2 = tree_depth_to_subtree ( oldpop->ind[p2].tr[t2].data, st[2] ) +
                     tree_depth ( st[1] );
               if ( ns2 > tree_map[t2].depthlimit )
                    badtree2 |= 1;
          }

	  /* if we're supposed to keep trying, skip up and choose new crossover
	     points. */
	  
          if ( cd->keep_trying && badtree2 ){
			  if(cd->num_times==0 ||reselect_times < cd->num_times){
					#ifdef DEBUG_CONTINUE
					printf("continue   node2 limit \n");
					#endif 
				continue;
			  }
			  else{//HJJADD
				#ifdef DEBUG_BREAK
				printf("nodelimit break\n");
				#endif
			  break;
			  }
		  }


	  /* check both offspring against the individual node limits, set
	     badtree1 and/or badtree2 if either is too big. */
	  
          if ( ind_nodelimit > -1 )
          {
               for ( i = 0; i < tree_count; ++i )
               {
                    if ( i != t1 )
                         totalnodes1 += oldpop->ind[p1].tr[i].nodes;
                    if ( i != t2 )
                         totalnodes2 += oldpop->ind[p2].tr[i].nodes;
               }
               badtree1 |= (totalnodes1 > ind_nodelimit);
               badtree2 |= (totalnodes2 > ind_nodelimit);
          }

	  /* choose new crossover points if either tree is too big. */
          if ( cd->keep_trying && (badtree1 || badtree2)){
			  if(cd->num_times==0 ||reselect_times < cd->num_times){
				continue;          
			  }
			  else{//HJJADD
				#ifdef DEBUG_BREAK
				printf("individual node limit break\n");
				#endif
			  break;
			  }
		  }

		  //------------------------------------------------------------
		  //in the following case, either we don't set the keep_trying option
		  // or we don't have bad trees in subtree1, subtree2
			//ready for crossover operation

          /* copy the first parent to the first offspring position */
          duplicate_individual ( newpop->ind+newpop->next,
                                oldpop->ind+p1 );
		  g->copy_times++;//HJJD
          if ( !badtree1 )
          {
	       /* if the first offspring is allowable... */
	       
	       /* make a copy of the crossover tree, replacing the
						selected subtree with the crossed-over subtree. */
         copy_tree_replace_many ( 0, oldpop->ind[p1].tr[t1].data,
                                       st+1, st+2, 1, &repcount );
			   g->update_times++;//HJJD
			   g->copy_times--;

	if(g->debug_output==1){
		  printf("---------------------------------\n");
          printf ( "subtree 1 is: " );
          print_tree ( st[1], stdout );
          printf ( "subtree 2 is: " );
          print_tree ( st[2], stdout );
	}

              if ( repcount != 1 )
               {
		    /* this can't happen, but check anyway. */
                    error ( E_FATAL_ERROR,
                           "botched crossover:  this can't happen" );
               }

               /* free the appropriate tree of the new individual */
               free_tree ( newpop->ind[newpop->next].tr+t1 );
               /* copy the crossovered tree to the freed space */
               gensp_dup_tree ( 0, newpop->ind[newpop->next].tr+t1 );

	       /* the new individual's fitness fields are of course invalid. */
					 newpop->ind[newpop->next].evald = EVAL_CACHE_INVALID;
					 newpop->ind[newpop->next].flags = FLAG_NONE;

			   //HJJS  structure niching: mutation
			   //Assumption: we think any structure mutation will create a new structure
			   //for numeric mutation, we won't change the structure
   			   globaldata* g = get_globaldata();
			   if(st[1]->f->return_type != OP_REAL){//structure change
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
				   if( pSt==NULL){
					   	  pSt = new CStruct();
						  pSt->structID = ++(g->currMaxStructID);
						  pSt->age=1;
						  pSt->lastGen=g->current_generation+1;
						  pSt->nIndividuals=0;
						  pSt->nIndNextGen=1;
						  g->pStructHash->insert(pSt);
						  newpop->ind[newpop->next].structID = pSt->structID;
				   }
				   else{
					   if(pSt->lastGen != g->current_generation+1){//first ind of this structure in the next generations
						   pSt->age++;
						   pSt->lastGen = g->current_generation+1;
					   }
					   pSt->nIndNextGen++;//increase the ind no of this population
				   }
			   }
			   //HJJS end
		  }
          else
          {
	       /* offspring too big but keep_trying not set, just leave the copied
		  parent 1 in the offspring position. */
			   //HJJS  structure niching: mutation
			   //Assumption: we think any structure mutation will create a new structure
			   //for numeric mutation, we won't change the structure
			   // the follwing code applys for individual  operation with structure copy
   			   globaldata* g = get_globaldata();
			   CStruct* pSt;

			   int structID = newpop->ind[newpop->next].structID;
			   pSt = g->pStructHash->find(structID);

				if(pSt==NULL){
					  pSt = new CStruct();
					  pSt->structID = ++(g->currMaxStructID);
					  pSt->age=1;
					  pSt->lastGen=g->current_generation+1;
					  pSt->nIndividuals=0;
					  pSt->nIndNextGen=1;
					  newpop->ind[newpop->next].structID =pSt->structID; 
					  g->pStructHash->insert(pSt);
				}
				else{

				   if(pSt->lastGen != g->current_generation+1){//the first individual in this generation
					   pSt->age++;
					   pSt->lastGen = g->current_generation+1;
				   }
				   pSt->nIndNextGen++;//increase the ind no of this population
			   //HJJS end
				}
          }

	  /* we've just filled in one member of the new population. */
          ++newpop->next;
	  /* if the new population needs another member (it's not full) */
	  //-------------------------------------------------------------------------------
          if ( newpop->next < newpop->size )
          {
	       /* copy the second parent to the second offspring position. */
               duplicate_individual ( newpop->ind+newpop->next,
                                     oldpop->ind+p2 );
	   		  g->copy_times++;//HJJD
               if ( !badtree2 )
               {
		    /* if the second offspring is allowable... */
		    /* then make a copy of the tree, replacing the crossover
		       subtree. */
                    copy_tree_replace_many ( 0, oldpop->ind[p2].tr[t2].data,
                                           st+2, st+1, 1, &repcount );
 			   g->update_times++;//HJJD
			   g->copy_times--;

               if ( repcount != 1 )
                {
					 printf(" st1......\n");
					 print_tree(st[1], stdout);
					 printf(" st2......\n");
					 print_tree(st[2], stdout);
                     error ( E_FATAL_ERROR,
                            "bad crossover:  this can't happen" );
                }

		    /* free the old tree in the new individual, and replace
		       it with the crossover tree. */
                    free_tree ( newpop->ind[newpop->next].tr+t2 );
                    gensp_dup_tree ( 0, newpop->ind[newpop->next].tr+t2 );
                  
                    newpop->ind[newpop->next].evald = EVAL_CACHE_INVALID;
                    newpop->ind[newpop->next].flags = FLAG_NONE;

			   //HJJS  structure niching: mutation
			   //Assumption: we think any structure mutation will create a new structure
			   //for numeric mutation, we won't change the structure
   			   globaldata* g = get_globaldata();
			   if(st[2]->f->return_type != OP_REAL){//structure change
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
				   if(pSt==NULL){
					   	  pSt = new CStruct();
						  pSt->structID = ++(g->currMaxStructID);
						  pSt->age=1;
						  pSt->lastGen=g->current_generation+1;
						  pSt->nIndividuals=0;
						  pSt->nIndNextGen=1;
						  g->pStructHash->insert(pSt);
						  newpop->ind[newpop->next].structID = pSt->structID;
				   }
				   else{
					   if(pSt->lastGen != g->current_generation+1){//first ind of this structure in the next generations
						   pSt->age++;
						   pSt->lastGen = g->current_generation+1;
					   }
					   pSt->nIndNextGen++;//increase the ind no of this population
				   }
			   }
			   //HJJS end

               }
               else 
               {
				/* offspring too big but keep_trying not set; just leave
				   the copy of parent 2 where it is. */

					//HJJS  structure niching: mutation
				   //Assumption: we think any structure mutation will create a new structure
				   //for numeric mutation, we won't change the structure
				   // the follwing code applys for individual  operation with structure copy
   				   globaldata* g = get_globaldata();
				   CStruct* pSt;

				   int structID = newpop->ind[newpop->next].structID;
				   pSt = g->pStructHash->find(structID);

					if(pSt==NULL){
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

          ++newpop->next;
               
          }
          break;
     }

#ifdef TIME_STATISTICS
	if(CrossoverTimes%2000==0){
	endT = clock();
	durationT = (double)(endT -startT )/ CLOCKS_PER_SEC;
	printf( "Time used in this crossover operator: %10.8f miliseconds\n", durationT );
	}
#endif

     
}





//***************************************************************
// all the following subroutines are for the improvement of crossover
// HJJ 
//

//HJJM get the nodelist of a certain return type 
lnode * get_tree_nodes_type ( lnode *data, int start, lnode* node, vector<lnode*>* pNodelist  )
{
     lnode *l = data;
     /* initialize a counter with start.  the counter is passed by address,
      * and so is shared across all recursive calls.  it is decremented once
      * for every node in the tree -- the current node when it reaches zero
      * is returned.
      */
     int c = start;
     return get_tree_nodes_type_recurse ( &l, &c, node, pNodelist );
}

lnode *get_tree_nodes_type_recurse ( lnode **l, int *c , lnode* node, vector<lnode*>* pNodelist)
{
     function *f = (**l).f;
     lnode *r;
     int i;
 
	 if( (*l)->f->return_type ==node->f->return_type && 
		 IsUsefulCrossover((*l), node)) pNodelist->push_back((*l));//HJJM

     /* if the counter is zero, return the current subtree. */
     if ( *c == 0 ){
          return *l;
	 }

     ++*l;
     --*c;

     if ( f->arity == 0 )
     {
          /* skip over the terminal nodes. */
          if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          /* recurse into this node's children.  if one of them returns
           * non-NULL, it means that the subtree has been found and the
           * return value is immediately propagated up the call chain.
           * if all of them return NULL, then the desired subtree is
           * not in this subtree and we return NULL. */
          
          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    r = get_tree_nodes_type_recurse ( l, c , node, pNodelist );
                    if ( r != NULL )
					{
						return r;
					}

               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    r = get_tree_nodes_type_recurse ( l, c , node, pNodelist );
                    if ( r != NULL ){
                         return r;
					}
               }
               break;
          }
     }

     return NULL;
}





// Function name	: get_tree_nodes_ERC
// Description	    : Get all the ERC node in a subtree
// Return type		: lnode * 
// Argument         : lnode *data
// Argument         : int start
// Argument         : vector<lnode*>* pERCNodelist
lnode * get_tree_nodes_ERC(lnode *data, int start, vector<lnode*>* pERCNodelist)
{
	//Idea: get all the numeric nodes, and pick up the ERC nodes.
	vector<lnode*> NumericNodelist;
	lnode* rt = get_tree_nodes_numeric(data, start, &NumericNodelist, NULL);
	for(int i=0;i<NumericNodelist.size();i++){
		if(NumericNodelist[i]->f->type == TERM_ERC)
			pERCNodelist->push_back(NumericNodelist[i]);
	}
	return rt;
}



/*HJJM get the numeric nodes (including + - * / etc arithmetic operation nodes)
	of the tree, also return the non-numeric if the last parameter is not NULL*/

lnode * get_tree_nodes_numeric( lnode *data, int start, vector<lnode*>* pNumericNodelist,vector<lnode*>* pNonNumericNodelist)
{
     lnode *l = data;
     /* initialize a counter with start.  the counter is passed by address,
      * and so is shared across all recursive calls.  it is decremented once
      * for every node in the tree -- the current node when it reaches zero
      * is returned.
      */
     int c = start;
     return get_tree_nodes_numeric_recurse ( &l, &c,pNumericNodelist, pNonNumericNodelist);
}

lnode *get_tree_nodes_numeric_recurse ( lnode **l, int *c ,vector<lnode*>* pNumericNodelist,vector<lnode*>* pNonNumericNodelist)
{
     function *f = (**l).f;
     lnode *r;
     int i;
 
//		 printf("in recursive: rtype : %d   f->rtype: ;%d \n",rtype, f->return_type);
	 if( (*l)->f->return_type == OP_REAL) 
		 pNumericNodelist->push_back((*l));//HJJM
	 else if (pNonNumericNodelist!= NULL)
		 pNonNumericNodelist->push_back((*l));//HJJM

     /* if the counter is zero, return the current subtree. */
     if ( *c == 0 ){
          return *l;
	 }

     ++*l;
     --*c;

     if ( f->arity == 0 )
     {
          /* skip over the terminal nodes. */
          if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          /* recurse into this node's children.  if one of them returns
           * non-NULL, it means that the subtree has been found and the
           * return value is immediately propagated up the call chain.
           * if all of them return NULL, then the desired subtree is
           * not in this subtree and we return NULL. */
          
          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    r = get_tree_nodes_numeric_recurse( l, c,pNumericNodelist, pNonNumericNodelist);

                    if ( r != NULL )
					{
						return r;
					}

               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    r = get_tree_nodes_numeric_recurse( l, c,pNumericNodelist, pNonNumericNodelist);
                    if ( r != NULL ){
                         return r;
					}
               }
               break;
          }
     }

     return NULL;
}



lnode *get_tree_nodes_type_internal ( lnode *data, int start ,lnode* node, vector<lnode*>* pNodelist)
{
     lnode *l = data;
     int c = start;
     return get_tree_nodes_type_internal_recurse ( &l, &c, node, pNodelist );
}

lnode *get_tree_nodes_type_internal_recurse ( lnode **l, int *c ,lnode* node, vector<lnode*>* pNodelist)
{
     function *f = (**l).f;
     int i;
     lnode *r;

     /* if the current subtree is a terminal node, skip it immediately. */
     if ( f->arity == 0 )
     {
         if ( f->ephem_gen )
               ++*l;
          ++*l;
     }
     else
     {
				 if( (*l)->f->return_type ==node->f->return_type && 
					 IsUsefulCrossover((*l), node)) pNodelist->push_back((*l));//HJJM

          if ( *c == 0 )
               return *l;

          ++*l;
          --*c;

          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    r = get_tree_nodes_type_internal_recurse ( l, c, node, pNodelist );
                    if ( r != NULL )
                         return r;
               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    r = get_tree_nodes_type_internal_recurse ( l, c, node, pNodelist );
                    if ( r != NULL )
                         return r;
               }
               break;
          }
     }

     return NULL;
}

/*
 * get_subtree_external:  just like get_subtree, but only selects terminal
 *     nodes (that is, 1-node subtrees).  start ranges from
 *     0..(externalnodecount-1).
 */

lnode *get_tree_nodes_type_external ( lnode *data, int start ,lnode* node, vector<lnode*>* pNodelist)
{
     lnode *l = data;
     int c = start;
     return get_tree_nodes_type_external_recurse ( &l, &c , node, pNodelist);
}

lnode *get_tree_nodes_type_external_recurse ( lnode **l, int *c,lnode* node, vector<lnode*>* pNodelist )
{
     function *f = (**l).f;
     int i;
     lnode *r;

     if ( f->arity == 0 )
     {
	 if( (*l)->f->return_type ==node->f->return_type && 
		 IsUsefulCrossover((*l), node)) pNodelist->push_back((*l));//HJJM

          if ( *c == 0 )
               return *l;

          ++*l;
          --*c;

          if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          ++*l;

          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    r = get_tree_nodes_type_external_recurse ( l, c , node, pNodelist);
                    if ( r != NULL )
                         return r;
               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    r = get_tree_nodes_type_external_recurse ( l, c , node, pNodelist);
                    if ( r != NULL )
                         return r;
               }
               break;
          }
     }

     return NULL;
}


//Now the two crosspoints are sure to have the same return_type
//the following cross over is useless, then reselect crossponts
int IsUsefulCrossover(lnode *st1, lnode* st2)
{
		//APPCODE HJJ
	  if(st1->f->code==f_endV && st2->f->code==f_endV) {//endn HJJQ
		  return 0;
	  }
	  else if(st1->f->code==f_endE && st2->f->code ==f_endE) {//endb
		  return 0;
	  }
	  else
		  return 1;
		//APPCODE END
}

// Return type		: lnode *get_subtree_element_list 
// Argument         :  lnode *data
// Argument         : vector<lnode*>* pNodeList, put the node in this container link list
// Get all the nodes which have numeric subtrees
lnode *get_subtree_element_list ( lnode *data,vector<lnode*>* pNodeList)
{
     lnode *l = data;
     /* initialize a counter with start.  the counter is passed by address,
      * and so is shared across all recursive calls.  it is decremented once
      * for every node in the tree -- the current node when it reaches zero
      * is returned.
      */
     return get_subtree_element_list_recurse ( &l, pNodeList );
}

lnode *get_subtree_element_list_recurse ( lnode **l, vector<lnode*>* pNodeList )
{
     function *f = (**l).f;
     lnode *r;
     int i;


     if ( f->arity == 0 )
     {
          /* skip over the terminal nodes. */
   		 ++*l;
		 if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          /* recurse into this node's children.  if one of them returns
           * non-NULL, it means that the subtree has been found and the
           * return value is immediately propagated up the call chain.
           * if all of them return NULL, then the desired subtree is
           * not in this subtree and we return NULL. */
				//APPCODE HJJ
				//collect all the elements which has parametric subtrees
 				 if(f->code == f_insertV||f->code == f_attachV|| f->code==f_splitVL||
					 f->code== f_splitV){
					 //printf("get_subtree_element_list_recurse:\n added node: %s\n", f->string);
					 pNodeList->push_back(*l);
				 }
				//APPCODE END

				 ++*l;
         
          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    r = get_subtree_element_list_recurse ( l, pNodeList );
               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    r = get_subtree_element_list_recurse ( l, pNodeList );
               }
               break;
          }
     }

     return NULL;
}


//HJJM    not debuged , maybe wrong, need test!
lnode** get_node_after_skip_over_subtree( lnode **l )
{
     function *f = (**l).f;
     int i;

     ++*l;
     if ( f->arity == 0 )
     {
          if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
                    skip_over_subtree ( l );
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    skip_over_subtree ( l );
               }
               break;
          }
     }
     return l;
}

//! Get the numeric subtree root
void get_subtree_parameter_list ( lnode *data,vector<lnode*>* pNodeList)
{
			lnode *l = data;
			/* initialize a counter with start.  the counter is passed by address,
			* and so is shared across all recursive calls.  it is decremented once
			* for every node in the tree -- the current node when it reaches zero
			* is returned.
			*/
			function* f;
			lnode *ln;
			
			//get all the elemenet nodes which have numeric subtree
			vector<lnode*> elementNodes;
			get_subtree_element_list ( l, &elementNodes);

			//APPCODE HJJ
			//get each of the parameter subtree and push them into pNodeList
			for(int i=0;i<elementNodes.size();i++){
					 ln = elementNodes[i];
					 f = elementNodes[i]->f;

					 if(f->code == f_insertV||f->code == f_attachV){
							pNodeList->push_back(get_subtree_branch(ln,3));
							pNodeList->push_back(get_subtree_branch(ln,4));
					 }else if(f->code==f_splitVL||f->code== f_splitV){
							pNodeList->push_back(get_subtree_branch(ln,4));
							pNodeList->push_back(get_subtree_branch(ln,5));
					 }
					 //printf("get_subtree_parameter_list\n subtree = %s\n\n", ln->f->string);
			}
			//APPCODE END
}


// Function name	: get_subtree_branch
// Description	    : 
// Return type		: lnode*  
// Argument         : lnode *data
// Argument         : int i    the ith branch of this function node.
lnode*  get_subtree_branch(lnode *data, int index){
	function* f = data->f;
	int nodeSum =0;
	if( index>f->arity){
		printf("get_subtree_branch\n, this function %s don't have %3d branches\n",f->string, index);
		exit(-1);
		return data;
	}

	if(index == 1) return data;

	lnode *currBranchNode = get_subtree(data, 1);
	for(int i=0;i<=index-2;i++){
		int nCurrentBranchNodes = tree_nodes(currBranchNode);
		nodeSum += nCurrentBranchNodes;
		currBranchNode=get_subtree(data,nodeSum+1);
	}
	//print_tree(currBranchNode,stdout);
	return currBranchNode;
}




// Function name	: get the relative nodeno from the root of node *data
// Description	    : 
// Return type		: int *get_node_no 
// Argument         : lnode* tree
// Argument         : lnode *data
int get_node_no (lnode* tree, lnode *data)
{
     lnode *l = data;
     /* initialize a counter with start.  the counter is passed by address,
      * and so is shared across all recursive calls.  it is decremented once
      * for every node in the tree -- the current node when it reaches zero
      * is returned.
      */
     int c = 0;
     
     bool bFound = get_node_no_recurse ( &tree, data, &c );
	 if(bFound) return c;
	 else return -1;
}

bool get_node_no_recurse( lnode **l,lnode* data, int *c )
{
     function *f = (**l).f;
//     lnode *r;
     int i;

	 bool bFound;

     /* if the counter is zero, return the current subtree. */
     if ( *l == data  )
          return true;

     ++*l;
     ++*c;

     if ( f->arity == 0 )
     {
          /* skip over the terminal nodes. */
          if ( f->ephem_gen )
               ++*l;
     }
     else
     {
          /* recurse into this node's children.  if one of them returns
           * non-NULL, it means that the subtree has been found and the
           * return value is immediately propagated up the call chain.
           * if all of them return NULL, then the desired subtree is
           * not in this subtree and we return NULL. */
          
          switch ( f->type )
          {
             case FUNC_DATA:
             case EVAL_DATA:
               for ( i = 0; i < f->arity; ++i )
               {
                    bFound = get_node_no_recurse ( l, data,c );
                    if ( bFound )
                         return true;;
               }
               break;
             case FUNC_EXPR:
             case EVAL_EXPR:
               for ( i = 0; i < f->arity; ++i )
               {
                    ++*l;
                    bFound = get_node_no_recurse ( l, data,c );
                    if ( bFound )
                         return true;;
               }
               break;
          }
     }

     return false;
}