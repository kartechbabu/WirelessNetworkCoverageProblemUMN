/*  HashTable for SFS 
 *  Copyright (C) 2003  Michigan State University
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
 */


// Data.cpp: implementation of the CStruct class.
//
//////////////////////////////////////////////////////////////////////
#include "Struct.h"
#include "stdio.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStruct::CStruct()
{
	structID = 0; // not set yet	
	age = 0;			
	initFit =0.0 ;		
	bestFit =0.0;		
	growthRate =0.0;		
	nGrowthTestGens =10;	//default 10
	lastKFit  =0.0;	
	nIndividuals = 1;//each time we create a structure, we assume we have at least one individual
	noGrowthEvals	=0;		
	nIndNextGen =0;
	evaluations = 0;
}


CStruct::~CStruct()
{

}

CStruct& CStruct::operator =(CStruct&  rhs )
{
	structID = rhs.structID; // not set yet	
	age = rhs.age;			
	initFit =rhs.initFit ;		
	bestFit =rhs.bestFit;		
	growthRate =rhs.growthRate;		
	nGrowthTestGens =rhs.nGrowthTestGens;	//default 10
	lastKFit  =rhs.lastKFit;	
	nIndividuals = rhs.nIndividuals;
	noGrowthEvals	=rhs.noGrowthEvals;	
	return *this;
}



CStruct::CStruct(CStruct &st)
{
	structID = st.structID; // not set yet	
	age = st.age;			
	initFit =st.initFit ;		
	bestFit =st.bestFit;		
	growthRate =st.growthRate;		
	nGrowthTestGens =st.nGrowthTestGens;	//default 10
	lastKFit  =st.lastKFit;	
	nIndividuals = st.nIndividuals;
	noGrowthEvals	=st.noGrowthEvals;		
	nIndNextGen = st.nIndNextGen;
}

void CStruct::report()
{
//	printf("Structure %d Info......\n", structID);
// 	printf("structID age  bestFit  GrowRate  iniFit lastGrow nGrowGen nInds noGrowGen\n");
	printf("%4d  %5d %5f  %8f  %8f  %6f    %6d  %4d  %5d \n",
		structID, age, bestFit,  growthRate,   initFit,  lastKFit,  nIndNextGen, nIndividuals, noGrowthEvals);
}
