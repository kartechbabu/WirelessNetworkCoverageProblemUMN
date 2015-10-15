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

#include "UndirectedGraph.h"
#include <iostream>



/* Method that addes an edge between 2 vertices in the graph.  The vertices must have been already added to the Graph and the Edge must not have been added.
 * If any of those conditions is violated and DEFENSIVE is defined in the makefile, then an Exception will be thrown.
 */
Edge* UndirectedGraph::addEdge(const string& SourceVertex, const string& DestinationVertex, double EdgeWeight) {
	unsigned int from_loc, to_loc;  //Locations of vertices in the Adjacency Matrix.

	//Getting the vertices locations in the AdjacencyMatrix. 
	from_loc = GraphUtils::getVertexValue(*this, SourceVertex);
	to_loc = GraphUtils::getVertexValue(*this, DestinationVertex);


//Check that the Edge was not previously inserted in the Graph.
#if(DEFENSIVE) 
		for(vector<Edge*>::iterator I = m_Edges -> begin(); I != m_Edges -> end(); ++I) {
			if( (*I) -> EdgeMatch(SourceVertex, DestinationVertex) ) 
				throw DuplicateEdge( SourceVertex + DestinationVertex);
		}
#endif
	
	//Create the new Edge object that will be added to the Graph.
	Edge *NewEdge =  new Edge(SourceVertex, DestinationVertex, EdgeWeight);
	NewEdge->setFromVertexObject(getVertex(SourceVertex));		//HJJ
	NewEdge->setToVertexObject(getVertex(DestinationVertex));	//HJJ
			
	//Storing the weight in the AdjacenyMatrix.  The weight is stored in location [i][j] and [j][i] since the Graph is a Undirected Graph.
	m_Matrix[from_loc][to_loc] = EdgeWeight;
	m_Matrix[to_loc][from_loc] = EdgeWeight;


	//Inserting the edge in the vector of edges.  Edges are inserted in ascending order of their weight.
	if(m_Edges -> size() == 0)
		m_Edges -> push_back(NewEdge);
	
	else for(vector<Edge*>::iterator i = m_Edges -> begin(); i != m_Edges -> end(); ++i) {
		//If the current Edge has a weight greater than the new one, or we reached the end, then insert the edge.
		if( (EdgeWeight < (*i) -> getWeight()) ) {
			m_Edges -> insert(i, NewEdge);
			break;
		}
		else if(i == m_Edges -> end() -1) {
			m_Edges -> push_back(NewEdge);
			break;
		}
	}

	return NewEdge;
}


//HJJ output the GDL format file 
void UndirectedGraph:: printGDL(std::ostream& o)
{

	//write the title part
	o<<"graph: {	"<<"\n";
  o<<"colorentry 42: 104 192 226	"<<"\n";
  o<<"color: 42	"<<"\n";
  o<<"scaling: 1.0	"<<"\n";
  o<<"amax: 0	"<<"\n";
  o<<"xbase: 5	"<<"\n";
  o<<"ybase: 5	"<<"\n";
  o<<"xspace: 20	"<<"\n";
  o<<"xlspace: 10	"<<"\n";
  o<<"yspace: 70	"<<"\n";
  o<<"xraster: 1	"<<"\n";
  o<<"xlraster: 1	"<<"\n";
  o<<"yraster: 1	"<<"\n";
  o<<"layout_downfactor: 1	"<<"\n";
  o<<"fstraight_phase: no		"<<"\n";
  o<<"layout_upfactor: 1		"<<"\n";
  o<<"layout_nearfactor: 1	"<<"\n";
  o<<"layout_splinefactor: 70		"<<"\n";
  o<<"spreadlevel: 1				"<<"\n";
  o<<"treefactor: 0.500000	"<<"\n";
  o<<"bmax: 100							"<<"\n";
  o<<"cmin: 0								"<<"\n";
  o<<"cmax: 32000						"<<"\n";
  o<<"pmin: 0								"<<"\n";
  o<<"pmax: 100							"<<"\n";
  o<<"rmin: 0								"<<"\n";
  o<<"rmax: 100							"<<"\n";
  o<<"smax: 100							"<<"\n";
  o<<"orientation: top_to_bottom	"<<"\n";
  o<<"late_edge_labels: no	"<<"\n";
  o<<"display_edge_labels: no			"<<"\n";
  o<<"dirty_edge_labels: no	"<<"\n";
  o<<"finetuning: yes				"<<"\n";
  o<<"nearedges: yes				"<<"\n";
  o<<"splines: no						"<<"\n";
  o<<"ignoresingles: no			"<<"\n";
  o<<"straight_phase: no		"<<"\n";
  o<<"priority_phase: no		"<<"\n";	
  o<<"manhatten_edges: no		"<<"\n";
  o<<"smanhatten_edges: no	"<<"\n";
  o<<"port_sharing: yes			"<<"\n";
  o<<"crossingphase2: no		"<<"\n";
  o<<"crossingoptimization: yes	"<<"\n";
  o<<"crossingweight: bary	"<<"\n";
  o<<"arrow_mode: free			"<<"\n";
  o<<"edge.arrowsize: 8			"<<"\n";
  o<<"node.height: 34				"<<"\n";
  o<<"node.width: 34				"<<"\n";
  o<<"node.textcolor: white	"<<"\n";
  o<<"node.color: black			"<<"\n";		

	//write the node description
	for(vector<Vertex*>::iterator i = m_Vertices -> begin(); i != m_Vertices -> end(); ++i) 
	{
		(*i)->printGDL(o);
	}

	//write the edge description
	for(vector<Edge*>::iterator j = m_Edges -> begin(); j != m_Edges -> end(); ++j) 
	{
		(*j)->printGDL(o);
	}

	//close the description
	o<<"}"<<"\n";

}

