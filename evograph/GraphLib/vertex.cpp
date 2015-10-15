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

// Vertex.cpp: implementation of the Vertex class.
//
//////////////////////////////////////////////////////////////////////

#include "Vertex.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Vertex::Vertex()
{
	writeHead=NOT_DEFINED;
}

Vertex::~Vertex()
{

}

Vertex::	Vertex(string title, int flag)
{
	first = title;
	second= flag;
	x=NOT_DEFINED;
	y=NOT_DEFINED;
	writeHead = NOT_DEFINED;
}

void Vertex::	printGDL(std::ostream& o)
{
	o<<"node: { title:\"";
	o<<first;
	o<<"\" label: \"";
	if (label.empty())
		o<<first;	//if no label,use the title (ID) instead
	else
		o<<label;

	o<<"\"";

	if(x!= NOT_DEFINED){
			o<<"loc: { x: ";
			o<<(int)x<<" y: ";
			o<<(int)y<<" }";
	}

	o<<"}"<<"\n";
}

string Vertex::getTitle()
{
	return first;

}

string Vertex::getLabel()
{
	return label;

}

void Vertex::setX(double xPos)
{
	x = xPos;

}

void Vertex::setY(double yPos)
{
	y= yPos;

}

double Vertex::getX()
{
	return x;

}

double Vertex::getY()
{
	return y;

}

Vertex::Vertex(string title,string Label ,int flag,double xPos, double yPos)
{
	first = title;
	if (Label.data()==NULL)
		label = title;
	else
		label = Label;

	x= xPos;
	y= yPos;

	writeHead =NOT_DEFINED;
}

void Vertex::setPos(double xPos, double yPos, double zPos)
{
	x = xPos;
	y = yPos;

}
