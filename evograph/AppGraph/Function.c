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

#include "..\GPKernal\lilgp.h"
#include "..\GraphLib\Graph.h"
#include <stdlib.h>

//#define DEBUG_F

app_data *get_appdata(void) {
  return( &ad );
}

DATATYPE f_Tree ( int tree, farg *args, int arity )
{
		globaldata* g = get_globaldata();

		//APPCODE HJJ
		if(args[0].writeHead != 0){
				cout<<"f_Tree"<<endl;
				oprintf(OUT_SYS, 30,"f_Tree: Wrong writehead value: %d in f_Tree!!!\n", args[0].writeHead);
				exit(-1);
		}
		else {
				//g->writeHead=args[0].writeHead;
				args[1].writeHead = args[0].writeHead+1;

				for (int i=0; i<arity; i++) {
					//we assume the modifiable sites on embryo graph starts from 0, 1, 2... HJJC
					evaluate_tree ( args[i].t, tree, args[i] );
				}

		}
	return 0.0;
}


//----------------------------------------------------------
//   O------->B    C is a randomly generated vertex
//==>
//	 O---C----B
//----------------------------------------------------------
DATATYPE f_insertV(int tree, farg *args, int arity)
{

		//Find the edge (the modifiable site)
		app_data* app = get_appdata();
		globaldata* g = get_globaldata();


		//get the modifiable site ID
		int modifySite = args[0].writeHead;

		Edge* pE = app->getEdge(modifySite);
		if( pE ==NULL) return -1;

		#ifdef DEBUG_F
			cout<<"f_insertV:   "<<pE->getFromVertex()<<"---> "<<pE->getToVertex()<<endl;
		#endif
	//allocate new writehead for new vertex and edge
		args[1].writeHead = ++g->writeHead;
		args[2].writeHead = ++g->writeHead;

		//Create a new vertex and edge, then link into the graph
		char vname[20];
		itoa(args[1].writeHead,vname,10);
		Vertex* pV = new Vertex(string(vname));

		pV->writeHead = args[1].writeHead;
		app->addVertex(pV);
		app->cutEdge(pE);
		//app->report();

		Vertex* toVertex = pE->getToVertexObject();
		pE->setToVertex(pV->getTitle());
		pE->setToVertexObject(pV);
		app->addEdge(pE);

		Edge* pNewE = new Edge(pV, toVertex);
		pNewE->writeHead = args[2].writeHead;
		app->addEdge(pNewE);


		//Calculate the numeric subtree to establish the x, y cooridinates 
		//for new vertex.

		//evaluate all the children, considering ERC child
		double erc_value;
		erc_value = calculate_erc(evaluate_tree(args[3].t,tree, args[3]));
		pV->setX(erc_value);
		erc_value = calculate_erc(evaluate_tree(args[4].t,tree, args[4]));
		pV->setY(erc_value);
		for(int i=0;i<arity;i++){
				if(i != 3 &&i!=4)
					evaluate_tree(args[i].t, tree, args[i]);
		}
		pE->setWeight(pE->getLength());
		pNewE->setWeight(pNewE->getLength());
		
		#ifdef DEBUG_F
			cout<<"after f_insertV "<<" New V: "<<pV->getTitle()<<endl;
			app->report();
		#endif

		return 0.0;
}

//----------------------------------------------------------
//   O------->B 
//==>
//	 O
//----------------------------------------------------------

//HJJQ  cut vertex may cause invalid modifiable site problem!
//Not used.
DATATYPE f_cutV(int tree, farg *args, int arity)
{

		//get the modifiable site ID
		globaldata* g = get_globaldata();
		app_data* app = get_appdata();

		int modifySite = args[0].writeHead;

		if( args[0].writeHead < app->iniNumWriteHead)
			return 0.0;	//we don't cut embryo graph!

		//Find the vertex (the modifiable site)
		Vertex* pV = app->getVertex(modifySite);

		#ifdef DEBUG_F
			cout<<"f_cutV: "<< pV->getTitle()<<endl;
			app->report();
		#endif

		if(pV==NULL){
			printf("f_cutV: Wrong Modifiable site: %d\n", modifySite);
			exit(-1);
		}
		app->cutVertex(pV->getTitle());
		#ifdef DEBUG_F
			cout<<"After f_cutV: "<< pV->getTitle()<<endl;
			app->report();
		#endif
		delete pV;
		return 0.0;
}

