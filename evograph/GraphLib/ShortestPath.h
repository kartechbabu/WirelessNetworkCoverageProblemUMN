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

/*~ ShortestPath
 */
/*! This class performs Ford's shortest path algorithm on a graph.  Given a source vertex u and a destination vertex v,
 * the method Run() will find the shortest path between them, if one exists.  It can also calculate the graph's diameter.
 */

#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include <vector>
#include <algorithm>
#include <string>
#include "Algorithm.h"
#include "Edge.h"
#include "Graph.h"
using namespace std;

class ShortestPath: public Algorithm {
private:

	/* The source and destination vertices.
	 */
	unsigned int m_Source, m_Destination;
	
	/* Cost of the path from the source to destination vertices.
	 */
	double m_Cost;

	/* The graph's diameter, which is the maximum shortest distance between any vertices u and v in the graph.
	 */
	double m_Diameter;

	/* The path from the source to the destination.
	 */
	vector<unsigned int> m_Path;

public:

	/*@ Constructor that takes the numerical value of the vertices on which a shortest path between them is to be found.
	 * Throws: VertexNotFound
	 */
	ShortestPath(Graph* TheGraph, unsigned int u, unsigned int v);
	
	/*@ Constructor that takes the names of the vertices on which a shortest path between them is to be found.
	 */
	ShortestPath(Graph* TheGraph, const string& Source, const string& Destination);

	/*@ Method that perform's Ford's Shortest Path Algorithm.
	 *  Throws: NoPathFoun
	 */
	void CalculateShortestPath();

	/*@ Method that calculates the graph's diameter.
	 */
	void CalculateDiameter();

	/*@ Method that returns the path (if it exists) from the source to the destination vertices.
	 */
	const vector<unsigned int>& getPath() const { return m_Path; }

	/*@ Method that returns the cost of the path.
	 */
	double getPathCost() const { return m_Cost; }

	/*@ Method that returns the graph's Diameter.
	 */
	double getDiameter() const { return m_Diameter; }

	~ShortestPath() { }
};

#endif
// Local Variables:
// mode:C++
// End:
