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

/*~ Algorithm*/

/*! Algorithm is an abstract base class that contains the pure virtual function Run().  Children of Algorithm have to implement
 * this function to perform the desired graph algorithm.  Graph objects need not know anything about Algorithm's ancestors.
 * Graph interfaces with Algorithm.
 * Part of Graph library by Karim Fanous(kfanous@acm.org)
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

//Forward declaration of Graph whose interface is in Graph.h
class Graph;

class Algorithm {
 protected:

	/* Pointer to the Graph on which an algorithm will be performed.
	 */
	Graph *m_Graph;
	
 public:

	/*@ Default Constructor
	 */
	Algorithm():m_Graph(0) { }
	
	/*@ Constructor.
	 */
	Algorithm(Graph *TheGraph):m_Graph(TheGraph) { }
	
	/*@ Destructor.
	 */
	virtual ~Algorithm() { }
};
		
#endif
// Local Variables:
// mode:C++
// End:
