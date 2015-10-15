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
 *	Hiearchical Fair Competition Model 
 *	  Devised by:  Jianjun Hu during 03/01-04/12/01
 *	  Implmented:  04/12/01-04/22/01
 *    Department of Computer Science & Enginnering
 *	  Michigan State Univerisity
 *		East Lansing, MI, 48823
 *	  hujianju@msu.edu
*/



#ifndef _TYPES_H
#define _TYPES_H
using namespace std;

#include <vector>
#include <string>
#include ".\Hashtable\HashTable.h"
typedef struct
    {
    int fset;			/* function set for tree */
    int return_type;		/* return type for tree */
    char* name;			/* tree name */
    } user_treeinfo;


/* holds information about one function (or terminal). */
//HJJ

struct farg;


typedef struct
{
     DATATYPE (*code)( int tree, farg *args, int arity);
     void (*ephem_gen)( DATATYPE *r);
     char * (*ephem_str)(DATATYPE d);
     int arity;
     char *string;
     int type;
     int evaltree;
     int index;
     /* Added to support strong data typing */
     int return_type;
     int argument_type[MAXARGS];
} function;

typedef struct
{
     function *cset;
     int function_count;
     int terminal_count;
     int num_args;
     int size;
     /* Added to support strong data typing */
     function* cset_by_type[NUMTYPES];
     int function_count_by_type[NUMTYPES];
     int terminal_count_by_type[NUMTYPES];
} function_set;

/* holds one ERC.  contains the data, reference count, and pointer for
   building a linked list. */

typedef struct _ephem_const
{
     int refcount;
     struct _ephem_const *next;
     DATATYPE d;
     function *f;
} ephem_const;

/* the basic building block of the tree structure.  can be a function pointer,
   a skip value, or a pointer to an ERC. */

typedef union
{
     int s;
     function *f;
     ephem_const *d;
} lnode;

/* one tree -- consists of an array of lnodes.  the size and node counts are
   cached here for speed improvement. */

typedef struct _tree
{
     lnode *data;
     int size;         /* the lnode count */
     int nodes;        /* the actual node count */
} tree;

/* the arguments passed to the function (terminal) code.  can be either a
   value or a pointer to a tree. */

struct farg

{

   DATATYPE d;
   lnode *t;

	 //APPCODE HJJ
	 //Graph evolving
	 int writeHead;
};



/* one individual.  holds the expression, fitness values, etc. */

typedef struct
{
     tree *tr;
     double r_fitness;
     double s_fitness;
     double a_fitness;
     int hits;
     int evald;
     int flags;

	 //HJJS for structure niching technique: individual
	 unsigned long structID;
} individual;

/* struct for doing a binary search of successive real-valued intervals. */

typedef struct
{
     double fitness;
     int index;
} reverse_index;

/* one population -- an array of individuals, and some global info. */

typedef struct
{
     individual *ind;
     int size;
     int next;
} population;


struct breedphase;



//HJJ begin

struct sel_context;

//define function pointer types for select method

typedef int (*select_func_ptr)(sel_context *sc );

//define function pointer types for select context function

typedef sel_context * (*select_context_func_ptr)(

	int op,sel_context *sc,population *p,char *string );


struct sel_context
{
     population *p;
     select_func_ptr select_method;
     select_context_func_ptr context_method;
     void *data;
};

//HJJ end



typedef struct
{
     char *name;
     select_context_func_ptr func;
} select_method;

//HJJ

typedef struct

{

     char *name;

     int (*func)(char *options, breedphase *bp);

} Operator; //HJJ change operator to Operator



typedef struct
{
     int copywhole;
     int * from;
     int * as;
     char ** fromsc;
     int to;
     char * tosc;
     int count;
} exchange;

//HJJ typedef struct

struct breedphase
{
     int operatorID;
     double rate;
     void *data;
     void (*operator_free)( void *data);
     void (*operator_start)(population *oldpop, void *data);
     void (*operator_end)( void *data );
     void (*operator_operate)(population *oldpop, population *newpop,

                         void *data );
};


//HJJB*******************************************************
//for HFCPM
class CSubpopIndividual{
public:
	CSubpopIndividual(int subpop, int ind){
		subpopID = subpop;
		individualID = ind;
	};

	virtual~CSubpopIndividual(){
	};

	int subpopID;		//this individual is belonging to subpopID's
	int individualID;	// individualID
};



//define the structure of each level in the HFCPM hiearchy
class CLevel{
public:
	CLevel(double fitness){
		level = 0;
		admission_fitness = fitness;
	};

	virtual~CLevel(){
	};
	int level;//this is the levelth level in the hiearchy
	double admission_fitness;//admission level of this level
	vector<int> subpop_list;//which subpopulations belongs to this level
};

//define the HFCPM fitness hiearchy topology info here
class CHFCPMHierarchy: public vector<CLevel*>{
public:
	CHFCPMHierarchy(){
	};
	virtual~CHFCPMHierarchy(){
	};
	int bAdaptive;//Adaptive HFC: determine the admission fitness adaptively
	double adaptationRate;//how much the highest admission fitness be adjusted
						  //new = old * (1-adapationRate) + new * adaptationRate
};

//HJJE*******************************************************

struct multipop
{
	virtual ~multipop(){
//		delete pHF;
	};
     int size, exchanges;
	 CHFCPMHierarchy* pHF;// exchange hierarchy HFCPM
	 int* subpopActiveFlag;//indicate if we evaluate and change this subpopulation or not
						   //if we don't import any qualified individuals yet, then we
						   //don't evaluate and do crossover.
     population **pop;
     exchange *exch;
     breedphase **bpt;
} ;

