/*  Graph library
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
 *	
 *	This graph library is based on Graphlib by Karim Fanous enhanced with input/output related functions 
 *	needed for graph evolution.
 *
 *	Karim Fanous
 *	Department of Computer Science 
 *	American University in Cairo 
 *	http://www.geocities.com/TheTropics/Paradise/7231/GraphLib.htm
 *  
 */


#include <iostream>
#include <fstream>
#include "Graph.h"
#include "Algorithm.h"
#include "Prim.h"
#include "ShortestPath.h"
#include "BreadthFirstTraversal.h"
#include "DepthFirstTraversal.h"
#include "UndirectedGraph.h"
#include "DirectedGraph.h"
#include <time.h>
int random_int ( int max );
void random_seed ( int seed );
using namespace std;



int main(int argc, char **argv) {

	/*
	//generating a graph by hand
	test.addVertex("a");
	test.addVertex("b");
	test.addVertex("c");
	test.addVertex("d");
	test.addVertex("e");

 	test.addEdge("a", "b", 2);
 	test.addEdge("a", "d", 5);
	test.addEdge("a", "e", 10);
	test.addEdge("b", "c", 7);
	test.addEdge("b", "e", 5);
	test.addEdge("c", "e", 3);
	test.addEdge("c", "d", 20);
	test.addEdge("d", "e", 15);
	test.addEdge("e", "a", 10);
*/


	//{randomly generate a client distribution graph!

	if(argc==1){
				cout<<"---------------------------------------------- \n";
				cout<<"            Random Graph Generator 1.0\n";
				cout<<"Generating a random graph as a client distribution map...\n";
				cout<<"Author: Jianjun Hu (hujianju@msu.edu)\n";
				cout<<"    Department of Computer Science  \n";
				cout<<"        Michigan State University    \n";
				cout<<"----------------------------------------------\n\n";
				cout<<"Usage:  graphLib FileName VertexNo [EdgeNo MaxXCoordinate MaxYCoordinate]\n";
				cout<<" e.g:  graphLib client 20 10 1000 1000\n\n";
				return 0;
	}

	char fileName[20] = "clientGraph";
	int numVertices=10;
	int numEdges=1;
	int MaxX = 1000;
	int MaxY = 1000;

	if(argc ==3){ 
		strcpy(fileName,argv[1]);
		numVertices = atoi(argv[2]);
	}
	else if(argc==6){
		strcpy(fileName,argv[1]);
		numVertices = atoi(argv[2]);
		numEdges=atoi(argv[3]);
		MaxX = atoi(argv[4]);
		MaxY = atoi(argv[5]);
	}

	string fn1 = string(fileName) +".gdl";
	string fn2 = string(fileName) +".gw";
	string fn3 = string(fileName) +".txt";

	std::ofstream gdlClient(fn1.data());
	std::ofstream gwClient(fn2.data());
	std::ofstream egfClient(fn3.data());
	
	UndirectedGraph clientGraph(numVertices);
	char index[10];
	srand( (unsigned)time( NULL ) );
	random_seed(rand());
	for(int i=0;i<numVertices;i++){
			itoa(i,index,10);
			Vertex* pV = clientGraph.addVertex(string("C")+index);
			pV->setX(random_int (MaxX));
			pV->setY(random_int (MaxY));
	};
	
	int row, col;
	for(int k=0;k<numEdges;k++){
		row = random_int(numVertices);
		col = random_int(numVertices);
		while(row == col){
			row = random_int(numVertices);
			col = random_int(numVertices);
		}

		clientGraph.addEdge(GraphUtils::getVertexName(clientGraph, row),
			GraphUtils::getVertexName(clientGraph, col));
	}
	clientGraph.report();

	//export an graph file in GDL format, which can be viewed and layouted by a grap viewer aiSee 
	//which can downloaded at http://www.aisee.com/download/
	clientGraph.printGDL(gdlClient);

	//export an graph file in GW format, which can be viewed and layouted by a grap viewer LEDA 
	//which can downloaded at http://www.algorithmic-solutions.info/eval/0.php3
	clientGraph.printGW(gwClient);


	//export an graph file in EGF format,a simple graph format for GP evolution defined by Jianjun Hu
  clientGraph.save(egfClient);
	gdlClient.close();
	gwClient.close();
	egfClient.close();

	//}randomly generate a client distribution graph




/*
	ofstream gdl("test.gdl");
	ofstream gdl1("test_deledge.gdl");

	try {

		UndirectedGraph test(5);
//	test.load("Graph_xy.txt");
//	test.load("Tsp_76city.txt");
		test.load("Graph_xy.txt");
		test.DumpEdgeVector();
		test.DumpAdjacencyMatrix();
		//add coordinates of nodes
		vector<Vertex*> nodelist;
		nodelist = test.getVertices();
		random_seed(2337);
		for(vector<Vertex*>::iterator i = nodelist.begin(); i != nodelist.end(); ++i)
		{
			(*i)->setX((*i)->getX()*50);
			(*i)->setY((*i)->getY()*50);
//		(*i)->setX(random_int(1000));
//		(*i)->setY(random_int(1000));
		}

		Prim sol(&test);
		sol.CalculateMinimumSpanningTree();
		cout<<"Cost of tree = "<< sol.getCost() << endl;

		ShortestPath som(&test, 0 , 4);
		som.CalculateShortestPath();
		cout<<"Cost = "<< som.getPathCost() << endl;
		test.printGDL(gdl);
		//test.cutVertex("a");	//Test OK. 
		//test.cutEdge("a", "e");//test ok.
		//test.selfLoop(test.getVertex("a"));//ok
		//test.connectVertex(test.getVertex("a"), test.getVertex("c"));//ok
		//test.parallelEdge(test.getEdge("a", "c"));//ok

		Vertex* pV = test.addVertex("f");
		test.insertVertex(test.getEdge("a", "b"),pV, 111); 

		test.printGDL(gdl);
		test.DumpEdgeVector();
		test.DumpAdjacencyMatrix();

		gdl.close();

	}
	
	catch(GraphException &e) {
		e.DisplayErrorMessage();
	}
	//UndirectedGraph soil("Graph.txt");
		*/

	
	return(0);
}
