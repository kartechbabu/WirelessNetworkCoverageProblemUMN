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

#include "DirectedGraph.h"


/* Method that addes an edge between 2 vertices in the graph.  The vertices must have been already added to the Graph and the Edge must not have been added.
 * If any of those conditions is violated and DEFENSIVE is defined in the makefile, then an Exception will be thrown.
 */
Edge* DirectedGraph::addEdge(const string& SourceVertex, const string& DestinationVertex, double EdgeWeight) {
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
			
	//Storing the weight in the AdjacenyMatrix.  The weight is only stored in location [i][j] since the Graph is a Directed Graph.
	m_Matrix[from_loc][to_loc] = EdgeWeight;

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