//----------------------------------------------------------
//   O------->B is a randomly generated vertex
//==>
//	 O        B
//----------------------------------------------------------
DATATYPE f_cutE(int tree, farg *args, int arity)
{
		//Find the edge (the modifiable site)
		globaldata* g = get_globaldata();
		app_data* app = get_appdata();

		//get the modifiable site ID
		int modifySite = args[0].writeHead;
		Edge* pE = app->getEdge(modifySite);
		if( pE ==NULL) return -1;

		#ifdef DEBUG_F
			cout<<"f_cutE:"<<pE->getFromVertex()<<"-->"<<pE->getToVertex()<<endl;
		#endif
		app->cutEdge(pE);
		//allocate writehead for new vertex and edge
		args[0].writeHead = pE->getFromVertexObject()->writeHead;
		args[1].writeHead = pE->getToVertexObject()->writeHead;
		
		for(int i=0;i<arity;i++){
					evaluate_tree(args[i].t, tree, args[i]);
		}
	
		return 0.0;
}

//----------------------------------------------------------
//   O------->B 
//==>
//	 O--------B
//	 |________|
//	 O'				B'
//----------------------------------------------------------
DATATYPE f_parallelE(int tree, farg *args, int arity)
{
	return 0.0;
}

//----------------------------------------------------------
//   O			  B 			
//=>  
//	 O--------B
//
//----------------------------------------------------------
DATATYPE f_attachV(int tree, farg *args, int arity)
{
		//Find the edge (the modifiable site)
		globaldata* g = get_globaldata();
		app_data* app = get_appdata();

		//get the modifiable site ID
		int modifySite = args[0].writeHead;

		Vertex* pV = app->getVertex(modifySite);
		if( pV ==NULL) return -1;

		//allocate writehead for new vertex and edge
		args[1].writeHead = ++g->writeHead;
		args[2].writeHead = ++g->writeHead;

		//Create a new vertex and edge, then link into the graph
		char vname[20];
		itoa(args[1].writeHead,vname,10);
		Vertex* pNewV = new Vertex(string(vname));
		pNewV->writeHead = args[2].writeHead;
		
		#ifdef DEBUG_F
			cout<<"f_attachV: "<<pV->getTitle()<<" New Vertex: "<<pNewV->getTitle()<<endl;
			app->report();
		#endif

		app->addVertex(pNewV);

		Edge* pNewE = new Edge(pNewV, pV);
		pNewE->writeHead = args[1].writeHead;
		app->addEdge(pNewE);

		//Calculate the numeric subtree to establish the x, y cooridinates 
		//for new vertex.

		//evaluate all the children, considering ERC child
		double erc_value;
		erc_value = calculate_erc(evaluate_tree(args[3].t,tree, args[3]));
		pNewV->setX(erc_value);
		erc_value = calculate_erc(evaluate_tree(args[4].t,tree, args[4]));
		pNewV->setY(erc_value);

		for(int i=0;i<arity;i++){
				if(i!= 3 &&i!=4)
					evaluate_tree(args[i].t, tree, args[i]);
		}
		pNewE->setWeight(pNewE->getLength());

		#ifdef DEBUG_F
			cout<<"After attach vertex: "<<pNewV->getTitle()<<"  to: "<<pV->getTitle()<<endl;
			app->report();
		#endif
		return 0.0;
}

//----------------------------------------------------------
//   O--------B			=>
//	 O--------B
//	 |________|
//	 C
//----------------------------------------------------------
DATATYPE f_splitVL(int tree, farg *args, int arity)
{

		//get the modifiable site ID
		int modifySite = args[0].writeHead;
		globaldata* g = get_globaldata();

		//Find the edge (the modifiable site)
		app_data* app = get_appdata();
		Edge* pE = app->getEdge(modifySite);
		if( pE ==NULL) return -1;
		#ifdef DEBUG_F
			cout<<"f_splitVL:  "<<pE->getFromVertex()<<"-->"<<pE->getToVertex()<<endl;
			app->report();
		#endif

		//allocate new writehead for new vertex and edge
		args[1].writeHead = ++g->writeHead;
		args[2].writeHead = ++g->writeHead;
		args[3].writeHead = ++g->writeHead;
		
		//Create a new vertex and edge, then link into the graph
		char vname[20];
		itoa(args[1].writeHead,vname,10);
		Vertex* pV = new Vertex(string(vname));
		app->addVertex(pV);
		pV->writeHead = args[2].writeHead;

		Vertex* toVertex = pE->getToVertexObject();
		Vertex* fromVertex = pE->getFromVertexObject();

		Edge* pNewE1 = new Edge(fromVertex, pV);
		app->addEdge(pNewE1); //to ensure the consistancy of adj matrix!
		pNewE1->writeHead = args[1].writeHead;

		Edge* pNewE2 = new Edge(pV, toVertex);
		pNewE2->writeHead = args[3].writeHead;
		app->addEdge(pNewE2);

		//Calculate the numeric subtree to establish the x, y cooridinates 
		//for new vertex.

		//evaluate all the children, considering ERC child
		double erc_value;
		erc_value = calculate_erc(evaluate_tree(args[4].t,tree, args[4]));
		pV->setX(erc_value);
		erc_value = calculate_erc(evaluate_tree(args[5].t,tree, args[5]));
		pV->setY(erc_value);

		for(int i=0;i<arity;i++){
				if(i != 4 && i!=5)
					evaluate_tree(args[i].t, tree, args[i]);
		}

		pNewE1->setWeight(pNewE1->getLength());
		pNewE2->setWeight(pNewE2->getLength());
		#ifdef DEBUG_F
			cout<<"after f_splitVL:  "<<pE->getFromVertex()<<"-->"<<pE->getToVertex()<<endl;
			app->report();
		#endif

		return 0.0;
}

//----------------------------------------------------------
//   O--------B------K			=>
//	 O--------B
//	  				|
//	          B'------K
//----------------------------------------------------------
DATATYPE f_splitV(int tree, farg *args, int arity)
{
		//Find the edge (the modifiable site)
		app_data* app = get_appdata();
		globaldata* g = get_globaldata();

		//get the modifiable site ID
		int modifySite = args[0].writeHead;
		Edge* pE = app->getEdge(modifySite);
		if( pE ==NULL) return -1;
		#ifdef DEBUG_F
			cout<<"f_splitV:  "<<pE->getFromVertex()<<"-->"<<pE->getToVertex()<<endl;
			app->report();
		#endif

		//allocate new writehead for new vertex and edge
		args[1].writeHead = ++g->writeHead;
		args[2].writeHead = ++g->writeHead;
		args[3].writeHead = ++g->writeHead;
		
		//Create a new vertex and edge, then link into the graph
		char vname[20];
		itoa(args[1].writeHead,vname,10);
		Vertex* pV = new Vertex(string(vname));
		app->addVertex(pV);
		pV->writeHead = args[2].writeHead;

		Vertex* toVertex = pE->getToVertexObject();
		Vertex* fromVertex = pE->getFromVertexObject();

		Edge* pNewE1 = new Edge(fromVertex, pV);
		app->addEdge(pNewE1); //to ensure the consistancy of adj matrix!
		pNewE1->writeHead = args[1].writeHead;

		Edge* pNewE2 = new Edge(pV, toVertex);
		pNewE2->writeHead = args[3].writeHead;
		app->addEdge(pNewE2);

		//Calculate the numeric subtree to establish the x, y cooridinates 
		//for new vertex.

		//evaluate all the children, considering ERC child
		double erc_value;
		erc_value = calculate_erc(evaluate_tree(args[4].t,tree, args[4]));
		pV->setX(erc_value);
		erc_value = calculate_erc(evaluate_tree(args[5].t,tree, args[5]));
		pV->setY(erc_value);

		for(int i=0;i<arity;i++){
				if(i != 4 && i!=5)
					evaluate_tree(args[i].t, tree, args[i]);
		}
		pNewE1->setWeight(pNewE1->getLength());
		pNewE2->setWeight(pNewE2->getLength());
		#ifdef DEBUG_F
			cout<<"after f_splitV:  "<<pE->getFromVertex()<<"-->"<<pE->getToVertex()<<endl;
			app->report();
		#endif

		return 0.0;
}




