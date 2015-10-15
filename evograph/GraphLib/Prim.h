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

/*~ Prim
 */

/*! Interface for the class Prim which finds the minimum spanning tree of a graph.
 * Part of the Graph library developed by Karim Fanous (kfanous@acm.org)
 */

#ifndef PRIM_H
#define PRIM_H

#include <vector>
#include "Graph.h"
#include "Algorithm.h"
using namespace std;

namespace {

	/* EDGE keeps the vertices that an edge is incident on.
	 */
	typedef pair<int, int> EDGE; 
}

class Prim:public Algorithm {
private:

	/* The edges in the minimum spanning tree.
	 */
	vector<EDGE> *m_MinimumSpanningTree;

	/* Total cost of the spanning tree.
	 */
	double m_Cost;
	
public:

	/*@ Constructor.
	 */
	Prim(Graph *TheGraph):Algorithm(TheGraph), m_Cost(0.0) { m_MinimumSpanningTree = new vector<EDGE>; }

	/*@ Method that executes Prims algorithm.
	 *  Throws: GraphDisconnected
	 */
	void CalculateMinimumSpanningTree();

	/*@ Method that returns the cost of the spanning tree.
	 */
	double getCost() const { return m_Cost; }

	/*@ Method that returns the edges in the minimum spanning tree.
	 */
	const vector<EDGE>& getMinimumSpanningTree() const { return *m_MinimumSpanningTree; }
	
	/*@ Destructor.
	 */
	~Prim() { delete m_MinimumSpanningTree; }
};

#endif
// Local Variables:
// mode:C++
// End:
