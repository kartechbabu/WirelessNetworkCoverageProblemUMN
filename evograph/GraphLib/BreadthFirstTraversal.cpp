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

#include "BreadthFirstTraversal.h"
#include <queue>

//Constants that are only local to this file.
namespace {
	const int UNVISITED  = 0;      //Used to designate that a vertex has not been visited.
	const int VISITED    = 1;      //Used to designate that a vertex has been visited.
}

BreadthFirstTraversal::BreadthFirstTraversal(Graph *TheGraph, unsigned int SourceVertex):Algorithm(TheGraph), m_SourceVertex(SourceVertex), m_IsTree(true) { 

	//Initialize all of the vertices to unvisited.
	m_VisitedVertices.resize(m_Graph -> getNumVertices(), UNVISITED);
}

BreadthFirstTraversal::BreadthFirstTraversal(Graph *TheGraph, const string& SourceVertex):Algorithm(TheGraph), m_IsTree(true) {

	//Checking to see if the vertex is in the Graph.
	m_SourceVertex = GraphUtils::getVertexValue(*m_Graph, SourceVertex);

}

void BreadthFirstTraversal::BFT() {

	//Initialization.
	m_VisitedVertices.clear();
	m_Path.clear();

	//Traverse the graph.
	for(unsigned int i = 0; i < m_Graph -> getNumVertices(); ++i)
		if(m_VisitedVertices[i] == UNVISITED)
			Traverse(i);

	#ifdef DEBUG
		cout<< endl;
	#endif

}
	
void BreadthFirstTraversal::Traverse(unsigned int SourceVertex) {
	unsigned int CurrentVertex = SourceVertex; 
	queue<unsigned int> UnVisitedVertices;         //A queue that stores the vertices adjacent to the current vertex that have not been visited.
	vector<Vertex*> AdjacentVertices;     //List of vertices adjacent to the current vertex.

	m_VisitedVertices[CurrentVertex] = VISITED;

	m_Path.push_back(CurrentVertex);

#ifdef DEBUG
	cout<<CurrentVertex;
#endif

	//Traverse all of the Vertices adjacent to the CurrentVertex.
	do {

		//List of all of the vertices adjacent to the current vertex.
		AdjacentVertices = GraphUtils::getAdjacent( *m_Graph, CurrentVertex );
	
		//Loop on all of the vertices adjacent to the current vertex.
		for(unsigned int i = 0;  i < AdjacentVertices.size(); ++i) {

			//A vertex adjacent to the current vertex.
			Vertex *AdjacentVertex = AdjacentVertices[i];

			//If the adjacent vertex has not been visited then, visit it.  Otherwise there is a cycle in the Graph.
			if( m_VisitedVertices[ AdjacentVertex -> second ] == UNVISITED ) {

				//Pushing the vertex in the path.
				m_Path.push_back(AdjacentVertex -> second);

				#ifdef DEBUG
					cout<< " " << AdjacentVertex -> second;
				#endif

				UnVisitedVertices.push( AdjacentVertex -> second );
				m_VisitedVertices[ AdjacentVertex -> second ] = VISITED;
			}

			//Vertex has been visited before, so the graph has a cycle.
			else
				m_IsTree = false;
		}
		//If all of the vertices have been visited, then stop.
		if(UnVisitedVertices.size() == 0)
			return;

		//There are still more vertices to visit, so visit the next one.
		else {
			CurrentVertex = UnVisitedVertices.front();
			UnVisitedVertices.pop();
		}
	}while(true);

}
				
