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


#ifndef GRAPHEXCEPTION_H
#define GRAPHEXCEPTION_H

#include <string>
#include <iostream>
using namespace std;

/*~ GraphException*/

/*! GraphException is a generic class that encapsulates the exceptions that can occur on a Graph object.  Such exceptions can be adding a vertex that is already in the Graph,
 * looking for an edge/vertex not in the Graph or attempting to perfom an algorithm on a disconnected Graph while the algorithm works only on connected ones.  GraphException
 * is an abstract base class and all of Graph exceptions will derive from this class.
 */
class GraphException {
 protected:

	/* The error message that is to be displayed to the user when the exception occurs.
	 */
	string m_ErrorMessage;
	
 public:

	/*@ Constructor that sets the error message.
	 */
	GraphException(const string& ErrorMessage):m_ErrorMessage(ErrorMessage) { }

	/*@ Virtual function that displays the error message. 
	 */
	virtual void DisplayErrorMessage() { cout << m_ErrorMessage << endl; }	
};

/*~ DuplicateEdge
 */
/*! DuplicateEdge is an Exception that is thrown when an Edge is added twice into a Graph.  The exception will display the Edge that was added.
 */
class DuplicateEdge:public GraphException {
private:

	/* The name of the Edge that the user tried to add more than once to the Graph.
	 */ 
	string m_EdgeName;
public:

	/*@ Constructor.
	 */
	DuplicateEdge(const string& EdgeName):GraphException("Exception: Trying to add Edge twice. "), m_EdgeName(EdgeName) { }

	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << "Edge name: " << m_EdgeName << endl; }
};

/*~ DuplicateVertex
 */

/*! DuplicateVertex is an Exception that is thrown when an Vertex is added twice into a Graph.  The exception will display the Vertex that was added.
 */
class DuplicateVertex:public GraphException {
private:

	/* The name of the Vertex that the user tried to add more than once to the Graph.
	 */ 
	string m_VertexName;
public:

	/*@ Constructor.
	 */
	DuplicateVertex(const string& VertexName):GraphException("Exception: Trying to add Vertex twice. "), m_VertexName(VertexName) { }
	
	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << "Vertex name: " << m_VertexName << endl; }
};

/*~ EdgeNotFound
 */

/*! EdgeNotFound is an Exception that is thrown when the user is searching for an not in the Graph.  Such a case can occur, if the user wants to find the Edge's weight or
 * the vertices it is incident on.
 */
class EdgeNotFound:public GraphException {
public:

	/*@ Constructor.
	 */
	EdgeNotFound():GraphException("Exception: Edge not found in Graph ") { }

	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() { cout << m_ErrorMessage << endl; }
};

/*~ VertexNotFound
 */

/*! VertexNotFound is an Exception that is thrown when the user is searching for an not in the Graph.  Such a case can occur, if the user wants to find the Vertex's weight or
 * the vertices it is incident on.
 */
class VertexNotFound:public GraphException {
public:

	/*@ Constructor.
	 */
	VertexNotFound():GraphException("Exception: Vertex not found in Graph. ") { }
	
	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() { cout << m_ErrorMessage << endl; }
};

/*~ GraphDisconnected.
 */

/*! GraphDisconnected is an Exception that is thrown when an attempt is made to perform an Algorithm on a Graph that must be connected.  For example, trying to get the
 * Minimum Spanning Tree of a disconnected Graph.
 */
class GraphDisconnected:public GraphException {
public:

	/*@ Constructor.
	 */
	GraphDisconnected():GraphException("Exception: The Graph is disconnected ") { }
	
	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << endl; }
};

/*~ TooManyVertices
 */

/*! TooManyVertices is an Exception that is thrown when an attempt is made to add more vertices in the Graph than was specified in the Graph constructor when the Graph object
 * was created.
 */
class TooManyVertices:public GraphException {
private:
	int m_MaxVertices;
	
public:

	/*@ Constructor.
	 */
	TooManyVertices(int MaxVertices):GraphException("Exception: The too many vertices added to Graph.  Max Number is: "), m_MaxVertices(MaxVertices) { }

	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << m_MaxVertices << endl; }
};

/*~ TooFewVertices
 */

/*! TooFewVertices is an Exception that is thrown when an attempt is made to execute an Algorithm on a Graph that has less vertices than  was specified in the Graph constructor
 * when the Graph object was created.
 */
class TooFewVertices:public GraphException {
private:
	int m_RequiredVertices;
	
public:

	/*@ Constructor.
	 */
	TooFewVertices(int RequiredVertices):GraphException("Exception: The too few vertices added to Graph.  Required Number is: "), m_RequiredVertices(RequiredVertices) { }

	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << m_RequiredVertices << endl; }
};

/*~ NoPathFound
 */

/*! NoPathFound is an exception that is thrown if 2 vertices have no path between them and an attempt is made to find the shortest path between them.
 */
class NoPathFound:public GraphException {
private:
	string m_SourceVertex;
	string m_DestinationVertex;
	
public:

	/*@ Constructor.
	 */
	NoPathFound(string SourceVertex, string DestinationVertex):GraphException("Exception: There is no path between these 2 vertices: "), m_SourceVertex(SourceVertex), m_DestinationVertex(DestinationVertex) { }

	/*@ Method that displays an appropriate error message.
	 */
	void DisplayErrorMessage() {  cout << m_ErrorMessage << m_SourceVertex << " " << m_DestinationVertex << endl; }
};


#endif
// Local Variables:
// mode:C++
// End:
