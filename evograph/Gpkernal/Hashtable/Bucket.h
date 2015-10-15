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


// Bucket.h: interface for the CBucket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUCKET_H__BFC32136_30E4_11D4_81F8_9F12352DA82A__INCLUDED_)
#define AFX_BUCKET_H__BFC32136_30E4_11D4_81F8_9F12352DA82A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
using namespace std;


class CStruct;
typedef list<CStruct*> CStructList;

class CBucket  
{
public:
	CStruct* GetStruct(int structID);
	void MakeEmpty();
	unsigned long GetIndex(void);	//Returns the Index of the current bucket
	CStructList::iterator Exists(const CStruct *pData);
	CStructList::iterator Exists(int structID);
	bool Remove( CStruct* pItem);	//Removes the specified data from the current bucket
	CStructList* GetDataList(void);	//Returns data list of the current bucket
	void SetIndex(unsigned long nIndex);
	void InsertSorted(CStruct* pData);	//Inserts an item ascending
	CBucket();
	virtual ~CBucket();

public:
	bool Remove(int structID);
	list<CStruct*> m_DataList;	//The list of data
	unsigned long m_nIndex;	//Bucket index
private:
};

#endif // !defined(AFX_BUCKET_H__BFC32136_30E4_11D4_81F8_9F12352DA82A__INCLUDED_)