DATATYPE f_loop_start(int tree, farg *args, int arity)
{
	
	//get the global data
	globaldata* g = get_globaldata();
	app_data* app = get_appdata();
	//cout<<"before f_loop_start...............\n"<<endl;
	//app->report();

		//find the junction to which the capacitor will be added
		Vertex* pV = app->getVertex(args[0].writeHead);
		//check for wrong writehead into which cause memeory error
		if(pV==NULL){// couldn't find the juction with this writehead then report error
		oprintf(OUT_SYS, 30, "Improper writehead type in f_loop_start\n");
			printf("Error: In f_loop_start..Wrong writehead info, can't find the find the vertex with  writehead %d!\n",args[0].writeHead);
			cout<<args[0].writeHead;
			app->report();
			return -1;
		}
		//keep the modifiable sites of this junction into connection list
		g->StartConnectSiteList.push_back(args[0].writeHead);
		evaluate_tree(args[0].t, tree, args[0]);
	return 0.0;

}

DATATYPE f_loop_end(int tree, farg *args, int arity)
{
	//get the global data
	globaldata* g = get_globaldata();
	app_data* app = get_appdata();
	//cout<<"in f_loop_end............\n";
	//app->report();

		//find the junction to which the capacitor will be added
		Vertex* pV = app->getVertex(args[0].writeHead);
		//check for wrong writehead into which cause memeory error
		if(pV==NULL){// couldn't find the juction with this writehead then report error
		oprintf(OUT_SYS, 30, "Improper writehead type in f_loop_end\n");
			 print_tree(args[0].t,stdout);

			printf("Error: In f_loop_end..Wrong writehead info, can't find the find the vertex with  writehead %d!\n",args[0].writeHead);
			cout<<args[0].writeHead;
			app->report();
			return -1;
		}
		//keep the modifiable sites of this junction into connection list
		g->EndConnectSiteList.push_back(args[0].writeHead);
		evaluate_tree(args[0].t, tree, args[0]);
	  return 0.0;
}





//**********************************************************
//arithmetic functions
DATATYPE f_add ( int tree, farg *args, int arity )
{
     return args[0].d + args[1].d;
}

DATATYPE f_sub ( int tree, farg *args, int arity )
{
     return args[0].d - args[1].d;
}

DATATYPE f_mul ( int tree, farg *args, int arity )
{
     return args[0].d * args[1].d;
}

DATATYPE f_div ( int tree, farg *args, int arity )
{
    if (args[1].d >= -0.00000000001 || args[1].d <= 0.00000000001)
          return 1.0;
     else
          return args[0].d / args[1].d;

}

//**********************************************************
//Stop operators

DATATYPE f_nop ( int tree, farg *args, int arity )
{
	 evaluate_tree ( args[0].t, tree, args[0] );
	 return 0.0;
}


DATATYPE f_endV ( int tree, farg *args, int arity )
{
     return 0.0;
}



DATATYPE f_endE ( int tree, farg *args, int arity )
{
     return 0.0;
}

void f_erc_gen ( DATATYPE *r )
{

//APPCODE  Decide the range of erc 
//  *r = (random_double()*2.0 -1.0) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)
//	*r = (random_double()*3.0-1.5) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)
//	*r = (random_double()*5.0-2.5) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)
//	*r = (random_double()*6.0-3.0) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)
//  *r = (random_double()*6.0-3.0) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)
//  *r = (random_double()*20.0-10.0) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)

    *r = (random_double()*4.0) ;	   //  [0,1) *2.0 -> [0,2) -1.0 -> [-1.0,1.0)

}


char *f_erc_print ( DATATYPE d )

{
     static char buffer[20];
     sprintf ( buffer, "%.5f", d );
     return buffer;
}



//--------------------------------------------------------				
//		calculate_erc()
//        -- calculate parmeter of physical element
//           by Koza's erc
//			 intput : erc   
//           output : parameter value
//--------------------------------------------------------				
double calculate_erc(double erc)
{

	app_data* g = get_appdata();
	double U;

	if(erc>-5.0 && erc<3.0) 

		U = erc;

	else if(erc<-100.0 || erc>100.0) 

		U = 0;

	else if(erc>-100.0 && erc<-5.0) 

		U = -0.053*erc - 5.3;

	else if(erc>5.0 && erc<100.0) 

		U = -0.053*erc + 5.3;
	else
		U=0.0;

	//APPCODE map the numeric value of variables in structure to 
	// the desired range
	int d;
	U= pow(10,U);
	if( U>g->maxX){ 
		d= (int)U/g->maxX;
		U=U-d* g->maxX;
	}
	return U;
}



