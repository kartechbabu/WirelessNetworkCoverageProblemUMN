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

#include "..\Gpkernal\lilgp.h"
#include <sys/types.h>
#include <sys/timeb.h>
#include "..\GraphLib\Prim.h"
using namespace std;
#define APP_SAVE
#define EXPORT_GW_LEDA

app_data ad;		//application data: global_data
extern int seed;// random seed used in this experiment run
void embryo_graph(); 

//keep all available multiple fitness functions array	HJJ
vector<CMultiFitness> FitnessFunctions;
//fitness mapping array, mapping subpop to its corresponding fitness function
vector<CMultiFitness> FitnessMapping;


int app_build_function_sets ( void )
{
     function_set fset;
     user_treeinfo tree_map;
     function sets[] = 
     { { f_Tree,	NULL,		NULL,		2, "Tree",	FUNC_EXPR, -1, 0, OP_ROOT, {OP_EDGE,OP_NODE,0,0,0,0} },
	   { f_insertV,	NULL,		NULL,		5, "insV",	FUNC_EXPR, -1, 1, OP_EDGE, {OP_EDGE,OP_NODE,OP_EDGE,OP_REAL, OP_REAL,0} },
//	   { f_cutV,	NULL,		NULL,		0, "cutV",	TERM_NORM, -1, 2, OP_NODE, {0,0,0,0,0,0} },
//	   { f_cutE,	NULL,		NULL,		2, "cutE",		FUNC_EXPR, -1, 3, OP_EDGE, {OP_NODE,OP_NODE,0,0,0,0} },
//	   { f_parallelE,	NULL,		NULL,		2, "parallE",		FUNC_EXPR, -1, 4, OP_EDGE, {OP_EDGE,OP_EDGE,0,0,0,0} },
	   { f_attachV,	NULL,		NULL,		5, "attV",		FUNC_EXPR, -1, 5, OP_NODE, {OP_NODE,OP_EDGE,OP_NODE,OP_REAL,OP_REAL,0} },
//		{ f_splitVL,	NULL,		NULL,		6, "splitVL",		FUNC_EXPR, -1, 6, OP_EDGE, {OP_EDGE,OP_EDGE,OP_NODE,OP_EDGE, OP_REAL,OP_REAL} },	
//		{ f_splitV,	NULL,		NULL,		6, "splitV",		FUNC_EXPR, -1, 7, OP_EDGE, {OP_EDGE,OP_EDGE,OP_NODE,OP_EDGE,OP_REAL, OP_REAL} },
    { f_endV,	NULL,		NULL,		0, "endV",		TERM_NORM, -1, 8,OP_NODE, {0,0,0,0,0,0} },
	  { f_endE,	NULL,		NULL,		0, "endE",		TERM_NORM, -1, 9,OP_EDGE, {0,0,0,0,0,0} },

		 { NULL,		f_erc_gen,	f_erc_print,0, "erc",		TERM_ERC,  -1, 10,OP_REAL, {0,0,0,0,0,0}},
	   { f_add,		NULL,		NULL,		2, "+",			FUNC_DATA, -1, 11, OP_REAL, {OP_REAL,OP_REAL,0,0,0,0} },
       { f_sub,		NULL,		NULL,		2, "-",			FUNC_DATA, -1, 12,OP_REAL, {OP_REAL,OP_REAL,0,0,0,0} },
       { f_mul,		NULL,		NULL,		2, "*",			FUNC_DATA, -1, 13,OP_REAL, {OP_REAL,OP_REAL,0,0,0,0} },//HJJM
       { f_div,		NULL,		NULL,		2, "/",			FUNC_DATA, -1, 14,OP_REAL, {OP_REAL,OP_REAL,0,0,0,0} },//HJJM

//     { f_sin ,	NULL,		NULL,		1, "sin",		FUNC_DATA, -1, 15,4, {4,0,0,0,0,0} },//HJJM
//     { f_cos ,	NULL,		NULL,		1, "cos",		FUNC_DATA, -1, 16,4, {4,0,0,0,0,0} },//HJJM
//     { f_exp ,	NULL,		NULL,		1, "exp",		FUNC_DATA, -1, 17,4, {4,0,0,0,0,0} },//HJJM
//     { f_rlog,	NULL,		NULL,		1, "logr",		FUNC_DATA, -1, 18,4, {4,0,0,0,0,0} },//HJJM
//	   { f_loop_start,NULL,		NULL,		1, "loop_start",FUNC_EXPR, -1, 15,OP_NODE, {OP_NODE,0,0,0,0,0} },	// site
//	   { f_loop_end,NULL,		NULL,		1, "loop_end",FUNC_EXPR, -1, 16,OP_NODE, {OP_NODE,0,0,0,0,0} },	// site
//	   { f_ins_Selfloop,NULL,		NULL,		4, "self_loop",	FUNC_EXPR, -1, 21,OP_JUNC, {2,1,2,1} },	// site

	 };
//     { code,		ephem_gen,	ephem_str,  arity,string,   type,evaltree,index,return_type,argument_type} 

     fset.size = 10;//HJJM
     fset.cset = sets;
     tree_map.fset=0;
     tree_map.return_type=0;
     tree_map.name = "TREE";
     return function_sets_init ( &fset, 1, &tree_map, 1 );
}


static  evaluatingTimes =0;//

void app_eval_fitness ( individual *ind )
{
		app_before_evaluate_individual();

		globaldata* g = get_globaldata();//get the lists
		(*(FitnessMapping[g->current_population].app_eval_fitness))(ind);

		app_end_of_evaluate_individual();
}


//APPCODE:  define your fitness function here
void app_eval_fitness_parameter ( individual *ind )
{

	globaldata* g = get_globaldata();//get the lists
	app_data* app = get_appdata();


	g->TotalEvalTimes ++;

	evaluatingTimes++;
	//get the global data structure
	//printf("current eval pop =%d  parameter fitness\n", g->current_population);


	int wrtmp = 0;
	double finalfitness;
  char *param;   
	int pop_size;


	set_current_individual ( ind );

  // --- reset the simulation variables
	embryo_graph();	//in app_eval_fitness_parameter


	//output the embryo graph
	if(evaluatingTimes==1){
		app->report();
		printf("\nEmbryo graph created.........................\n");
	}

  farg arg;
	arg.writeHead = 0;
  evaluate_tree ( ind->tr[0].data, 0, arg );

	//report the evolved bondgraph
	//g->report();	
	//calculate fitness

	double numEdges = app->getNumEdges();
	double numVertex = app->getNumVertices();

	//***************************************************************
	//Fitness for evolving a tree with 8 nodes.
	/*
  finalfitness = numEdges;
	finalfitness = 1/(1.0+abs(numEdges-numVertex));
	finalfitness = 1/(1+abs(7-numEdges));
	if(numVertex!=8) finalfitness =0 ;
	if(g->getMaxVertexDegree()>2) finalfitness = finalfitness/2;
	if(!g->isConnected()) finalfitness=finalfitness/4 ;
  */


	//***************************************************************
	//Fitness for wireless acess point configuration problem

	int uncoveredClients=0;
	//test how many clients are not covered
	for(int i=0;i<app->pClientGraph->getNumVertices();i++){
		string vName = GraphUtils::getVertexName(*(app->pClientGraph),i);
		Vertex* pClient = app->pClientGraph->getVertex(vName);
		double minDis = 9999999;
		for(int ii=0;ii<app->getNumVertices();ii++){
			Vertex* pV = app->getVertex(GraphUtils::getVertexName(*app,ii));
			double distance = (pV->getX()- pClient->getX())*(pV->getX()- pClient->getX()) +
						(pV->getY()-pClient->getY())*(pV->getY()-pClient->getY());
			if(distance !=0) distance = sqrt(distance);
			if(distance<minDis)
				minDis = distance;
		}
		if (minDis>app->maxServiceRadius){
				uncoveredClients++;
		}
	}

	//penalty of the total length of edges
	double wireCost =  app->getSumWeight();
	if(wireCost==0) wireCost=1.0;

	const vector<Vertex*>& vetexList = app->getVertices();
	for(int i=0;i<app->getNumVertices();i++){
		for(int j=0;j<app->getNumVertices();j++){
			if(vetexList[i]==vetexList[j]) continue;
			Vertex* pV1 = vetexList[i];
			Vertex* pV2 = vetexList[j];
			//std::cout<<i<<"  "<<j<<"*********\n"<<pV1->writeHead<<" "<<pV2->writeHead<<"\n";
			double distance = (pV1->getX()- pV2->getX())*(pV1->getX()- pV2->getX()) +
						(pV1->getY()-pV2->getY())*(pV1->getY()-pV2->getY());
			//we can only operate on the adjacency matrix to calculate wireCost of MST
			double** adjMatrix = app->getMatrix();
			adjMatrix[i][j]=distance;
		}
	}


	//construct the minimal spanning tree and calculate the connecting wire length.
	Prim MSTFinder(app);
	MSTFinder.CalculateMinimumSpanningTree();
	wireCost=MSTFinder.getCost();


	//numer of covered clients
	double nCoveredClients = app->pClientGraph->getNumVertices()-uncoveredClients;

	//cost of building each relay station
	double stationCost = app->getNumVertices();
	int clientsNum = app->pClientGraph->getNumVertices();

	double coverFit = 4.0*(double)nCoveredClients/(nCoveredClients+clientsNum);
	double wireFit = 10000.0/(10000.0+wireCost);
	double stationFit = (clientsNum/1.5)/(clientsNum/1.5+stationCost);

	//cout<<"wireCost: "<<wireCost<<"  stations: "<<stationCost<<endl;
	//cout<<coverFit<<"  "<<wireFit<<"  "<<stationFit<<endl;
	//total fitness value
	finalfitness= 0.7*coverFit+0.1*wireFit+0.2*stationFit;

	//penalty if not connected
	if(!app->isConnected()) finalfitness=finalfitness/2 ;
	app->clean();

	ind->hits = 0;
	ind->r_fitness = 0;
	ind->s_fitness = nCoveredClients;//The objective raw fitness of an inidividual
	ind->a_fitness = finalfitness;//Can be adjusted later by (Structure) Fitness Sharing methods
	ind->evald = EVAL_CACHE_VALID;
}


//stucture fitness evalution function
void app_eval_fitness_struct( individual *ind )
{
	app_eval_fitness_parameter(ind);
}


//some operation after evaluate each individual
void app_before_evaluate_individual(void){
	//For loop construction
	globaldata* g = get_globaldata();//get the lists
	g->StartConnectSiteList.clear();
	g->EndConnectSiteList.clear();
}




//Connecting distant vertices here!
void app_end_of_evaluate_individual(void){
	//here we connect the junctions which is labelled as loop_start, loop_end
	//one by one
	globaldata* g = get_globaldata();//get the lists
	app_data* app = get_appdata();

	if( g->StartConnectSiteList.empty() ||g->EndConnectSiteList.empty())
		return;

	int startLoopSites = g->StartConnectSiteList.size();
	int endLoopSites = g->EndConnectSiteList.size();
	int size = startLoopSites>endLoopSites? endLoopSites:startLoopSites;

	Vertex* pVStart, *pVEnd;

	//for each pair of startloop modifiable site and endloop site, we connect them
	for(int i=0;i<size;i++){
		pVStart = app->getVertex((g->StartConnectSiteList)[size-1-i]);
		pVEnd = app->getVertex((g->EndConnectSiteList)[i]);
		if(pVStart==NULL|| pVEnd ==NULL) continue;
		if(pVStart==pVEnd) continue;	//Don't allow self-loop
		if(app->getEdge(pVStart->getTitle(),pVEnd->getTitle())!=NULL)
			continue;	//already exists this edge!
		Edge* pE = app->connectVertex(pVStart,pVEnd);
		pE->setWeight(pE->getLength());
	}

}



