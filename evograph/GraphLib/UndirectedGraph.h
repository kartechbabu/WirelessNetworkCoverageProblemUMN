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

/*~ UndirectedGraph*/

/*! An Undirected graph is a Graph in which the edges are not  directed.  This means that an edge
 * connecting vertices u and v in the Graph also connects v to u.
 */ 

#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include "Graph.h"

class UndirectedGraph: public Graph {

public:
	UndirectedGraph();
	bool load(const string &FileName);
	
	/*@ Constructor that initializes the graph.
	 */
	UndirectedGraph(unsigned int NumVertices):Graph(NumVertices) { }
	
	/*@ Constructor that initializes the graph by reading the graph configuration from a file.
	 */
	UndirectedGraph(const string& FileName):Graph() { ReadGraph(FileName, this); }
	
	/*@ Method that adds the edges into the graph.
	 *  Throws: VertexNotFound, DuplicateEdge
	 */
	virtual Edge* addEdge(const string& SourceVertex, const string& DestinationVertex, double EdgeWeight=0);
	virtual void printGDL(std::ostream& o);	//HJJ
	virtual Edge* addEdge(Edge* pEdge);	//HJJ
	virtual Edge* addEdge(Vertex* pFromV, Vertex* pToV, int weight=0);	//HJJ

	//HJJ graph manipulation operators
	 virtual Edge* selfLoop(Vertex* v, double weight=0);
	 virtual Edge* connectVertex(Vertex* pFromV, Vertex* pToV, double weight=0);
	 virtual Edge* parallelEdge(Edge* pE, double weight=0);	//create a parallel edge
	 //insert a new node into an edge
	 virtual Edge* insertVertex(Edge* pEdge, Vertex* pVertex, double weight=0.0);	
	 //virtual void cutVertex(Vertex* pVertex);
	 //virtual void cutEdge(Edge* pEdge);
	 //virtual void reverseDirection(Edge* pE);

};

#endif
// Local Variables:
// mode:C++
// End:
