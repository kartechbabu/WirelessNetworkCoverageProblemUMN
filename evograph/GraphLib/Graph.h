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





/*~ Graph
 */
/*! Graph is the abstract base class whose descendants are used to represent either directed or undirected graphs.
 */

/* TODO:   
 */

#ifndef GRAPH_H 
#define GRAPH_H

#include <vector>
#include <deque>
#include <string>
#include <fstream>
#include "Edge.h"
#include "GraphException.h"
#include "Vertex.h"	//HJJ

//#define DEBUG 1
//#define DEFENSIVE 1

#ifdef DEBUG
	#include <iostream>
#endif

using namespace std;

//typedef pair<string, int> Vertex;
const double c_INFINITY = 999999.0;    //Maximum Weight on an edge.

//Forward declaration.
class GraphUtils;

class Graph {

 protected:
	
	/* Number of vertices.
	 */
	unsigned int m_NumVertices;

	/* Number of added vertices.  For any Graph object the number of added vertices should equal to the number of vertices specified in the
	 * constructor.
	 */
	unsigned int m_VertexCounter;
	

	/* Helper functions for class Graph are defined in the friend class, GraphUtils.
	 */
	friend class GraphUtils;

 protected:

	/* Array of vertices in the graph.
	 */
	vector<Vertex*> *m_Vertices;

	/* Array of edges in the graph.  Edges are stored in ascending order of their weight.
	 */
	vector<Edge*> *m_Edges;
	
	/* Adjaceny Matrix that is used to keep track of adjacent vertices in the graph.
	 */
	double **m_Matrix;

 public:
	 virtual void save(std::ostream& o);
	 virtual bool isConnected();
	 int getMinVertexDegree();
	 int getMaxVertexDegree();
	 int getVertexDegree(string vertexName);
	 virtual void printGW(std::ostream& o);
	 virtual double getSumWeight();
	 int getNumEdges();
	 	/* Method that initializes the graph data structures.
	 */
	 void Initialize();

	 void setNumVertices(int n);
	 Edge* getEdge(int writeHead);
	 Vertex* getVertex(int writeHead);
	 virtual void report();
	 virtual void clean();
	 virtual void removeEdges(){
		 for(int i=0;i<m_Edges->size();i++) delete (*m_Edges)[i];
		 m_Edges->clear();
	 };

	 void cutVertex(string title);
	 void cutEdge(string fromVertex, string toVertex);
	 
	 Edge* getEdge(string from, string to);
	 Vertex* getVertex(string title);
	 bool load(const string& FileName);

	/*@ Default constructor.
	 */
	 Graph():m_NumVertices(1), m_VertexCounter(0) {
		 m_NumVertices=1;//HJJ a graph at least have 1 vertices.
		 Initialize(); 
	 }

	/*@ Constructor that sets the number of vertices in the graph.
	 */
	Graph(unsigned int NumVertices);
	
	/*@ Copy constructor.
	 */
	Graph(const Graph& NewGraph);

	/*@ Assignment operator.
	 */
	const Graph& operator=(const Graph& Rhs);
	
	/*@ Method that adds a vertex to the graph.
	 *  Throws:	TooManyVertices, DuplicateVertex
	 */
	Vertex* addVertex(const string& VertexName);	//HJJ

	/*@ Method that adds an edge to the graph.
	 */
	virtual Vertex* addVertex(Vertex* pVertex);
	virtual Edge* addEdge(const string& SourceVertex, const string& DestinationVertex, double EdgeWeight) = 0;
	virtual void printGDL(std::ostream& o)=0;
	virtual Edge* addEdge(Edge* pEdge){return NULL;};
	virtual Edge* addEdge(Vertex* pFromV, Vertex* pToV){return NULL;};

	//HJJ graph manipulation operators
	 virtual void cutVertex(Vertex* pVertex);
	 virtual void cutEdge(Edge* pEdge);
	 virtual void reverseDirection(Edge* e);
	 virtual Edge* selfLoop(Vertex* v, double weight=0);
	 virtual Edge* connectVertex(Vertex* pFromV, Vertex* pToV, double weight=0);
	 virtual Edge* parallelEdge(Edge* pE, double weight=0.);	//create a parallel edge
	 //insert a new node into an edge
	 virtual Edge* insertVertex(Edge* pEdge, Vertex* pVertex, double weight=0.0);	
	


	/*@ Method that returns the number of vertices in the graph.
	 */
	unsigned int getNumVertices() const { return m_NumVertices; }

	const vector<Vertex*>& getVertices() const { return *m_Vertices; }

	const vector<Edge*>& getEdges() const { return *m_Edges; }
	
	double** getMatrix() const { return m_Matrix; }

	/*@ Method that returns the actual number of vertices in the graph.
	 */
	unsigned int getNumAddedVertices() const { return m_VertexCounter; }

	/*@ Friend function that initialzes the graph by reading a file that contains the vertices and edges in the graph.
	 * The file should adhere to a certain format.  This function is a friend so that the appropriate addEdge() method is selected at runtime, depending on the object TheGraph
	 * points to. 
	 */
	friend void ReadGraph(const string& FileName, Graph* TheGraph);


//#if (DEBUG) 
	/*@ Debugging method that dumps the Adjacency Matrix.  Only used if the DEBUG option is defined.
	 */
	void DumpAdjacencyMatrix();
	
