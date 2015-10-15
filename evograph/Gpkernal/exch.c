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
 */


/*  Some modification for Graph evolving  
 *	Jianjun Hu		hujianju@msu.edu
 *	Homepage: www.egr.msu.edu/~hujianju
 *	Genetic Algorithm Research and Application Group
 *  Department of Computer Science
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA  
 */
#include "lilgp.h"

#define NEGATIVE_LARGE -10000.0


//#define REPLACE_WITH_MUTATE_EXPORTED 
//we replace the exported ind in base line using mutate
//else we replace the exported ind in base line using new random generated individuals
//very slow!,
//#define REPLACE_WITH_GENERATE_RANDOM  
//we randomly generate a brand new individuals to replace the exported individuals
#define REPLACE_WITH_COPY_AND_MUTATE
//We copy the unexported candidates in current subpop and mutate it to create a individual to
//replace the exported individuals. Much faster than generate a new individual



/* initialize_topology()
 *
 * reads the parameter database and builds the exchange table.
 */



void initialize_topology ( multipop *mpop )
{
     char pnamebuf[100], pnamebuf2[100];
     char *param, *param2, *param3;
     int i, j, k;
     int errors = 0;
	 globaldata* g=get_globaldata();

     if ( mpop->size == 1 )
     {
	  /* singlepop problem -- no topology needed. */
          mpop->exch = NULL;
          mpop->exchanges = -1;
          return;
     }

     oprintf ( OUT_SYS, 30, "building subpopulation exchange topology:\n" );

     if(g->bUseHFC==true){//we use HFC model

		 //set the subpopActivation flag array
		 mpop->subpopActiveFlag = (int*)MALLOC(mpop->size * sizeof(int));

		 if(g->bAdaptiveHFC ==false){
			 for(i=0;i<mpop->size;i++){ //HJJHFC
				 mpop->subpopActiveFlag[i]=0;//initially only the base level is active
				 //we will set the base level subpops be active after setting up the 
				 //leveltopology
			 }
			 mpop->subpopActiveFlag[mpop->size-1] = 1;//activate the dynamic subpop(the last subpop)
		 }

		 vector<double> subpop_admission_fitness;//keep the admission fit for each subpop
	
		 mpop->pHF = new CHFCPMHierarchy();


		//Static determination of admission levels
		 if(g->bAdaptiveHFC ==false){ 
			 for(i=0;i<mpop->size;i++){
	 				sprintf(pnamebuf,"subpop[%d].admission_fitness",i+1);
					//printf("i = %d %s\n",i, pnamebuf);
					param = get_parameter ( pnamebuf);
					if ( param == NULL ){// not specified, it is the lowest level. admission fitness = 0.0;
						subpop_admission_fitness.push_back(NEGATIVE_LARGE);

						int level = locate_level(NEGATIVE_LARGE, *mpop->pHF);
						if(level<0){// the lowest level not set up, then set up it
							CLevel* pLevelObj = new CLevel(NEGATIVE_LARGE);
							mpop->pHF->insert(mpop->pHF->begin(),pLevelObj);

						}
						else{
							//(*mpop->pHF)[level]->subpop_list.push_back(i);
						}

					}
					else{ // may be lowest level, or a new level, or just add into existing level
						double fitness = atof(param);
						subpop_admission_fitness.push_back(fitness);//

						int level = locate_level(fitness, *mpop->pHF);
						if(level<0){// the level not set up, then set up it by finding its position and insert
							if((*mpop->pHF).size()!=0){//some level already set up !
								vector<CLevel*>::iterator jk;//define the iterator of list
								for(jk=(*mpop->pHF).end()-1;jk>=(*mpop->pHF).begin();jk--){
									double fff = (*jk)->admission_fitness;
									if( fitness > (*jk)->admission_fitness &&jk==mpop->pHF->end()-1){// the highest level
										mpop->pHF->push_back(new CLevel(fitness));
									
										break;
									}
									else if(jk!=mpop->pHF->begin()){
										if(fitness >(*(jk-1))->admission_fitness){
											mpop->pHF->insert(jk,new CLevel(fitness));
											break;
										}
									}
									else{

										mpop->pHF->insert(mpop->pHF->begin(),new CLevel(fitness));

									}
								}
							}
							else{// the level not set up ! then just insert!
								CLevel* pLevelObj = new CLevel(fitness);
								mpop->pHF->insert(mpop->pHF->begin(),pLevelObj);
							}

						}
						else{
							//(*mpop->pHF)[level]->subpop_list.push_back(i);
						}
					}

			 }
			 //set the level to each Clevel object
			 for (j=0;j<mpop->pHF->size();j++){
				 (*mpop->pHF)[j]->level= j;
			 }

			 //decide which subpops belongs to which level
			 for(i=0;i<mpop->size;i++){
				 int nlevel = mpop->pHF->size();
				 for(int level = 0;level<nlevel;level++){
					 individual a;
					 a.s_fitness = subpop_admission_fitness[i];
					 //printf(" sub pop = %d pop fit %f\n\n",i, a.s_fitness);

					 if(IsAdmitted(&a, level,mpop)){
						//printf(" pub sub pop = %d popfit %f into level %d\n\n",i, a.s_fitness, level);
						(*mpop->pHF)[level]->subpop_list.push_back(i);
					 break;
					 }
				 }
			 }
		 }
		 //we use Adaptive HFC model: decide the admission fitness dynamically
		 //we need to set up level hiearchy and the membership of each subpops
		 else{

			 //get the highest admission fitness adpation rate
			 param = get_parameter ( "multiple.HFCPM.adaptive.adaptation_rate" );
			 if(param ==NULL){
				 mpop->pHF->adaptationRate = 0.8;//default
			 }
			 else{
				 mpop->pHF->adaptationRate = atof(param);//default
			 }

			 //We use the how many levels
			 int levels;
			 int subpops = mpop->size;
			 param = get_parameter ( "multiple.HFCPM.adaptive.level" );
			 if(param == NULL){//use default levels: # of subpops
				 levels = mpop->size;
				 for(int i=0;i<levels;i++){
					 CLevel* pLevel = new CLevel(NEGATIVE_LARGE);//all admission fitness set to -INF, which 
																 //will be decided later
					 pLevel->subpop_list.push_back(i);//Add the i subpop to i level,each level has one subpop
					 mpop->pHF->insert(mpop->pHF->begin(),pLevel);
				 }
			 }
			 else{
				 levels = atof(param);
				 if(subpops<levels){//too small subpops to be allocated to each level, so 
									//just use # subpops as the # of levels
					 for(int i=0;i<subpops;i++){
						 CLevel* pLevel = new CLevel(NEGATIVE_LARGE);//all admission fitness set to -INF, which 
																	 //will be decided later
						 pLevel->subpop_list.push_back(i);//Add the i subpop to i level,each level has one subpop
						 mpop->pHF->insert(mpop->pHF->begin(),pLevel);
					 }
				 }
				 else{
					 //how to allocate n subpops to m levels, where n>m
					 //we first assign [n/m] subpops to each level
					 //then, assign mod(n,m) from the uppest level to lower level
					 for(int i=0;i<levels;i++){
						 CLevel* pLevel = new CLevel(NEGATIVE_LARGE);//all admission fitness set to -INF, which 
																	 //will be decided later
						 mpop->pHF->insert(mpop->pHF->begin(),pLevel);
					 }

					 int surplusSubpops = subpops%levels; 
					 int subpopOfEachLevel = subpops/levels;

					 int currentSubpop = subpops-1;
					 for(i=levels-1;i>=0;i--){
						 for(int j=0;j<subpopOfEachLevel;j++){
							(*mpop->pHF)[i]->subpop_list.push_back(currentSubpop);
							currentSubpop--;
						 }
						 if(surplusSubpops!=0){
							(*mpop->pHF)[i]->subpop_list.push_back(currentSubpop);
							currentSubpop--;
							surplusSubpops--;
						 }
					 }
				}//allocate n subpops to m levels

			 }//allocate subpops to levels
			 //set the level to each Clevel object
			 for (j=0;j<mpop->pHF->size();j++){
				 (*mpop->pHF)[j]->level= j;
			 }

		 }//end of use adaptive HFC


		 //report the HFCOMPEL model topology

		 printf("\nHFCOMPEL model hiearchy topology.............................\n");
		 for( i=0;i<subpop_admission_fitness.size();i++){
			 printf("	subpop %d  = admission fit  %f\n", i+1, subpop_admission_fitness[i]);
		 }
		
		 report_HFC_topology(mpop);

	 }
	 
	 
	 //*******************************************************************
	 else{//we use old exchange method

		 param = get_parameter ( "multiple.exchanges" );
		 if ( param == NULL )
		 {
		  /* multipop problem, but no exchanges specified. */
		  
			  mpop->exch = NULL;
			  mpop->exchanges = 0;
			  return;
		 }
		 else
		 {
			  mpop->exchanges = atoi ( param );
			  if ( mpop->exchanges < 0 )
				   error ( E_FATAL_ERROR, "\"exchanges\" must be nonnegative." );
		 }

		 mpop->exch = (exchange *)MALLOC ( mpop->exchanges * sizeof ( exchange ) );
 
		 for ( i = 0; i < mpop->exchanges; ++i )
		 {
		  /** read the destination subpop. **/
		  
			  sprintf ( pnamebuf, "exch[%d].to", i+1 );
			  param = get_parameter ( pnamebuf );
			  if ( param == NULL )
			  {
				   ++errors;
				   error ( E_ERROR, "\"%s\" must be set.", pnamebuf );
			  }
			  else
			  {
				   mpop->exch[i].to = atoi ( param ) - 1;
				   if ( mpop->exch[i].to < 0 || mpop->exch[i].to >= mpop->size )
				   {
						++errors;
						error ( E_ERROR, "\"%s\" is out of range.\n", pnamebuf );
				   }
			  }

		  /** read how the individuals to be replaced in the destination
			subpop are selected. **/
		  
			  sprintf ( pnamebuf, "exch[%d].toselect", i+1 );
			  mpop->exch[i].tosc = get_parameter ( pnamebuf );
			  if ( mpop->exch[i].tosc == NULL )
			  {
				   ++errors;
				   error ( E_ERROR, "\"%s\" must be set.", pnamebuf );
			  }
			  else
			  {
				   if ( ! exists_select_method ( mpop->exch[i].tosc ) )
				   {
						++errors;
						error ( E_ERROR, "\"%s\": \"%s\" is not a selection method.",
							   pnamebuf, mpop->exch[i].tosc );
				   }
			  }

		  /** read how many individuals are to be exchanged in this
			manner. **/
		  
			  sprintf ( pnamebuf, "exch[%d].count", i+1 );
			  param = get_parameter ( pnamebuf );
			  if ( param == NULL )
			  {
				   ++errors;
				   error ( E_ERROR, "\"%s\" must be set.", pnamebuf );
			  }
			  else
			  {
				   mpop->exch[i].count = atoi ( param );
				   if ( mpop->exch[i].count < 0 )
				   {
						++errors;
						error ( E_ERROR, "\"%s\" must be nonnegative.", pnamebuf );
				   }
			  }

		  /** check to see if "from" is specified without a "tree[#]". **/
		  
			  sprintf ( pnamebuf, "exch[%d].from", i+1 );
			  param = get_parameter ( pnamebuf );
			  if ( param )
			  {
				   /** if "from" is specified, then we're copying whole individuals
			 from one subpop to another. **/

			   /* these arrays are not needed. */
				   mpop->exch[i].from = NULL;
				   mpop->exch[i].as = NULL;
			   /* allocate an array of one string (to hold the selection
			  method). */
				   mpop->exch[i].fromsc = (char **)MALLOC ( sizeof ( char * ) );

			   /* the subpop that individuals are taken from. */
				   mpop->exch[i].copywhole = atoi ( param ) - 1;
				   if ( mpop->exch[i].copywhole < 0 ||
						mpop->exch[i].copywhole >= mpop->size )
				   {
						++errors;
						error ( E_ERROR, "\"%s\" is out of range.", pnamebuf );
				   }

			   /* the selection method used to pick the individuals from the
			  source subpop. */
				   sprintf ( pnamebuf, "exch[%d].fromselect", i+1 );
				   mpop->exch[i].fromsc[0] = get_parameter ( pnamebuf );
				   if ( mpop->exch[i].fromsc[0] == NULL )
				   {
						++errors;
						error ( E_ERROR, "\"%s\" must be set.", pnamebuf );
				   }
				   else
				   {
						if ( ! exists_select_method ( mpop->exch[i].fromsc[0] ) )
						{
							 ++errors;
							 error ( E_ERROR, "\"%s\": \"%s\" is not a selection method.",
									pnamebuf, mpop->exch[i].fromsc[0] );
						}
				   }
			  }
			  else
			  {

				   /** since "from" is not defined, we're taking trees from different
			 subpops and merging them to create a composite individual to place
			 in the destination subpop. **/
			   
				   mpop->exch[i].copywhole = -1;
			   /* this array lists, for each tree, which subpop it comes from. */
				   mpop->exch[i].from = (int *)MALLOC ( tree_count * sizeof ( int ) );
			   /* this array keeps track of when two trees are supposed to always
			  come from the same individual (not just the same subpop). */
				   mpop->exch[i].as = (int *)MALLOC ( tree_count * sizeof ( int ) );
			   /* this array holds the selection method strings used for each
			  tree. */
				   mpop->exch[i].fromsc = (char **)MALLOC ( tree_count * sizeof ( char * ) );

			   /* get the default selection method, if one is specified. */
				   sprintf ( pnamebuf, "exch[%d].fromselect", i+1 );
				   param3 = get_parameter ( pnamebuf );
           
				   for ( j = 0; j < tree_count; ++j )
				   {
				/** for each tree, attempt to read the "from" and
				  "fromselect" parameters. **/
				
						sprintf ( pnamebuf, "exch[%d].from.tree[%d]", i+1, j );
						param = get_parameter ( pnamebuf );
						sprintf ( pnamebuf2, "exch[%d].fromselect.tree[%d]",
								 i+1, j );
						param2 = get_parameter ( pnamebuf2 );

						if ( param == NULL && param2 == NULL )
						{
				 /* neither is set, we're supposed to leave this
					tree untouched in the destination individual. */
				 
							 mpop->exch[i].from[j] = -1;
							 mpop->exch[i].as[j] = -1;
							 mpop->exch[i].fromsc[j] = NULL;
						}
						else if ( param2 == NULL )
						{
							 /* only "from" is set, examine param3 for default
					selection method. */

				 /* source subpop. */
							 mpop->exch[i].from[j] = atoi ( param ) - 1;
							 if ( mpop->exch[i].from[j] < 0 || mpop->exch[i].from[j] >= mpop->size )
							 {
								  ++errors;
								  error ( E_ERROR, "\"%s\" is out of range.", pnamebuf );
							 }

				 /* no default set, error. */
							 if ( param3 == NULL )
							 {
								  ++errors;
								  error ( E_ERROR, "\"%s\" must be set.", pnamebuf2 );
							 }
							 else
							 {
								  mpop->exch[i].as[j] = -1;
								  if ( ! exists_select_method ( param3 ) )
								  {
									   ++errors;
									   error ( E_ERROR, "\"%s\": \"%s\" is not a selection method.",
											  pnamebuf, param3 );
								  }
							 }
							 mpop->exch[i].fromsc[j] = param3;
						}
						else if ( param == NULL )
						{
							 /* only "fromselect" is set; it better be of the form
					"as_#". */
                     
							 if ( strncmp ( param2, "as_", 3 ) == 0 )
							 {
								  mpop->exch[i].from[j] = -1;
								  mpop->exch[i].fromsc[j] = NULL;
					  /* "as" stores which tree this one comes from the
					 same subpop as. */
								  mpop->exch[i].as[j] = atoi ( param2 + 3 );
								  if ( mpop->exch[i].as[j] < 0 ||
									  mpop->exch[i].as[j] >= tree_count )
								  {
									   ++errors;
									   error ( E_ERROR, "\"%s\" is out of range.", pnamebuf2 );
								  }
							 }
							 else
							 {
								  ++errors;
								  error ( E_ERROR, "\"%s\" must be \"as_#\".", pnamebuf2 );
							 }
						}
						else
						{
							 /* they're both set. */

							 mpop->exch[i].as[j] = -1;
							 mpop->exch[i].from[j] = atoi ( param ) - 1;
							 if ( mpop->exch[i].from[j] < 0 || mpop->exch[i].from[j] >= mpop->size )
							 {
								  ++errors;
								  error ( E_ERROR, "\"%s\" is out of range.", pnamebuf );
							 }
							 mpop->exch[i].fromsc[j] = param2;
							 if ( ! exists_select_method ( param2 ) )
							 {
								  ++errors;
								  error ( E_ERROR, "\"%s\": \"%s\" is not a selection method.",
										 pnamebuf2, param2 );
							 }
						}
				   }

			   /* now we need to resolve any chains of "as_" references: if
			  tree 2 comes from the same individual as tree 1, and tree 1
			  comes from the same individual as tree 0, we need to change that
			  to say that both 2 and 1 come from tree 0.

			  also detect circular references. */
			   
				   for ( j = 0; j < tree_count; ++j )
				   {
						if ( mpop->exch[i].as[j] == -1 )
							 continue;
						k = mpop->exch[i].as[j];
						while ( k != -1 )
						{
							 if ( k == j )
							 {
								  ++errors;
								  error ( E_ERROR, "Circular reference resolving \"exch[%d].fromselect.tree[%d]\".",
										 i+1, j );
								  j = tree_count;
								  break;
							 }
							 mpop->exch[i].as[j] = k;
							 k = mpop->exch[i].as[k];
						}
						k = mpop->exch[i].as[j];
						if ( mpop->exch[i].from[k] == -1 && mpop->exch[i].as[k] == -1 )
							 mpop->exch[i].as[j] = -1;
				   }
			  }

      }

		 /* if any errors occurred then stop now. */
		 if ( errors )
			  error ( E_FATAL_ERROR, "Errors occurred while building topology.  Aborting." );

		 /* print out the summary of exchanges. */
		 oprintf ( OUT_SYS, 30, "    %d exchange(s) total.\n", mpop->exchanges );
		 for ( i = 0; i < mpop->exchanges; ++i )
		 {
			  oprintf ( OUT_SYS, 30, "    exchange %d:\n", i+1 );
			  oprintf ( OUT_SYS, 30, "        replace %d individual(s) in subpop %d (selected by %s)\n",
					   mpop->exch[i].count, mpop->exch[i].to+1, mpop->exch[i].tosc );
			  if ( mpop->exch[i].copywhole != -1 )
				   oprintf ( OUT_SYS, 30, "        with individual(s) from subpop %d (selected by %s)\n",
							mpop->exch[i].copywhole+1, mpop->exch[i].fromsc[0] );
			  else
				   for ( j = 0; j < tree_count; ++j )
				   {
						if ( mpop->exch[i].from[j] == -1 )
						{
							 if ( mpop->exch[i].as[j] == -1 )
								  oprintf ( OUT_SYS, 30, "        tree %d: leaving original tree\n", j );
							 else
								  oprintf ( OUT_SYS, 30, "        tree %d: from same individual as tree %d\n", j, mpop->exch[i].as[j] );
						}
						else
							 oprintf ( OUT_SYS, 30, "        tree %d: from subpop %d (selected by %s)\n", j,
									  mpop->exch[i].from[j]+1, mpop->exch[i].fromsc[j] );
				   }
		 }
		}//end of old exchange model
}

/* free_topology()
 *
 * this frees the topology table.
 */

void free_topology ( multipop *mpop )
{

//    char pnamebuf[100];
    char *param;
	 //decide if use HFCPM model or use traditional exchange logic
     param = get_parameter ( "multiple.HFCPM" );
     if ( param != NULL )//we use HFCPM model
     {
		 delete mpop->pHF;
		 FREE(mpop->subpopActiveFlag);//HJJHFC
	 }
	 else{//old exchange method HJJ
		 int i;
		 for ( i = 0; i < mpop->exchanges; ++i )
		 {
			  if ( mpop->exch[i].from )
				   FREE ( mpop->exch[i].from );
			  if ( mpop->exch[i].as )
				   FREE ( mpop->exch[i].as );
			  FREE ( mpop->exch[i].fromsc );
		 }
		 if ( mpop->exch )
			  FREE ( mpop->exch );
	 }
}

/* exchange_subpopulations()
 *
 * this performs the actual exchanges, using the information stored
 * in the exchange table.
 */

typedef vector<CSubpopIndividual*> CIndivdiualList;
//HJJM 

void  get_generation_param(int * & mindepth, int * & maxdepth,int * &method, FILE ** &tree_data);



void exchange_subpopulations ( multipop *mpop )
{
     char pnamebuf[100];

     int i, j, k ;
	 int kj;
     sel_context *tocon;
     sel_context **fromcon;
     select_context_func_ptr select_con;
     int tp, *fp;
     int ti, *fi;
	 char* param = NULL;
	 //decide if use HFCPM model or use traditional exchange logic

    if (get_globaldata()->bUseHFC)
    {
				//HFC code removed to reduce confusion
		}
 	 
		else{ // use old method


     /** arrays used for composite individuals. **/

     /* fromcon[j] holds the selection context used to pick individual to take tree j from. */
     fromcon = (sel_context **)MALLOC ( tree_count * sizeof ( sel_context * ) );
     /* fp[j] holds the population from which to take tree j from. */
     fp = (int *)MALLOC ( tree_count * sizeof ( int ) );
     /* fi[j] holds the individual from which to take tree j from. */
     fi = (int *)MALLOC ( tree_count * sizeof ( int ) );

     for ( i = 0; i < mpop->exchanges; ++i )
     {
	  /* where individuals are going. */
          tp = mpop->exch[i].to;

	  /* set up selection method to pick individuals to be replaced. */
          select_con = get_select_context ( mpop->exch[i].tosc );
          tocon = select_con ( SELECT_INIT, NULL, mpop->pop[tp],
                              mpop->exch[i].tosc );

	  /* are we copying whole individuals or creating composites? */
          if ( mpop->exch[i].copywhole > -1 )
          {
	       /*** copying whole individuals. ***/

	       /* the source subpop. */
               fp[0] = mpop->exch[i].copywhole;

	       /* selection method for choosing individuals from source
		  subpop. */
               select_con = get_select_context ( mpop->exch[i].fromsc[0] );
               fromcon[0] = select_con ( SELECT_INIT, NULL, mpop->pop[fp[0]],
                                        mpop->exch[i].fromsc[0] );

               for ( k = 0; k < mpop->exch[i].count; ++k )
               {
		    /** pick an individual to be replaced that has not already
		      been replaced during this exchange cycle. **/
                    do
                    {
                         ti = tocon->select_method ( tocon );
                    }
                    while ( mpop->pop[tp]->ind[ti].flags & FLAG_NEWEXCH );

		    /* pick an individual from the source subpop. */
                    fi[0] = fromcon[0]->select_method ( fromcon[0] );
                    
#ifdef DEBUG
                    printf ( "COPYING WHOLE INDIVIDUAL: ind %d subpop %d --> ind %d subpop %d\n",
                            fi[0], fp[0], ti, tp );
#endif
                    
                    /** remove the old iondividual from the population. **/
                    for ( j = 0; j < tree_count; ++j )
                    {
			 /* always dereference ERCs when removing trees
			    from the population. */
                         reference_ephem_constants ( mpop->pop[tp]->ind[ti].tr[j].data, -1 );
                         free_tree ( mpop->pop[tp]->ind[ti].tr+j );
                    }

		    /* copy the individual. */
                    duplicate_individual ( mpop->pop[tp]->ind+ti,
                                           mpop->pop[fp[0]]->ind+fi[0] );

		    /* reference the ERCs in the new individual. */
                    for ( j = 0; j < tree_count; ++j )
                         reference_ephem_constants ( mpop->pop[tp]->ind[ti].tr[j].data, 1 );

		    /* mark the individual as just coming from an exchange. */
                    mpop->pop[tp]->ind[ti].flags = FLAG_NEWEXCH;
               }

	       /* all done with this exchange, delete the selection context. */
               fromcon[0]->context_method ( SELECT_CLEAN, fromcon[0],
                                           NULL, NULL );
          }
          else
          {
               /*** creating composite individuals. ***/

	       /** create selection contexts for each tree. **/
               for ( j = 0; j < tree_count; ++j )
               {
		    /* does this tree need a context? */
                    if ( mpop->exch[i].fromsc[j] )
                    {
#ifdef DEBUG
                         printf ( "getting selection context for tree %d (%s)\n",
                                 j, mpop->exch[i].fromsc[j] );
#endif
			 /* create it. */
                         select_con = get_select_context ( mpop->exch[i].fromsc[j] );
                         fromcon[j] = select_con ( SELECT_INIT, NULL,
                                                  mpop->pop[mpop->exch[i].from[j]],
                                                  mpop->exch[i].fromsc[j] );
                    }
                    else
			 /* don't need one. */
                         fromcon[j] = NULL;
               }

               for ( k = 0; k < mpop->exch[i].count; ++k )
               {
		    /** select an individual to be replaced that hasn't already
		      been during this exchange cycle. **/
                    do
                    {
                         ti = tocon->select_method ( tocon );
                    }
                    while ( mpop->pop[tp]->ind[ti].flags & FLAG_NEWEXCH );
                    
#ifdef DEBUG
                    printf ( "SELECTED TREE %d FOR REPLACEMENT.\n", ti );
                    print_individual ( mpop->pop[tp]->ind+ti, stdout );
#endif
		    /** now select the individuals that we will merge to
		      replace trees of the destination individual. **/
                    for ( j = 0; j < tree_count; ++j )
                    {
			 /* we don't need to do a selection for a particular
			    tree if (1) it uses the same individual as another
			    tree or (2) it doesn't get replaced in the destination
			    individual. */

                         fp[j] = mpop->exch[i].from[j];
                         if ( fp[j] != -1 )
                         {
                              fi[j] = fromcon[fp[j]]->select_method ( fromcon[fp[j]] );
#ifdef DEBUG
                              printf ( "selecting using (%s) from subpop %d (for tree %d): individual %d\n",
                                      mpop->exch[i].fromsc[j], fp[j], j, fi[j] );
                              print_individual ( mpop->pop[fp[j]]->ind+fi[j], stdout );
#endif
                         }
                    }

		    /** now resolve "as_" references in the fp and fi arrays. */
                    for ( j = 0; j < tree_count; ++j )
                         if ( fp[j] == -1 )
                         {
                              if ( mpop->exch[i].as[j] == -1 )
				   /* tree j doesn't get replaced, so set both
				      values to -1. */
                                   fp[j] = fi[j] = -1;
                              else
                              {
				   /* tree j comes from the same individual as
				      some other tree. */
                                   fp[j] = fp[mpop->exch[i].as[j]];
                                   fi[j] = fi[mpop->exch[i].as[j]];
                              }
                         }

#ifdef DEBUG
                    printf ( "the fp,fi arrays are:\n" );
                    for ( j = 0; j < tree_count; ++j )
                         printf ( "   %3d:  fp = %3d    fi = %4d\n", j, fp[j], fi[j] );
#endif

                    /** replace the appropriate parts of the old tree. **/
                    for ( j = 0; j < tree_count; ++j )
                    {
			 /* skip trees that don't get replaced. */
                         if ( fp[j] == -1 )
                              continue;

			 /* dereference ERCs in old tree. */
                         reference_ephem_constants ( mpop->pop[tp]->ind[ti].tr[j].data, -1 );
			 /* delete old tree. */
                         free_tree ( mpop->pop[tp]->ind[ti].tr+j );
			 /* copy new tree. */
                         copy_tree ( mpop->pop[tp]->ind[ti].tr+j, mpop->pop[fp[j]]->ind[fi[j]].tr+j );
			 /* reference ERCs in new tree. */
                         reference_ephem_constants ( mpop->pop[tp]->ind[ti].tr[j].data, 1 );
                    }
#ifdef COEVOLUTION
    error ( E_FATAL_ERROR, "Can't do COEVOLUTION and multi-pop experiments\n       together at this time, sorry.\n");
#else
		    /* evaluate the fitness of the new composite individual. */
//      printf ( " evaluate in exchange HJJH\n" );
                    app_eval_fitness ( mpop->pop[tp]->ind+ti );
#endif
                    mpop->pop[tp]->ind[ti].flags = FLAG_NEWEXCH;

#ifdef DEBUG
                    printf ( "the new individual is:\n" );
                    print_individual ( mpop->pop[tp]->ind+ti, stdout );
#endif
               }

	       /* destroy source selection contexts. */
               for ( j = 0; j < tree_count; ++j )
                    if ( fromcon[j] )
                         fromcon[j]->context_method ( SELECT_CLEAN,
                                                     fromcon[j], NULL, NULL );
          }

	  /* destroy destination selection context. */
          tocon->context_method ( SELECT_CLEAN, tocon, NULL, NULL );
     }

     FREE ( fromcon );
     FREE ( fp );
     FREE ( fi );

     /* erase all the NEWEXCH flags. */
     for ( i = 0; i < mpop->size; ++i )
          for ( j = 0; j < mpop->pop[i]->size; ++j )
               mpop->pop[i]->ind[j].flags &= ~FLAG_NEWEXCH;
 
	}
}
                   
/* rebuild_exchange_topology()
 *
 * rebuilds the exchange table.  called from user code after making changes
 * to the parameters governing exchanges.
 */

void rebuild_exchange_topology ( multipop *mpop )
{
     free_topology ( mpop );
     initialize_topology ( mpop );
}



void replace_with_random_individual( multipop *mp,int subpop,int ind, int *mindepth,
				  int *maxdepth, int *method, FILE **dataum )
{
//	return;
  int i, j,m;
  int attempts;
  int totalattempts = 0;
  int depth;
  int attempts_generation;
  tree *temp;
  int totalnodes = 0;
  int flag;
  char buf[2048];
  int ignore_limits;

  population* p = mp->pop[subpop];

  /* how many consecutive rejected trees we will tolerate before
     giving up. */
  attempts_generation = atoi ( get_parameter ( "init.random_attempts" ) );
  if ( attempts_generation <= 0 )
    error ( E_FATAL_ERROR,
	    "\"init.random_attempts\" must be positive." );

  ignore_limits = (get_parameter("init.ignore_limits")!=NULL);

  temp = (tree *)MALLOC ( tree_count * sizeof ( tree ) );
     
  attempts = attempts_generation;
  while (attempts>0 )
 {
      /* total nodes in the individual being generated. */
      totalnodes = 0;
          
      for ( j = 0; j < tree_count; ++j )
	{
          
		  if ( attempts <= 0 ){
			error ( E_FATAL_ERROR,"The last %d trees generated have been bad.  Giving up.",
		    attempts_generation );
		  }
               
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
	      continue; //HJJM
	    }
	  
	  /* throw away the individual if it's a duplicate. */
//	  for ( i = 0; i < k; ++i )//HJJM
	  for ( i = 0; i < p->size; ++i )
	    {
	      flag = 0;
	      for ( j = 0; j < tree_count && !flag; ++j ){
		
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

	  if ( i < p->size )
	    {
			  #ifdef DEBUG
			  printf ( "duplicate individual: (same as %d)\n", i );
			  for ( j = 0; j < tree_count; ++j ){
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
      for ( j = 0; j < tree_count; ++j ){
	  	  reference_ephem_constants ( p->ind[ind].tr[j].data, -1 );//HJJM
		  free_tree(&(p->ind[ind].tr[j]));
	  }


      memcpy ( p->ind[ind].tr, temp, tree_count * sizeof ( tree ) );


      /* reference ERCs. */
      for ( j = 0; j < tree_count; ++j )
		reference_ephem_constants ( p->ind[ind].tr[j].data, 1 );//HJJM
          
#ifdef DUMP_POPULATION
      printf ( "individual %5d:\n", ind );
      print_individual ( p->ind+ind, stdout );//HJJ
#endif
      /* mark individual as unevaluated. */
      p->ind[ind].evald = EVAL_CACHE_INVALID;//HJJ
      p->ind[ind].flags = FLAG_NONE;//HJJ
      printf ( " evaluate in replace_with_random_individuall\n" );
     app_eval_fitness ( &(p->ind[ind]) );
        
}

  FREE ( temp );
#ifdef DEBUG_EXCHANGE
       oprintf ( OUT_SYS, 10,
	    "    %d trees to fill the     subpop[ %d ] ind[ %d ] with size %d (%d trees).\n",
	    totalattempts, subpop,ind, p->size, p->size * tree_count );
#endif

}


void  get_generation_param(int * & mindepth, int * & maxdepth,int * &method, FILE ** &tree_data)
{

	char *param;
	char temp_name[256];
	char **tree_replace;
	int i;

	char *cp;
	char pnamebuf[100];
	FILE *fp;

	/* read the depth ramp and generation method(s), which can be
	 different for each tree. */
	tree_replace = (char **)MALLOC(sizeof(char *)*tree_count);

	for( i = 0; i < tree_count; i++ )
	{
	  sprintf( temp_name, "tree-replace[%d]", i );
	  tree_replace[i] = get_parameter( temp_name );
	}

	for( i = 0; i < tree_count; i++ )
	{
	  if( tree_replace[i] != NULL )
	{
	  fp = tree_data[i] = fopen( tree_replace[i], "r" );
	  if( fp == NULL )
		{
		  /* Error Handler */
		  error( E_FATAL_ERROR, "Unable to open file %s\n", tree_replace[i] );
		}
		 
	} else {
	  tree_data[i] = NULL;
	}
	}


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
	error ( E_FATAL_ERROR, "\"init.tree[%d].method\": \"%s\" is not a known generation method.",i, param );
	}
	FREE ( tree_replace );
}


void report_HFC_topology(multipop * mpop)
{
	int i, j;

	//for the level array 0 ----> size, the admission fitenss is ascending order
	//so the highest level is (*mpop->pHF)[mpop->pHF->size()-1]

	for (j=0;j<mpop->pHF->size();j++){
			 printf("		level %d   admission_fitness = %f\n", 	(*mpop->pHF)[j]->level, 
				 (*mpop->pHF)[j]->admission_fitness);
	}

	
	for(i=0;i<mpop->pHF->size();i++){
			 for(j=0;j<(*mpop->pHF)[i]->subpop_list.size();j++){
				 int subpop = (*mpop->pHF)[i]->subpop_list[j];
				 printf("	level %d include subpop %d  size %d \n", i, subpop+1,mpop->pop[subpop]->size);

				 if(mpop->pHF->bAdaptive ==1){
					 //if we use adaptive HFC, we active all subpops when we start evolution
					 mpop->subpopActiveFlag[subpop]=1;
				 }
				 else{
					 //else we only actie the base level subpop
					 if(i==0){ //HJJHFC
						 mpop->subpopActiveFlag[subpop]=1;
					 }
				 }
			 }
		 }
}





//find the level in the hiearchy for a fitness
int locate_level(double fitness, CHFCPMHierarchy& HF){
	int i;
	for(i=0;i<HF.size();i++){
		if(HF[i]->admission_fitness == fitness)
			return i;
	}
	return -1;//not found
}


//check if one individual is admissible to a certain level
bool IsAdmitted( individual* pI, int level, multipop *mpop){

	globaldata* g = get_globaldata();
	double admissionFitness;
	double exportFitness;

	//Decide the admission and export fitness
	if(level ==0){// the base level
		admissionFitness = NEGATIVE_LARGE;
		// export fitness is the fitness of next higher level's admission fitenss
		exportFitness = (*mpop->pHF)[1]->admission_fitness;

	}
	else if(level == mpop->pHF->size()-1){//the highest level
		admissionFitness = (*mpop->pHF)[mpop->pHF->size()-1]->admission_fitness;
		exportFitness = -NEGATIVE_LARGE; //infinity
	}
	else{//intermediate level, 

		admissionFitness = (*mpop->pHF)[level]->admission_fitness;
		// export fitness is the fitness of next higher level's admission fitenss
		exportFitness = (*mpop->pHF)[level+1]->admission_fitness;

	}
#ifdef DEBUG_EXCHANGE

	//printf(" individ = %f   level= %d admiss %f   exportfit %f\n", pI->s_fitness, level, admissionFitness, 
	//	exportFitness);
#endif


	if(!g->bAdaptiveHFC){
		//if we use static HFC, we tend to use raw fitness to compare, since
		//use only know the range of raw fitness, which is also the quantity we want to optimize
		if(pI->s_fitness >=admissionFitness && pI->s_fitness<exportFitness)
			return true;
		else
			return false;
	}
	else{
		//if we use adaptive HFC, we use the adjusted fitness
		if(pI->a_fitness >=admissionFitness && pI->a_fitness<exportFitness)
			return true;
		else
			return false;

	}

}