int app_end_of_evaluation ( int gen, multipop *mpop, int newbest,
                           popstats *gen_stats, popstats *run_stats )
{
	farg arg;
	char* param;
	globaldata* g = get_globaldata();//get the lists
	app_data* app = get_appdata();


	//decide if there is progress
	if(newbest==0){
		printf("-:( Bad!   no progress in this generation...........\n\n");
	}
	else{//there is an improvement
		//get the global data structure
		printf("-:) Aha, Improved!.....................\n");
		printf("Best solution of this generation\n");
	}


	//report the best Bondgraph of this generation
	//anyhow, we evaluate the best bondgraph of this generation, 
	//even if it is the same as the last generation
	embryo_graph();	//app_end_of_evaluation
	//evaluate the tree to get the bond topology  
	app_before_evaluate_individual();
	arg.writeHead = 0;
	
	//printf("evaluate_tree in app_end_of evaluation\n");
	evaluate_tree ( run_stats[0].best[0]->ind->tr->data, 0, arg );
 	//g->writeHead=0;// number of used Writehead.. for writehead numbering

	app_end_of_evaluate_individual();
	app->report();
	g->NewBestFitness = run_stats[0].best[0]->ind->a_fitness;


#ifdef APP_SAVE
		//save the result to file
		fprintf ( g->f_result, "**************  Generation: %d ************\n", gen );
		//save the topology
		if(newbest== 0){
			fprintf ( g->f_result, "	No Progress\n", gen_stats->size);
		}
		else{
			fprintf ( g->f_result, "Popsize: %d\n", gen_stats->size);


	//export the minimal spanning tree
	app->removeEdges();
	const vector<Vertex*>& vetexList = app->getVertices();
	//construct complete graph
	for(int i=0;i<app->getNumVertices();i++){
		for(int j=0;j<app->getNumVertices();j++){
			if(vetexList[i]==vetexList[j]) continue;
			Vertex* pV1 = vetexList[i];
			Vertex* pV2 = vetexList[j];
			//std::cout<<i<<"  "<<j<<"*********\n"<<pV1->writeHead<<" "<<pV2->writeHead<<"\n";
			double distance = (pV1->getX()- pV2->getX())*(pV1->getX()- pV2->getX()) +
						(pV1->getY()-pV2->getY())*(pV1->getY()-pV2->getY());
			app->addEdge(pV1,pV2,distance);
		}
	}
	
	//find MST
	Prim MSTFinder(app);
	MSTFinder.CalculateMinimumSpanningTree();
	//double wireCost=MSTFinder.getCost();
	const vector<EDGE>& edgeSet=MSTFinder.getMinimumSpanningTree();
	app->removeEdges();

	//construct MST graph
	for(int i=0;i<edgeSet.size();i++){
			Vertex* pV1 = vetexList[edgeSet[i].first];
			Vertex* pV2 = vetexList[edgeSet[i].second];
			//std::cout<<i<<"  "<<j<<"*********\n"<<pV1->writeHead<<" "<<pV2->writeHead<<"\n";
			double distance = (pV1->getX()- pV2->getX())*(pV1->getX()- pV2->getX()) +
						(pV1->getY()-pV2->getY())*(pV1->getY()-pV2->getY());
			app->addEdge(pV1,pV2,distance);
	}

	//APPCODE
	//add the client graph into the relay station graph
	for(int i=0;i<app->pClientGraph->getNumVertices();i++){
		string vName = GraphUtils::getVertexName(*(app->pClientGraph),i);
		Vertex* pClient = app->pClientGraph->getVertex(vName);
		Vertex* pNewV = app->addVertex(vName);
		pNewV->setPos(pClient->getX(),pClient->getY());
	}
	char fn[20];
	int index = app->nGraphOutputed;


	//save the evolved graph if improved
	string fileName;
	param = get_parameter ( "output.basename" );
	if(param==NULL)
		fileName = "BestGraph";
	else
		fileName = param;
	fileName = fileName+"";

	if(index!=0)
		fileName = fileName+itoa(index,fn,10);

#ifdef EXPORT_GW_LEDA
	string gwfn(fileName+".gw");
	ofstream gwFile(gwfn.data());
	app->printGW(gwFile);
	gwFile.close();
#else
	string gdlfn(fileName+".gdl");
	app->printGDL(gdlFile); //
	ofstream gdlFile(gdlfn.data());
	gdlFile.close();
#endif

	app->nGraphOutputed++;
	app->clean();
	}
#endif

	return 0;		// temporalily
}



void app_end_of_breeding ( int gen, multipop *mpop )
{
     return;
}



int app_create_output_streams ( void )
{
     return 0;
}


int app_initialize ( int startfromcheckpoint )
{
	//get the global data structure
	globaldata* g = get_globaldata();//get the lists
	app_data* app = get_appdata();
	char* param;
	
	char clientGraphFile[50];
	
	param = get_parameter ( "app.clientGraphFile" );
	if(param==NULL)
			strcpy(clientGraphFile ,"client.txt");
	else
			strcpy(clientGraphFile, param);


	//APPCODE
	//load client graph
	app->pClientGraph = new UndirectedGraph(10);
	app->pClientGraph->load(clientGraphFile);
	cout<<" client distribution map\n";
	app->pClientGraph->report();

	//Get watchInterval
	param = get_parameter ( "app.watchInterval" );
	if(param==NULL)
		app->watchInterval=5;// 5 generations
	else{
		int i=atoi(param);
		if(i!=0)
			app->watchInterval=i;
		else
			app->watchInterval=5;
		printf("\n\n Save the evolved graph every %d generations\n", app->watchInterval);
	}

	//Get graphsize
	param = get_parameter ( "app.maxServiceRadius" );
	if(param==NULL)
		app->maxServiceRadius=200;// 5 generations
	else{
		int i=atoi(param);
		if(i!=0)
			app->maxServiceRadius=i;
		else
			app->maxServiceRadius=200;
	printf("\n\n maxServiceRadius=  %d \n", app->maxServiceRadius);
	}

	//time information
	char ampm[] = "AM";
	time_t ltime;
	int indiv =0;					// count individual in app_eval_fitness()
	g->generateNodeType = 0;// for generating root node with f_TREE function
	char basename[50];
  strcpy(basename, get_parameter ( "output.basename" ));;

//save result
#ifdef APP_SAVE
	g->f_result = fopen(strcat(basename,".egn"),"w+");

    if ( g->f_result == NULL )
	{
	  error ( E_FATAL_ERROR, "couldn't open file: \"%s\".",
			 strcat("basename",".egn") );
	}
	fprintf(g->f_result, "Bondgraph Based System Synthesis by Genetic Programming\n\n");
	fprintf(g->f_result, "www.DarwinInvention.com   (contact: hujianju@msu.edu)\n");
    fprintf(g->f_result, "Genetic Algorithm Research & Application Group\nMichigan State University\nEast Lansing, MI, 48823\n\n");
	fputs ( "Graph Evovlving problem experiment\n", g->f_result );
   
	//date and time info
	//Use time structure to build a customized time string.
    time( &ltime );
	//today = localtime( &ltime );
  //Use strftime to build a customized time string.
  //strftime( tmpbuf, 128,"Experiment Date: %A, %B - %d - %Y.\n", today );
  //fprintf(g->f_result, tmpbuf );
    fprintf(g->f_result, "Experiment time: %s", ctime( &ltime ) );

	//target graph
	fprintf(g->f_result, "------  %d graph evolving problem --------\n", app->watchInterval);
	fprintf(g->f_result, "random seed = %d \n", seed);
#endif

//****************************************************************************
//multiple fitness function feature  HJJ
	int nSubPopulations=1;
	param = get_parameter ( "multiple.subpops" );
	nSubPopulations = atoi ( param );

	//register all app_eval_fitness functions
	//this first one(0) is the default fitness evaluation function
	FitnessFunctions.push_back( CMultiFitness(app_eval_fitness_parameter, 0));
	FitnessFunctions.push_back( CMultiFitness(app_eval_fitness_parameter, 1));
	FitnessFunctions.push_back( CMultiFitness(app_eval_fitness_parameter, 2));
	FitnessFunctions.push_back( CMultiFitness(app_eval_fitness_parameter, 3));

	//build the fitness mapping
	int funcIndex;
	for(int i=0;i<nSubPopulations;i++){
		char buffer[30];
		sprintf(buffer,"subpop[%d].fitness",i+1);

		param = get_parameter ( buffer);//HJJM  strcat("multiple.fitness.subpop.",buffer));
		if(param == NULL){ // no specified fitness function, use the default app_eval_fitness
			FitnessMapping.push_back(FitnessFunctions[0]);
		}
		else{
			funcIndex = atoi(param);
			if(funcIndex<FitnessFunctions.size())//valid evaluate_function
				FitnessMapping.push_back(FitnessFunctions[funcIndex]);
			else								// non valid, use default
				FitnessMapping.push_back(FitnessFunctions[0]);
		}
	}


//report the embryo bondgraph
	embryo_graph();
	printf("\n---------------------------Embryo graph-------------------------------\n");
	app->report();

	#ifdef APP_SAVE		
	fprintf(g->f_result, "\n---------------------------Embryo graph----------------------\n");
	#endif

	printf("-----------------------------------------------------------\n\n");
	app->clean();
	return 0;
}



void app_uninitialize ( void )
{
	globaldata* g = get_globaldata();//get the lists
	#ifdef APP_SAVE
	fclose(g->f_result);
	#endif
	printf("end of evolution..... uninitialize\n");
    return;
}



void app_write_checkpoint ( FILE *f )
{
    return;
}



void app_read_checkpoint ( FILE *f )
{
     return;
}


// Set up the embryo graph, and especially the modifiable site
// this function will be called each time when the GP tree is evaluated to produce one graph.
void embryo_graph (void) {
  //get the global data structure
	globaldata* g = get_globaldata();//get the lists
	app_data* app = get_appdata();
	//Clear the content of list evolved by the last individual program of GP
	app->clean();
	app->setNumVertices(2);
	app->Initialize();
	app->iniNumWriteHead = 2;
	
	Vertex* SV = app->addVertex("1");	//add the source node
	SV->setPos(0,0);
	Vertex* pV = app->addVertex("2");
	pV->setPos(10,10);
	pV->writeHead=1;

	Edge* pE = app->addEdge("1", "2", 0);
	pE->setWeight(pE->getLength());
	pE->writeHead =0;


	g->StartConnectSiteList.clear();
	g->EndConnectSiteList.clear();
	g->m_bRefreshed = false;
	g->writeHead=2;

	//ofstream of("test.gw");
	//g->printGW(of);
	//g->report();
}