	/*@ Debugging method that dumps the Graphs edges.  Only used if the DEBUG option is defined.
	 */
	void DumpEdgeVector();

	/*@ This method insures that the class invariant is preserved, namely that the number of vertices specified in the constructor have actually been entered in the Graph.
	 * The number of added vertices can never be greater than the required vertices as this is an Exception that is caught in addVertex(). Only used if DEBUG is defined.
	 */
	void Invariant() { if(m_NumVertices != m_VertexCounter) throw TooFewVertices(m_NumVertices); }
//HJJ #endif
	
	/*@ Destructor.
	 */
	virtual ~Graph();
};

/*~ GraphUtils
 */

/*! This class is a helper class for Graph objects.  It includes many functions that are performed on Graph objects.  These functions are not included in 
 * the Graph class specification so as to minimize Graph's interface.  The functions in this class are all static, so they can be used without the
 * need to initialize an object.  GraphUtils is a friend of class Graph so that it can access the private data members of any Graph object.  I decided to add
 * these functions in a friend class to simplify things for the user.  Had these functions been global, then the user would have to supply them with the
 * adjacency matrix, vertices and edges of the graph object.  I think that having to do so everytime would be quite tedious, hence a friend class would
 * be much more practical. 
 */

class GraphUtils { 
protected:

  /* Constructor.
   */
  GraphUtils() { }

public: 

  /*@ Method that returns the integral value that the vertex is stored as in the Adjaceny Matrix.
	 *  Throws: VertexNotFound
   */
  static unsigned int getVertexValue(const class Graph& TheGraph, const string& VertexName);
	
  /*@ Method that returns all of the adjacent vertices to a vertex.
	 *  Throws: VertexNotFound
   */
  static vector<Vertex*> getAdjacent(const class Graph& TheGraph, unsigned int VertexNum);
	
	/*@ Method that returns all of the adjacent vertices to a vertex.
	 *  Throws: VertexNotFound
   */
  static vector<Vertex*> getAdjacent(const class Graph& TheGraph, const string& VertexName) { return getAdjacent(TheGraph, getVertexValue(TheGraph,VertexName) ); }
	
  /*@ Method that returns an edge incident on 2 vertices.
	 * Throws: VertexNotFound(), EdgeNotFound()
   */
  static Edge* getIncidentEdge(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex);

  /*@ Method that returns the edges incident on the vertex.
	 * Throws: VertexNotFound(), EdgeNotFound()
   */
  static vector<Edge*> getIncidentEdges(const class Graph& TheGraph, unsigned int vertex_val);

	/*@ Method that returns the edges incident on the vertex.
	 * Throws: VertexNotFound(), EdgeNotFound()
   */
  static vector<Edge*> getIncidentEdges(const class Graph& TheGraph, const string& VertexName) { return getIncidentEdges(TheGraph, getVertexValue(TheGraph, VertexName) ); }

  /*@ Method that returns an edge's weight.
	 * Throws: VertexNotFound(), EdgeNotFound()
   */	 
  static double getEdgeWeight(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex);

	/*@ Method that returns an edge's weight.
	 * Throws: VertexNotFound(), EdgeNotFound()
   */	 
  static double getEdgeWeight(const class Graph& TheGraph, const string& SourceVertex, const string& DestinationVertex) { return getEdgeWeight(TheGraph, getVertexValue(TheGraph, SourceVertex), getVertexValue(TheGraph, DestinationVertex) ); }

  /*@ Method that returns the name of a vertex given its position in the graph.
	 * Throws: VertexNotFound()
   */
  static string getVertexName(const class Graph& TheGraph, unsigned int VertexPosition);

  /*@ Method that checks if the Graph is connected.  It returns true if it is connected and false if not.
   */
  static bool IsConnected(const class Graph& TheGraph);
   
  /*@ Method that checks if 2 vertices are adjacent.
	 * Throws: VertexNotFound()
   */	
  static bool IsAdjacent(const class Graph& TheGraph, unsigned int SourceVertex, unsigned int DestinationVertex); 

	/*@ Method that checks if 2 vertices are adjacent.
	 * Throws:  VertexNotFound()	
   */	
  static bool IsAdjacent(const class Graph& TheGraph, const string& SourceVertex, const string& DestinationVertex) { return IsAdjacent(TheGraph, getVertexValue(TheGraph, SourceVertex), getVertexValue(TheGraph,DestinationVertex) ); }

  /*@ Method that checks if the vertex is in the Graph.
	 *  Throws: VertexNotFound
   */
  static bool IsVertexInGraph(const class Graph& TheGraph, unsigned int VertexNumber);
	
	/*@ Method that checks if the vertex is in the Graph.
	 *  Throws: VertexNotFound
   */
  static bool IsVertexInGraph(const class Graph& TheGraph, const string& VertexName) { return GraphUtils::getVertexValue(TheGraph, VertexName) <= TheGraph.m_NumVertices; }

};



#endif
// Local Variables:
// mode:C++
// End:
