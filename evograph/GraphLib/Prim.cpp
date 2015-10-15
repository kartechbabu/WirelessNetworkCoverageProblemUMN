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

#include "Prim.h"
//#define DEBUG

/* Method that performs Prim's algorithm on the Graph to get a minimum spanning tree of that graph.
 */
void Prim::CalculateMinimumSpanningTree() {

//Check if the Graph is disconnceted, if it is then throw an Exception.
#if DEFENSIVE
	if( GraphUtils::IsConnected( *m_Graph ) == false)
		throw GraphDisconnected();
#endif
	
	EDGE new_edge;

	//The number of vertices in the graph.
	const unsigned int NumVertices = m_Graph -> getNumVertices();

	//The edges in the graph.
	const vector<Edge*> ParentEdges = m_Graph -> getEdges();

	//The graph's adjacency matrix.
	double** AdjacencyMatrix = m_Graph -> getMatrix();
	
	//Initialization.
	m_Cost = 0.0;
	m_MinimumSpanningTree -> clear();
	int * Near = new int[NumVertices];
	
	//Getting the indices of the vertices in the adjaceny matrix.  
	int k = GraphUtils::getVertexValue( *m_Graph, ParentEdges[0] -> getVertices().first);
	int l = GraphUtils::getVertexValue( *m_Graph, ParentEdges[0] -> getVertices().second);

	//Add the edge (k,l) in the MST.
	new_edge.first = k;
	new_edge.second = l;
	m_MinimumSpanningTree -> push_back(new_edge);
	
	//Initialize the total cost to the weight of the first edge to be inserted in the MSP.
	m_Cost = AdjacencyMatrix[k][l];

	//Initialize the Near array.
	for(unsigned int i = 0; i < NumVertices; ++i) {
		if( (AdjacencyMatrix[i][l] < AdjacencyMatrix[i][k]) )
			Near[i] = l;
		else
			Near[i] = k;
	}
	Near[l] = Near[k] = -1;

	for(i = 1; i < NumVertices - 1; ++i) {
		unsigned int loc, min;
		
		for(loc = 0; loc < NumVertices; ++loc) {
			if(Near[loc] != -1) {
				min = loc;
				break;
			}
		}
		//Finding the minimum edge.
		for(unsigned int j = loc + 1; j < NumVertices; ++j) {
			if( (Near[j] != -1) && (AdjacencyMatrix[j][Near[j]] <= AdjacencyMatrix[min][Near[min]]) )
				min = j;
		}
		
		//Add the edge in the MST.
		new_edge.first = min;
		new_edge.second = Near[min];
		m_MinimumSpanningTree -> push_back(new_edge);
		
		//Update the total cost and the Near array.
		m_Cost += AdjacencyMatrix[min][Near[min]];

		Near[min] = -1;
		for(unsigned int k =1; k < NumVertices; ++k) 
			if( (Near[k] != -1) && (AdjacencyMatrix[k][Near[k]] > AdjacencyMatrix[k][min]) )
				Near[k] = min;
	}
	delete [] Near;

#ifdef DEBUG
	//Dump the edges in the MST and the total cost.
	for(vector<EDGE>::iterator I = m_MinimumSpanningTree -> begin(); I != m_MinimumSpanningTree -> end(); ++I)
		cout<< (*I).first << (*I).second<<" ";
	cout<<endl<<"Cost: "<<m_Cost<<endl;
#endif
}