typedef struct
{
     double total;
     reverse_index *ri;
     int count;
} interval_data;

typedef struct
{
     int i;
     ephem_const *e;
} ephem_index;
     
typedef struct _parameter
{
     char *n;
     char *v;
     int copyflags;
} parameter;

typedef struct _saved_ind
{
     individual *ind;
     int refcount;
     struct _saved_ind *next;
} saved_ind;

typedef struct _popstats
{
     int size;
     int maxnodes, minnodes, totalnodes, bestnodes, worstnodes;
     int maxdepth, mindepth, totaldepth, bestdepth, worstdepth;
     int maxhits, minhits, totalhits, besthits, worsthits;
     double bestfit, worstfit, totalfit;
	 double varfit;//HJJ variance of fitnesses in a subpopulation
     int bestgen, worstgen;
     int bestpop, worstpop;
     int bestn;
     saved_ind **best;
} popstats;

typedef struct 
{
     lnode *data;
     int size, used;
} genspace;

typedef struct
{
     int fset;
     int nodelimit;
     int depthlimit;
     farg *arguments;
     int argtype;
     int evaluatedfrom;
     char *name;
     /* Added to support typing */
     int return_type;
} treeinfo;


class globaldata
{
public:
  /* Included to patch the current_individual bug...leave this alone */
	individual* current_individual;
	/* End patch */

	//current evalution subpopulation
	//used for multiple fitness functions for subpopulations HJJ
	int current_population;
	int current_generation;

	//crossover times of each type of nodes in a generation
	int CrossoverTimes ;//Total crossover times
	int SelTimesRoot ;
	int SelTimesEdge ;
	int SelTimesReal ;
	int SelTimesNode ;
	int TotalEvalTimes; // how many evalutions of individual used

	//Generation Statistics
	int GenNoProgress;
	double OldBestFitness, NewBestFitness;
	int match_pairs, not_match_pairs;//select two types match times
	int copy_times, update_times; //copy two parents and updates

	//Evolution Control strategy
	bool bUseStructParamEvoAlternation;
	bool bProbabilisticSPA;//probabilistic structure and parameter evolution alternation
	bool bUseHFC;
	bool bUseSFS;
	bool bAdaptiveHFC;//use adaptive admission fitness determination
	bool m_bRefreshed;
	bool saveHistoryData;

	//Structure and parameter search!
	double probStructEvoStage;
	bool IsStructEvoStage;
	int maxParamEvoGen;  //20how many generation of parameter evolution after structure evolution
	int maxStructEvoGen; //5how many generation of structure evolution after parameter evolution
	int currParamEvoGen;//counter of paramEvoGen
	int currStructEvoGen;//counter of structEvoGen

	//generating loops!
	vector<int> StartConnectSiteList;
	vector<int> EndConnectSiteList;

	//time statistics
	clock_t startTime, finishTime, initialTime;
	double  duration;
	int startFlag;

	//Fitness type.
	int generateNodeType;// root node for generate tree  0 : first time, 1 : next time
	int fitnessType;

	//save to file
	FILE *f_result;//save the output of eigenvalue

	//HJJS 	Structure Niching (Structure sharing: definition of hashtable
	unsigned long currMaxStructID;
	//currently maximal structID, each time we create a new structure
	//we increase this variable by one
	HashTable* pStructHash;

	//what is the max capacity of hashtable
  //how many structures allowed to search simultaneously in the hashtable
	int maxStructNo;
	int debug_output;

	//APPCODE HJJ
	//Global writehead information
	int writeHead;		//used for generating new edge/vertex identifiers.
	//APPCODE END


	//**************************************************************************
	//functions	
	~globaldata(){
		delete pStructHash;
	};

	globaldata(){
			pStructHash = new HashTable(1000);
			current_population = 0;//default the 0th population
			maxStructNo = 1200;

			bUseSFS = false;
			bUseStructParamEvoAlternation = false;
			bProbabilisticSPA=false;
			///Initially, more structure search to find good topology 
			probStructEvoStage = 0.2;
			IsStructEvoStage=true;
			maxParamEvoGen=10;  //20how many generation of parameter evolution after structure evolution
			maxStructEvoGen=10; //5how many generation of structure evolution after parameter evolution
			currParamEvoGen = 0;//counter of paramEvoGen
			currStructEvoGen= maxStructEvoGen;//counter of structEvoGen

			//crossover times of each type of nodes in a generation
			CrossoverTimes =0;
			SelTimesRoot =0;
			SelTimesEdge =0;
			SelTimesNode =0;
			SelTimesReal =0;
			TotalEvalTimes=0;

			GenNoProgress=0;
			//Generation Statistics
			OldBestFitness=0.0, NewBestFitness=0.0;
			match_pairs=0, not_match_pairs=0;	//select two types match times
			copy_times=0, update_times=0;		//copy two parents and updates

			//time statistics
			duration=0;
			startFlag=0;
			debug_output =0;

			//global write head information
			//APPCODE
			writeHead =0;		// used for generating new edge/vertex identifiers.
			generateNodeType=0;	// number of used Writehead.. for writehead numbering
			
			// root node for generate tree  0 : first time, 1 : next time
			f_result = NULL;//save the output of eigenvalue
			saveHistoryData=true;
			currMaxStructID=0L;
	};
};




#endif



