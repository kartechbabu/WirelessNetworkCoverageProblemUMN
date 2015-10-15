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


// Bucket.cpp: implementation of the CBucket class.
//
//////////////////////////////////////////////////////////////////////

#include <list>
#include "Bucket.h"
#include "struct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBucket::CBucket()
{
	m_nIndex = 0L;
}

CBucket::~CBucket()
{
	CStructList::iterator i;

	if (m_DataList.empty())
		return;
   
	for(i=m_DataList.begin();i!=m_DataList.end();++i)
	{
		CStruct* pData = (CStruct*)(*i);
		delete pData;
	}
	m_DataList.clear();
}

void CBucket::InsertSorted( CStruct *pData)
{
	CStructList::iterator i, prevPos;
	if (pData)
	{
		if(m_DataList.empty())
		{
			m_DataList.push_back(pData);
			return;
		}

		//order from big index to small index
		for(i=m_DataList.begin();i!=m_DataList.end();++i)
		{
			CStruct* data = (CStruct*)(*i);
			if(pData->structID >= data->structID){
				m_DataList.insert(i,pData);
				return;
			}
		}
		if(i== m_DataList.end()){ // add to the end
				m_DataList.push_back(pData);
		}
	}
}

void CBucket::SetIndex(unsigned long nIndex)
{
	m_nIndex = nIndex;
}


CStructList* CBucket::GetDataList()
{
	return &m_DataList;
}

bool CBucket::Remove( CStruct *pItem)
{
	CStructList::iterator position;
	if ((position=Exists(pItem)) != this->m_DataList.end())
	{
		CStruct* data = (CStruct*)(*position);
		delete data;
		m_DataList.erase(position);
		return true;
	}
	return false;		
}

CStructList::iterator 
CBucket::Exists(const CStruct *pData)
{
	CStructList::iterator i;
	int size = m_DataList.size();
	if(m_DataList.empty()) return m_DataList.end();
	for(i=m_DataList.begin();i!=m_DataList.end();++i){
		if((*i)->structID == pData->structID)
			return i;
	}
	return m_DataList.end();
}


list<CStruct*>::iterator 
CBucket::Exists(int structID){
	CStructList::iterator i;
	if(m_DataList.empty()) return m_DataList.end();
	for(i=m_DataList.begin();i!=m_DataList.end();++i){
		if((*i)->structID == structID)
			return i;
	}
	return m_DataList.end();

}


unsigned long CBucket::GetIndex()
{
	return m_nIndex;
}

//delete all the hashed CStruct objects!
void CBucket::MakeEmpty()
{
	CStructList::iterator i;
	
	for(i=m_DataList.begin();i!=m_DataList.end();++i){
		delete (*i);
	}
	m_DataList.clear();
}

CStruct* CBucket::GetStruct(int structID)
{
	CStructList::iterator i;
	for(i=m_DataList.begin();i!=m_DataList.end();++i){
		if( (*i)->structID == structID)
			return (CStruct*) (*i);
	}
	return NULL;
}

bool CBucket::Remove(int structID)
{
	CStructList::iterator position;
	if ((position=Exists(structID)) != this->m_DataList.end())
	{
		CStruct* data = (CStruct*)(*position);
		delete data;
		m_DataList.erase(position);
		return true;
	}
	return false;	
}
