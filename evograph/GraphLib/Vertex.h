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

// Vertex.h: interface for the Vertex class.
// 
// Added by Jianjun Hu 2002-3-2
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTEX_H__63E55FEA_5AC1_44D3_BD98_EA45ACA3B2B4__INCLUDED_)
#define AFX_VERTEX_H__63E55FEA_5AC1_44D3_BD98_EA45ACA3B2B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include <iostream>

using namespace std ;
#define NOT_DEFINED   -9999999
class Vertex  
{
public:
	Vertex(string title, int flag=0);
	Vertex();
	virtual ~Vertex();
	void printGDL(std::ostream& o);	//output to GDL format graph file
public:
	Vertex(string title, string label,int flag,  double xPos=NOT_DEFINED, double yPos=NOT_DEFINED);
	double getY();
	double getX();
	void setY(double yPos);
	void setX(double xPos);
	string getLabel();
	string getTitle();
	vector<Vertex*> neighborList;//keep all the neighbor node pointer
	string	first;		//the title of vertex, must be UNIQUE in the graph!
	int		second;		//store the vertex position in the adjacency matrix
protected:
	string	label;		//the label of this vertex for display, can be duplicate. 
	double		x;			//x coordinate location { x: 59  y: 109 }
	double		y;			//y
public:
	void setPos(double xPos, double yPos, double zPos=0);
	int writeHead;
};

//! Definition of the Node drawing attribute for GRAPHWIN (GW) format graph drawing file
class Node:public Vertex{

public:
	int shape;
	int borderColor;
	double borderWidth;
	double radius1;	//horizontal radius of the node
	double radius2;	//vertical radius of the node
	int color;
	int labelType;
	int labelColor;
	int labelPos;
	string userLabel;

	Node(){
		x =0;
		y =0;
		shape = 0;
		borderColor =1;
		borderWidth = 0.5;
		radius1 = 12.0;
		radius2 = 12.0;
		color = 16;
		labelType = 1;//0 no lable, 1: userlabel,2: data label, 3: index label
		labelColor = -1;
		labelPos = 4;
		userLabel = "A";
	};
	Node(Vertex& v){
		x = v.getX();
		y = v.getY();
		writeHead = v.writeHead;
		first = v.getTitle();
		second = v.second;

		userLabel = v.getTitle();
		shape = 0;
		borderColor =1;
		borderWidth = 0.5;
		radius1 = 12.0;
		radius2 = 12.0;
		color = 16;
		labelType = 1;//0 no lable, 1: userlabel,2: data label, 3: index label
		labelColor = -1;
		labelPos = 4;
	}
	void printGW(ostream& o){
		o<<x<<" ";
		o<<y<<" ";
		o<<shape<<" "<<borderColor<<" "<<borderWidth<<" ";
		o<<radius1<<" "<<radius2<<" "<<color<<" "<<labelType<<" ";
		o<<labelColor<<" "<<labelPos<<" "<<userLabel<<endl;
		}
	~Node(){};
};




#endif // !defined(AFX_VERTEX_H__63E55FEA_5AC1_44D3_BD98_EA45ACA3B2B4__INCLUDED_)
