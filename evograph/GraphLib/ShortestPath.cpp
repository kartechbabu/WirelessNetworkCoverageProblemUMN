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

#include "ShortestPath.h"

ShortestPath::ShortestPath(Graph* TheGraph, unsigned int Source, unsigned int Destination):Algorithm(TheGraph), m_Source(Source), m_Destination(Destination), m_Cost(0.0), m_Diameter(0.0) {  

	//Checking to see if the vertices are in the Graph.
#if(DEFENSIVE) 
		
	 if (GraphUtils::IsVertexInGraph(*TheGraph, Source) == false)
		 throw VertexNotFound();

	 else if (GraphUtils::IsVertexInGraph(*TheGraph, Destination) == false)
		 throw VertexNotFound();

#endif
}


ShortestPath::ShortestPath(Graph* TheGraph, const string& Source, const string& Destination):Algorithm(TheGraph), m_Cost(0.0), m_Diameter(0.0) {
	m_Source	  = GraphUtils::getVertexValue(*m_Graph, Source);
	m_Destination = GraphUtils::getVertexValue(*m_Graph, Destination);
}


void ShortestPath::CalculateShortestPath() {

	//The number of vertices in the graph.
	const unsigned int NumVertices = m_Graph -> getNumVertices();

	//The graph's adjacency matrix.
	double** ParentMatrix = m_Graph -> getMatrix();

	//Vector that stores the distance between the current vertices and the remaing vertices in the graph.
	vector<double> Distance(NumVertices , c_INFINITY);

	//Vector that stores the predecesor of each vertex.  Used to determine the path.
	vector<unsigned int> Predecessor(NumVertices , -1);

	//Vector that states if a vertex has been traversed before in the path or not.
	vector<unsigned int> Used;

	//The current vertex.
	unsigned int CurrentVertex;

	//A flag that states if the algorithm changed the path or not.  If not then the algorithm terminates.
	bool Change;
				 
	//Initialization.
	Distance[m_Source] = 0.0;
	m_Cost = 0.0;
	Predecessor[m_Source] = m_Source;
	CurrentVertex = m_Source;
	Used.push_back(m_Source);
	m_Path.clear();
	
	
	do {
		Change = false;

		//DO IT FOR ADJACENT ONLY.
		for(unsigned int j = 0; j < NumVertices; ++j) {
			if( Distance[CurrentVertex] + ParentMatrix[CurrentVertex][j] <  Distance[j] ) {
				Change = true;
				Distance[j] = Distance[CurrentVertex] + ParentMatrix[CurrentVertex][j];
				Predecessor[j] = CurrentVertex;
			}
		}
		
		//Find the next current vertex which is the minimum vector in Distance and not in Used.
		vector<double>::iterator MinimumLoc;
		MinimumLoc = Distance.begin();

		bool FirstElement = true;
		do {  
			if( (!FirstElement))
				MinimumLoc++;
			MinimumLoc = min_element(MinimumLoc, Distance.end()-1);
			FirstElement = false;
		}
		while( find(Used.begin(), Used.end(), MinimumLoc - Distance.begin() ) != Used.end());
		
		CurrentVertex = MinimumLoc - Distance.begin();
		Used.push_back(CurrentVertex);

		//Reached the destination vertex.
		if(CurrentVertex == m_Destination)
			break;

		//If the current vertex is INIFINTELY away from the source vertex,
		//then no path exists between the source and destination vertices.
		if(Distance[CurrentVertex] == c_INFINITY)
			throw NoPathFound( GraphUtils::getVertexName(*m_Graph, m_Source), GraphUtils::getVertexName(*m_Graph, m_Destination) );
		
	} while( (Change == true) && (Used.size() != NumVertices -1) );

	//Find the path from the destination to the source, by following the
	//Predecessor of the destination until you reach the source.
	cout<<"Path is: ";
	unsigned int lol = m_Destination;
	m_Path.push_back(m_Destination);
	do {
		lol = Predecessor[lol];
		m_Path.push_back(lol);
	}while(lol != m_Source);
	
	//The path is stored in reverse order, so reverse it to get the right
	//order.
	reverse(m_Path.begin(), m_Path.end());

	vector<unsigned int>::const_iterator LookAhead;
	//Dump the path and the cost.
	for(vector<unsigned int>::const_iterator L = m_Path.begin(); L != m_Path.end(); ++L) {

		//Get the next vertex in the path.
		LookAhead = L;
		if( (++LookAhead) != m_Path.end() )
			m_Cost += GraphUtils::getEdgeWeight(*m_Graph, *L, *LookAhead);

		cout << *L << " ";
	}
#if DEBUG
	cout << endl << "Cost of path: " << m_Cost << endl;
#endif
		
}

void ShortestPath::CalculateDiameter() {
	
	double Distance = 0.0;
		
	//Keeping the old state.
	unsigned int Old_Source = m_Source;
	unsigned int Old_Destination = m_Destination;
	double Old_Cost = m_Cost;
	vector<unsigned int> Old_Path = m_Path;

	vector<Vertex*> GraphVertices = m_Graph->getVertices();
	for(vector<Vertex*>::const_iterator V = GraphVertices.begin(); V != GraphVertices.end(); ++V) {
		for(vector<Vertex*>::const_iterator U = V+1; U != GraphVertices.end(); ++U) {
			m_Source = (*V)->second;
			m_Destination = (*U)->second;
			try{
				CalculateShortestPath();
				if( m_Cost > Distance)
					Distance = m_Cost;
			}

			//Handle the case in which no path exists between the 2 vertices.
			catch (GraphException e) {
				continue;
			}
		}
	}
	m_Diameter = Distance;

	//Retrieving the original state.
	m_Source = Old_Source;
	m_Destination = Old_Destination;
	m_Cost = Old_Cost;
	m_Path = Old_Path;
}
	
	

			
