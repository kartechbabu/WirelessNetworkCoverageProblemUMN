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

// Data.h: interface for the CStruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__BFC3212F_30E4_11D4_81F8_9F12352DA82A__INCLUDED_)
#define AFX_DATA_H__BFC3212F_30E4_11D4_81F8_9F12352DA82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStruct  
{

public:
	CStruct();
	virtual ~CStruct();
	CStruct& operator=(CStruct&  rhs );

public:
	void report();
	CStruct(CStruct& st);
//	CString m_pivot;
	unsigned int structID;		/// the unique structure ID

	int		age;				/// how many generations this structure has been evolved
	int		evaluations;		/// how many evaluations 
	double	initFit;			/// initial fitness of this structure
	double	bestFit;			/// current best fitness
	double	growthRate;			///	(BestFit-initFit)/evaluations
	double  lastKFit;			/// how much fitness gain in last 10 generations
	int		nGrowthTestGens;	/// how many generations used to calculate lastKGenGrowth
	int		nIndividuals;		/// Number of individuals in current populations
	int		nIndNextGen;		/// number of individuals of this struct in the next generated population
	int		noGrowthEvals;		/// how many generations pass since last progress generations
	int		lastGen;			/// lastest generation
};

#endif // !defined(AFX_DATA_H__BFC3212F_30E4_11D4_81F8_9F12352DA82A__INCLUDED_)