bool UndirectedGraph::load(const string &FileName)
{
	
	//Opening the file.
	ifstream GraphFile(FileName.c_str(), ios::in | ios::binary);
	
	if (!GraphFile) {
		cout << "Cannot open Graph file "<<endl;
		return false;
	}

	string version;
	GraphFile>>version;
	if(version!="EGF_EvoGraphFormat"){
		cout<<"Wrong graph file format!"<<endl;
		cout<<"use: EGF_EvoGraphFormat graph files"<<endl;
		exit(-1);
	}

	//Reading the number of vertices in the graph.
	GraphFile >>  m_NumVertices;
	Initialize();
	
	string VertexName;	
	int x, y;
	//Reading the vertices in the graph
	for(unsigned int i = 0; i < m_NumVertices; ++i) {
		GraphFile >> VertexName;
		GraphFile >>x;
		GraphFile >>y;
		Vertex* v = addVertex(VertexName);
		v->setX(x);
		v->setY(y);
	}

	//Adding the edges in the graph.
	string SourceVertex;
	string DestinationVertex;
	double Weight;
	while(true) {
		GraphFile >> SourceVertex;

		//Keep on reading edges until the # is read.
		if(SourceVertex == "#")
			break;
		
		GraphFile >> DestinationVertex;
		GraphFile >> Weight;
		addEdge(SourceVertex, DestinationVertex, Weight);
	}
	return true;
}

//HJJ directly add an edge to a graph, similar to 
//void UndirectedGraph::addEdge(const string& SourceVertex, const string& DestinationVertex, double EdgeWeight) {
Edge* UndirectedGraph::addEdge(Edge *pEdge)
{
	unsigned int from_loc, to_loc;  //Locations of vertices in the Adjacency Matrix.
	string SourceVertex, DestinationVertex;
	SourceVertex =  pEdge->getFromVertex();
	DestinationVertex = pEdge->getToVertex();
	//Getting the vertices locations in the AdjacencyMatrix. 
	from_loc = GraphUtils::getVertexValue(*this, SourceVertex);
	to_loc = GraphUtils::getVertexValue(*this, DestinationVertex);

//Check that the Edge was not previously inserted in the Graph.
#if(DEFENSIVE) 
		for(vector<Edge*>::iterator I = m_Edges -> begin(); I != m_Edges -> end(); ++I) {
			if( (*I) -> EdgeMatch(SourceVertex, DestinationVertex) ) 
				throw DuplicateEdge( SourceVertex + DestinationVertex);
		}
#endif
	
	//set up the end vertex object
	if(pEdge->getFromVertexObject()==NULL){
		pEdge->setFromVertexObject(getVertex(pEdge->getFromVertex()));	//HJJ
		pEdge->setToVertexObject(getVertex(pEdge->getToVertex()));		//HJJ
	}

	//Storing the weight in the AdjacenyMatrix.  The weight is stored in location [i][j] and [j][i] since the Graph is a Undirected Graph.
	double EdgeWeight = pEdge->getWeight();
	m_Matrix[from_loc][to_loc] = EdgeWeight;
	m_Matrix[to_loc][from_loc] = EdgeWeight;


	//Inserting the edge in the vector of edges.  Edges are inserted in ascending order of their weight.
	if(m_Edges -> size() == 0)
		m_Edges -> push_back(pEdge);
	
	else for(vector<Edge*>::iterator i = m_Edges -> begin(); i != m_Edges -> end(); ++i) {
		//If the current Edge has a weight greater than the new one, or we reached the end, then insert the edge.
		if( (EdgeWeight < (*i) -> getWeight()) ) {
			m_Edges -> insert(i, pEdge);
			break;
		}
		else if(i == m_Edges -> end() -1) {
			m_Edges -> push_back(pEdge);
			break;
		}
	}
	return pEdge;
}

Edge* UndirectedGraph::addEdge(Vertex *pFromV, Vertex *pToV, int weight)
{
	string SourceVertex, DestinationVertex;
	SourceVertex =  pFromV->getTitle();
	DestinationVertex = pToV->getTitle();
	Edge* pE = addEdge(SourceVertex, DestinationVertex, weight);
	return pE;
}




//HJJ
//Initially:    A--------->B    insert C
//After:		A---->C---->B
//				  |
//				  !---this is the old edge!	
Edge* UndirectedGraph::insertVertex(Edge *pEdge, Vertex *pVertex, double weight)
{
	Edge* pNew = new Edge(pVertex,pEdge->getToVertexObject(), weight);
	addEdge(pNew);
	pEdge->setToVertexObject(pVertex);
	pEdge->setToVertex(pVertex->getTitle());
	return pNew;
}

Edge* UndirectedGraph::parallelEdge(Edge *pE, double weight)
{
	if(pE == NULL){
		cout<<"UndirectedGraph::parallelEdge: "<<" this edge dosen't exist!\n";
		return NULL;
	}

	Edge* pNew = new Edge(pE, weight);
	addEdge(pNew);
	return pNew;
}

//connect an existing vertex (already in UndirectedGraph) to another vertex
//by creating a new edge.
//Before we use an Vertex, we always assume this vertex has already been 
//added into the graph!
//From node: newVertex, To node: oldVertex
Edge* UndirectedGraph::connectVertex(Vertex *pFromV, Vertex *pToV, double weight)
{
	Edge* pNewEdge = new Edge(pFromV, pToV, weight);
	addEdge(pNewEdge);
	return pNewEdge;

}

Edge* UndirectedGraph::selfLoop(Vertex* pV, double weight)
{
	Edge* pNewEdge = new Edge(pV, pV, weight);
	addEdge(pNewEdge);
	return pNewEdge;

}








UndirectedGraph::UndirectedGraph()
{
	Graph::Graph();
}
