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


/* 
 *	Mutate the ERC constants of a GP tree
 *	Added by Jianjun Hu 2002-2-1  to 2/8/2002
 *	Computer Science Department
	
		ERC Mutate principles:
		
		ERC mutation is very important to parameter evolution. To some extent, 
		the limited supply of ERCs will make it extremely difficult to fine tune
		some parameters, which prevent refine the good solutions. In koza's work
		ERC mutation has a 20% probability. 


		1. Two parameters:   
					varpercent:		how much percent of parameters of each tree will be mutated
												(default: 20%)
					ercpercent:		how much percent of the ERC terminals of each numeric subtree of
												each parameter will be mutated
												(default: 20%)
		2. Gaussian Mutation
					For each ERC
					new value  =  old value + oldvalue * 10% * Gaussian(0,1)
		3. Crossreference of lilgp ERC
					Since lilgp share the ERC for many trees in the population, so change ERC of one
					individuals will potentially affect other individuals, which will make their 
					fitness value invalid (but here I didn't re-evaluate their fitness. 

					One solution is use more complex algorithm, that's each time when we mutate
					a ERC,if the ref_no is bigger than 1, we then create a new ERC nodes, and
					reduce their original ERC node's ref_no by 1. (this will be implemented later)
		4. How to use erc_mutate feature
					Add the following ERC_mutation information

					breed[4].operator = mutate_erc, select = fitness
					breed[4].varpercent=0.2, breed[4].ercpercent = 0.4
					breed[4].rate = 0.2

*/


#include "lilgp.h"

typedef struct
{
     double var_percent;	//how many percent of the variables of the GP individuals to be changed
     double erc_percent;	//how many percent of ERC terminals of each variable to be mutated.
													//HJJ_ME
     double *tree;
     double treetotal;
     char *sname;
     sel_context *sc;
} mutate_erc_data;

/* operator_mutate_erc_init()
 *
 * initialize a breedphase record for erc mutation.  much of this is cut and
 * paste from the crossover operator; see that for (a few) more comments.
 */

int operator_mutate_erc_init ( char *options, breedphase *bp )
{
     int errors = 0;
     mutate_erc_data *md;
     int i, j;
     double r;
     char **argv;
     int internalset = 0, externalset = 0;

     md = (mutate_erc_data *)MALLOC ( sizeof ( mutate_erc_data ) );

     /* fill in the breedphase record. */
     bp->operatorID = OPERATOR_MUTATE_ERC; //HJJ
     bp->data = (void *)md;
     bp->operator_free = operator_mutate_erc_free;
     bp->operator_start = operator_mutate_erc_start;
     bp->operator_end = operator_mutate_erc_end;
     bp->operator_operate = operator_mutate_erc;

     /* default values for the mutation-specific data structure. */
     md->var_percent = 0.2;
     md->erc_percent = 0.2;
     md->tree = (double *)MALLOC ( tree_count * sizeof ( double ) );
     for ( j = 0; j < tree_count; ++j )
          md->tree[j] = 0.0;
     md->treetotal = 0.0;
     md->sname = NULL;
     
     j = parse_o_rama ( options, &argv );

     for ( i = 0; i < j; ++i )
     {
					if ( strcmp ( "select", argv[i] ) == 0 )
          {
               if ( !exists_select_method ( argv[++i] ) )
               {
                    ++errors;
                    error ( E_ERROR, "mutation erc: \"%s\" is not a known selection method.",
                           argv[i] );
               }
               FREE ( md->sname );
               md->sname = (char *)MALLOC ( (strlen(argv[i])+1) * sizeof ( char ) );
               strcpy ( md->sname, argv[i] );
          }
          else if ( strcmp ( "varpercent", argv[i] ) == 0 )
          {
               md->var_percent = strtod ( argv[++i], NULL );
               if ( md->var_percent < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "mutation_erc: \"var_percent\" must be nonnegative." );
               }
          }
          else if ( strcmp ( "ercpercent", argv[i] ) == 0 )
          {
               md->erc_percent = strtod ( argv[++i], NULL );
               if ( md->erc_percent < 0.0 )
               {
                    ++errors;
                    error ( E_ERROR, "mutation erc: \"erc_percent\" must be nonnegative." );
               }
          }

          else
          {
               ++errors;
               error ( E_ERROR, "mutation erc: unknown option \"%s\".",
                      argv[i] );
          }
     }
     
     free_o_rama ( j, &argv );
  
     if ( md->sname == NULL )
     {
          ++errors;
          error ( E_ERROR, "mutation erc: no selection method specified." );
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

/* operator_mutate_erc_free()
 *
 * free mutation stuff.
 */

void operator_mutate_erc_free ( void *data )
{
     mutate_erc_data * md;

     md = (mutate_erc_data *)data;
     FREE ( md->sname );
     FREE ( md->tree );
     FREE ( md );
		#ifdef DEBUG_ERC_MUTATE
				 printf ( "ERC MUTATION free.\n\n\n" );
		#endif

}

/* operator_mutate_erc_start()
 *
 * get selection context for mutation operator.
 */

void operator_mutate_erc_start ( population *oldpop, void *data )
{
     mutate_erc_data * md;
     select_context_func_ptr select_con;

     md = (mutate_erc_data *)data;
     select_con = get_select_context ( md->sname );
     md->sc = select_con ( SELECT_INIT, NULL, oldpop, md->sname );
}

/* operator_mutate_erc_end()
 *
 * free selection context for mutation operator.
 */

void operator_mutate_erc_end ( void *data )
{
     mutate_erc_data * md;

     md = (mutate_erc_data *)data;
     md->sc->context_method ( SELECT_CLEAN, md->sc, NULL, NULL );
}


/* operator_mutate_erc()
 *
 * do the mutation.
 */
void operator_mutate_erc ( population *oldpop, population *newpop,
                      void *data )
{
			int i;
			int ps;
			mutate_erc_data * md;
			int t;
			double r;
			int p;
			int count=0;

			md = (mutate_erc_data *)data;

			/* choose a tree to mutate. */
			//t: the tTH tree of the individual
			r = random_double() * md->treetotal;
			for ( t = 0; r >= md->tree[t]; ++t );

			/* select an individual to mutate. */
			//p:  parent invidualIP
			//ps: parent tree size
			p = md->sc->select_method ( md->sc ); 
			ps = tree_nodes ( oldpop->ind[p].tr[t].data );

			vector<lnode*> ParamNodeList;		//the root nodes of all numeric subtree
			vector<lnode*> MutatedParamNodelist;////the root nodes of all numeric subtree to be mutated
			vector<lnode*> ERCNodelist;			//ERC nodelist for a subtree

			get_subtree_parameter_list(oldpop->ind[p].tr[t].data, &ParamNodeList);

			//decide which parameters needs to be mutated
			int nParam = ParamNodeList.size();

			if (nParam >= 1){
				int nParamMutated = nParam * md->var_percent;
				//at least we mutate one parameter!
				if (nParamMutated<1) nParamMutated = 1;

				//we randomly select nParamMutated parameters to be mutated. 
				//we allow duplicate selection of parameters.
				for(i=0;i<nParamMutated;i++){
					int vi = random_int (nParam);
					MutatedParamNodelist.push_back(ParamNodeList[vi]);
				}

				//for each parameter subtree, we randomly selection a portion of 
				//its ERCs and mutate them.
				for(i=0;i<nParamMutated;i++){
								ERCNodelist.clear();
								lnode* pNode = MutatedParamNodelist[i];
								/*	there are too possiblility
										1: the root node of numeric subtree is a ERC itself.
										2: it is a internal node*/

								if( pNode->f->type == TERM_ERC){
										//we mutate a ERC by ERC+ 5%ERC* N(0,1)
										++pNode; //HJJ!!!!!! The next node of pNode->f is the ERC node!

										double value = pNode->d->d;
										pNode->d->d = value * (0.05* gauss_dist()+1); 
										#ifdef DEBUG_ERC_MUTATE 
											printf("ERC  init_value = %f   new value = %f\n", value, pNode->d->d);
										#endif
								}
								else{
										int tSize = tree_nodes(pNode);
										get_tree_nodes_ERC(pNode,tSize-1, &ERCNodelist);
					
										//decide how many ERCs of this subtree is to be mutated.
										int nERC = ERCNodelist.size();
										
										if (nERC<1) continue;	//no ERC in this subtree. Is this possible?
										int nERCmut= nERC * md->erc_percent;
										if (nERCmut <1) nERCmut=1; //at least mutate one ERC
										
										for(int j=0;j<nERCmut;j++){
												lnode* pNode = ERCNodelist[ random_int(nERC)];
												//we mutate a ERC by ERC+ 5%ERC* N(0,1)
												++pNode;
												double value = pNode->d->d;
												pNode->d->d = value * (0.05* gauss_dist()+1); 
												#ifdef DEBUG_ERC_MUTATE 
													printf("ERC  init_value = %f   new value = %f\n", value, pNode->d->d);
												#endif
										}
								}//end else
				}

			}//end parameter mutation



			//Now copy the mutated individual to the new population
			/* ...and reproduce it into the new population. */
			duplicate_individual ( (newpop->ind)+newpop->next, (oldpop->ind)+p );
			newpop->ind[newpop->next].flags = FLAG_NONE;


			//HJJS  structure niching: mutation
			//Assumption: we think any structure mutation will create a new structure
			//for numeric mutation, we won't change the structure
			globaldata* g = get_globaldata();
			CStruct* pSt;

			int structID = newpop->ind[newpop->next].structID;
			pSt = g->pStructHash->find(structID);

			if(pSt==NULL){ //make sure,though redudency code
				pSt = new CStruct();
				pSt->structID = ++(g->currMaxStructID);
				pSt->age=1;
				pSt->lastGen=g->current_generation+1;
				pSt->nIndividuals=0;
				pSt->nIndNextGen=1;
				newpop->ind[newpop->next].structID=pSt->structID;
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

			++newpop->next;

			#ifdef DEBUG_ERC_MUTATE
				 printf ( "ERC MUTATION COMPLETE.\n\n\n" );
			#endif
}


