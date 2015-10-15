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

/*~ DepthFirstTraversal*/

/*! This Algorithm performs a Depth First Traversal on the Graph.  It can also be used to  
 * find if the Graph is a Tree or not.  i.e If it has cycles or not.  The Traversal will start from the
 * source destination specified in the class constructor.  This means that if a complete traversal is needed, or
 * the user wants to find if the graph has cycles, then the source vertex should be the root of the Graph.
 */

#ifndef DEPTHFIRSTTRAVERSAL_H
#define DEPTHFIRSTTRAVERSAL_H

#include <string>
#include <vector>
#include "Algorithm.h"
#include "Graph.h"
using namespace std;

class DepthFirstTraversal:public Algorithm {
private:
	
	/* Vertex at which the traversal will begin.
	 */
	unsigned int m_SourceVertex;
	
	/* An array that keeps track of the visited vertices.
	 */
	vector<unsigned int> m_VisitedVertices;

	/* The path of the traversal.
	 */
	vector<unsigned int> m_Path;

	/* Indicates if the graph is a tree or not.
	 */
	bool m_IsTree;

	/* Private method that performs a Depth first traversal on a given vertex.
	 */
	void Traverse(unsigned int SourceVertex);
		
public:

	/*@ The constructor takes the vertex at which the traversal will start at.
	 *  Throws: VertexNotFound 
	 */
	DepthFirstTraversal(Graph *TheGraph, unsigned int SourceVertex);

	/*@ The constructor takes the vertex at which the traversal will start at.
	 */
	DepthFirstTraversal(Graph *TheGraph, const string& SourceVertex);

	/*@ Method that preforms a Depth first traversal on all of the vertices of the Graph.
	 * While performing the traversal, it will also check if there are cycles in the graph.
	 * The Graph has to be connected, otherwise an Exception will be thrown.
	 * Throws: GraphDisconnected
	 */
	void DFT();

	/*@ Method that returns the path of the traversal.
	 */
	const vector<unsigned int>& getTraversalPath() const { return m_Path; }

	/*@ Method that returns true if the graph is a tree, otherwise false.
	 */
	bool IsTree() const { return m_IsTree; }
};

#endif

// Local Variables:
// mode:C++
// End:
