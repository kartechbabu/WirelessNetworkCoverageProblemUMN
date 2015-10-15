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


#include "Graph.h"

/* Method that returns the index of a vertex in the adjancy matrix.
 */
unsigned int GraphUtils::getVertexValue(const class Graph& TheGraph, const string& VertexName) {
	
	for(vector<Vertex*>::iterator i = TheGraph.m_Vertices -> begin(); i != TheGraph.m_Vertices -> end(); ++i) {
		if( (*i) -> first == VertexName )
			return (*i) -> second;
	}
	//Vertex not found in Graph , so throw an Exception.
	throw VertexNotFound();
}

/* Method that returns the name of a vertex given its position in the graph.
 */
string GraphUtils::getVertexName(const class Graph& TheGraph, unsigned int VertexPosition) { 
	if(VertexPosition <= TheGraph.m_NumVertices)
		return (*TheGraph.m_Vertices)[VertexPosition] -> first; 
	else 
		throw VertexNotFound();
}

/* Private method that returns an edge incident on 2 vertices.
 */
Edge* GraphUtils::getIncidentEdge(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex) {

	#ifdef DEBUG_G	
	printf("GraphUtils::getIncidentEdges1:...\n");
	for(int ii=0;ii<TheGraph.m_NumVertices;ii++){
		for(int jj=0;jj<TheGraph.m_NumVertices;jj++)
			cout<< TheGraph.m_Matrix[ii][jj]<<" ";
		cout<<endl;
	}//HJJD

	for(vector<Edge*>::iterator i = TheGraph.m_Edges -> begin(); i != TheGraph.m_Edges -> end(); ++i) 
	{
		cout<< (*i)->getFromVertex()<<" "<<(*i)->getToVertex()<<endl;
	}
	#endif
	


	if( (SourceVertex <= TheGraph.m_NumVertices) && (DestinationVertex <= TheGraph.m_NumVertices)) {
		string first_vertex = (*TheGraph.m_Vertices)[SourceVertex] -> first;
		string second_vertex = (*TheGraph.m_Vertices)[DestinationVertex] -> first;
		
		for(vector<Edge*>::iterator i = TheGraph.m_Edges -> begin(); i != TheGraph.m_Edges -> end(); ++i) {
			if( (*i) -> EdgeMatch(first_vertex, second_vertex))
				return (*i);
		}
		//Edge not found so throw an Exception.
		throw EdgeNotFound();
	}
	else
		throw VertexNotFound();
}

/* Method that returns the vertices adjacent to a vertex.
 */
vector<Vertex*> GraphUtils::getAdjacent(const class Graph& TheGraph, unsigned int VertexNum) {

	vector<Vertex*> temp;

	if(VertexNum <= TheGraph.m_NumVertices) {
		for(unsigned int i = 0; i < TheGraph.m_NumVertices; ++i) {
			if(TheGraph.m_Matrix[VertexNum][i] != c_INFINITY)
				temp.push_back( (*TheGraph.m_Vertices)[i] );
		}
		return temp;
	}
	else
		throw VertexNotFound();
}


/* Method that returns the edges incident on a vertex.
 */
vector<Edge*> GraphUtils::getIncidentEdges(const class Graph& TheGraph, unsigned int vertex_val) {

	vector <Edge*> temp;	
	#ifdef DEBUG_G	
		printf("GraphUtils::getIncidentEdges:...\n");
		for(int ii=0;ii<TheGraph.m_NumVertices;ii++){
			for(int jj=0;jj<TheGraph.m_NumVertices;jj++)
				cout<< TheGraph.m_Matrix[ii][jj]<<" ";
			cout<<endl;
		}//HJJD
	#endif

	if(vertex_val < TheGraph.m_NumVertices) {
	
		for(unsigned int i = 0; i < TheGraph.m_NumVertices; ++i) {	
			//Adjacent vertices.
			if( TheGraph.m_Matrix[vertex_val][i]!=c_INFINITY) {
				temp.push_back(getIncidentEdge(TheGraph, vertex_val, i));
			}
		}
	}
	else
		throw VertexNotFound();
	
	return temp;
}

/* Method that gets the weight on an edge between 2 vertices.
  */
double GraphUtils::getEdgeWeight(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex) {

	if( (SourceVertex <= TheGraph.m_NumVertices) && (DestinationVertex <= TheGraph.m_NumVertices) ) {
		
		//Locate the Edge in the Graph
		Edge* temp = getIncidentEdge(TheGraph, SourceVertex, DestinationVertex);
		
		//Edge is found, so return the weight, otherwise throw an Exception.
		if(temp)
			return temp -> getWeight();
		
		else
			throw EdgeNotFound();
	}
	else
		throw VertexNotFound();
}


/* Method that checks if the graph is connected or not.  If it is, then it returns true, otherwise false.
 */
bool GraphUtils::IsConnected(const Graph& TheGraph) {
	bool Connected = false;
	unsigned int col;

	//Loop on the vertices, row by row.
	for(unsigned int row = 0; row < TheGraph.m_NumVertices; ++row) {	

		Connected = false;
		col = 0;

		//Keep on looping on the columns, if all of the entries = infinity.
		do {
			if( (TheGraph.m_Matrix[row][col] == c_INFINITY) && (!Connected) ) {
				Connected = false;
				col++;
			}
			else 
				Connected = true;

		}while( (!Connected) && (col < TheGraph.m_NumVertices) );

		//If all of that vertex's entries where equal to infinty, then
		//that graph is disconnected.
		if(Connected == false)
			return(Connected);

	}
	return(Connected);
}

/* Method that checks if 2 vertices are adjacent.
 */
bool GraphUtils::IsAdjacent(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex) {
	if( (SourceVertex <= TheGraph.m_NumVertices) && (DestinationVertex <= TheGraph.m_NumVertices))
		return TheGraph.m_Matrix[SourceVertex][DestinationVertex] != c_INFINITY;
	else
		throw VertexNotFound();
}

/* Method that checks if the vertex is in the Graph.
 */
bool GraphUtils::IsVertexInGraph(const class Graph& TheGraph, unsigned int VertexNumber) {
	if(VertexNumber <= TheGraph.m_NumVertices)
		return VertexNumber <= TheGraph.m_NumVertices;
	else
		throw VertexNotFound();
}
	
