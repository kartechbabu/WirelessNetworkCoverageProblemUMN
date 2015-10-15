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

/*~ Edge
 */

/* Edge represents the edges that are inserted in a Graph.  It keeps track of the vertices on which the edge is incident on, together with the edges weight.
 */
#ifndef EDGE_H
#define EDGE_H

#include<string>
#include<vector>
#include "vertex.h"
#include <math.h>

#define INFINITY 999999

using namespace std;

class Edge {
public: 
	int writeHead;	//APPCODE used for graph evolution by GP. 

protected:
	/* Names of the 2 vertices associated with the edge.
	 */
	string m_From, m_To;
	
	//HJJ 
	//To speed the scanning of the graph, we store the object of two end vertex
	//in the edge object. 
	Vertex* m_pFrom;	//the from vertex object retrieved by its title
	Vertex* m_pTo;		//the to vertex object retrieved by its title


	/* Edge's weight.
	 */
	double m_Weight;

public:

	/*@ Constructor
	 */
	Edge(const string& From , const string& To, double Weight = 0.0)
		:m_From(From), m_To(To), m_Weight(Weight) {m_pFrom=NULL;m_pTo=NULL;writeHead=NOT_DEFINED; };
	Edge(Vertex* pFrom , Vertex* pTo, double Weight = 0.0)
		:m_pFrom(pFrom), m_pTo(pTo), m_Weight(Weight) {
		m_From=pFrom->getTitle(); m_To = pTo->getTitle();writeHead=NOT_DEFINED;
	};
	Edge(){
		m_pTo  = NULL;
		m_pFrom = NULL;
		
	}
	
	Edge(Edge* pE, double weight=0){ 
		m_From = pE->getFromVertex();
		m_To = pE->getToVertex();
		m_pFrom = pE->getFromVertexObject();
		m_pTo = pE->getToVertexObject();
		writeHead = NOT_DEFINED;
		m_Weight = weight;
	}
		
	inline string getFromVertex(){ return m_From;};
	inline string getToVertex(){ return m_To;};
	inline void setFromVertex(string from){ m_From = from;};
	inline void setToVertex(string to){ m_To =to;};

	inline Vertex* getFromVertexObject(){ return m_pFrom;};
	inline Vertex* getToVertexObject(){ return m_pTo;};
	inline void setFromVertexObject(Vertex* from){ m_pFrom=from;};
	inline void setToVertexObject(Vertex* to ){ m_pTo= to;};
	inline void reverseDirection(){ 
		string temp = m_To; m_To = m_From; m_From = temp;
		Vertex* pTempVertex = m_pFrom; m_pFrom =m_pTo; m_pTo =pTempVertex;
	};

	/*@ Method that returns the vertices associated with the Edge.
	 */
	pair<string, string> getVertices() const { return pair<string, string>(m_From, m_To); }

	/*@ Method that matches an edge with 2 vertices.
	 */
	bool EdgeMatch(string to, string from) { 
		if ( ( (to == m_To) && (from == m_From) ) || ( (m_To == from) && (m_From == to) ) )
			return true;
		else
			return false;
	}
	
	/*@ Method that returns the edge's weight.
	 */
	inline double getWeight() const { return m_Weight; }
	inline void setWeight(double w){ m_Weight = w; }

	//output the GDL format file (Graph Description Language)
	//edge: { sourcename: "35864"  targetname: "35928"  color: red }
	//HJJ output the GDL format file
	void printGDL(std::ostream& o){
		o<<"edge: { sourcename: \"";
		o<<m_From;
		o<<"\"  targetname: \"";
		if(m_To.empty())
			o<<"label";
		else
			o<<m_To;
		
		o<<"\"";
		o<<"}\n";
	}

	double getLength(){
		double dis;
		if( m_pFrom ==NULL || m_pTo ==NULL){
			dis= INFINITY;
		}
		else{
				double x1 = m_pFrom->getX();
				double y1 = m_pFrom->getY();
				double x2 = m_pTo->getX();
				double y2 = m_pTo->getY();

				if(abs(x1) > INFINITY || abs(x2) > INFINITY || abs(y1) > INFINITY || abs(y2) > INFINITY){
					return INFINITY;
				}

				dis =(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
				//printf("x1= %d  x2 = %d    \ny1= %d  y2 =%d \n",x1, x2,
				//		y1, y2);
				if(dis >=0)
					dis = sqrt(dis);
				//printf("dis = %f \n", dis);
		}
		return dis;
	}

	/*@ Destructor.
	 */
	~Edge() { }
};

class Arc:public Edge{
public:
	double width;
	int color;
	int shape;
	int style;
	int direction;
	int labelType;
	int labelColor;
	int labelPos;
	int poly;
	string userLabel;
	Arc(){
		width = 0.5;
		color = 1;
		shape = 0;
		style = 0;
		direction = 1;
		labelType = 1;
		labelColor = 1;
		labelPos = 4;
		poly = 2;
	}
};

									


#endif
// Local Variables:
// mode:C++
// End:
