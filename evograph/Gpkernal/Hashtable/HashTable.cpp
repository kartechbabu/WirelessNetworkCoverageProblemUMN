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


#include "Hashtable.h"
//#define DEBUG_SFS
/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

/**
 * Construct the hash table.
 */

HashTable::HashTable()
{
	tableSize = nextPrime(100);
	for(int i=0;i<tableSize;i++){
		CBucket* pB= new CBucket();
		theLists.push_back(pB);

	}
}

HashTable::~HashTable()
{
	for(int i=0;i<tableSize;i++){
		delete (CBucket*)(theLists[i]);
	}
	theLists.clear();
}

/**
 * Insert item x into the hash table. If the item is
 * already present, then do nothing.
 */

int HashTable::insert( CStruct*  x )
{
	#ifdef DEBUG_SFS
	printf("+++++++++insert struct   %d\n", x->structID);
	#endif

	int index = hash( x->structID, tableSize );
    CBucket*  whichBucket = theLists[index];
	CStructList:: iterator itr = whichBucket->Exists( x );

    if( itr== whichBucket->m_DataList.end()){
        whichBucket->InsertSorted(x);
		return 1;
	}
	else{
		return 0;//HJJ we find this structID exists, then return no action
		printf(" insert fail!\n");
		exit(-1);
	}
}

/**
 * Remove item x from the hash table.
 */

void HashTable::remove( CStruct*  x )
{
	#ifdef DEBUG_SFS
	printf("------- remove struct   %d\n", x->structID);
	x->report();
	#endif

   theLists[ hash( x->structID, tableSize )]->Remove( x );
}

/**
 * Find item x in the hash table.
 * Return the matching item or ITEM_NOT_FOUND if not found
 */

CStruct* HashTable::find(int structID) const
{
	list<CStruct*>:: iterator itr;
	int index = hash(structID, tableSize );
    itr = theLists[index]->Exists( structID );
	if(itr==theLists[index]->m_DataList.end())
		return NULL;
	else
		return (CStruct*)(*itr );
}

/**
 * Make the hash table logically empty.
 */
//delete all the hashed CStruct objects!

void HashTable::makeEmpty( )
{
    for( int i = 0; i < theLists.size( ); i++ )
        theLists[ i ]->MakeEmpty( );
}

/**
 * Deep copy.
 */

//copy all the struct obect to this new hashtable
HashTable& HashTable::operator=( HashTable&  rhs )
{
	list<CStruct*>::iterator itr;
	tableSize = rhs.tableSize;
	Create(tableSize);
	CStruct* pSt;

	for(int i=0;i<theLists.size();i++){
		for(itr=rhs.theLists[i]->m_DataList.begin();itr != rhs.theLists[i]->m_DataList.end();itr++){
			pSt = new CStruct(**itr);
			theLists[i]->m_DataList.push_back(pSt);
		}
	}
    return *this;
}


/**
 * A hash routine for string objects.
 */
int hash( const string  key, int tableSize )
{
    int hashVal = 0;

    for( int i = 0; i < key.length( ); i++ )
        hashVal = 37 * hashVal + key[ i ];

    hashVal %= tableSize;
    if( hashVal < 0 )
        hashVal += tableSize;

    return hashVal;
}


/**
 * A hash routine for ints.
 */
int hash( int key, int tableSize )
{
    if( key < 0 ) key = -key;
    return key % tableSize;

}

HashTable::HashTable(int size)
{
	Create(size);

}

int HashTable::GetSize()
{
	return tableSize;

}

void HashTable::remove(int structID)
{
   theLists[ hash( structID, tableSize )]->Remove( structID );

}

HashTable::HashTable(HashTable &rhs)
{
	list<CStruct*>::iterator itr;
	tableSize = rhs.tableSize;
	Create(tableSize);
	CStruct* pSt;

	for(int i=0;i<theLists.size();i++){
		for(itr=rhs.theLists[i]->m_DataList.begin();itr != rhs.theLists[i]->m_DataList.end();itr++){
			pSt = new CStruct(*(*itr));
			theLists[i]->m_DataList.push_back(pSt);
		}
	}
}

///rehash all the hashtable with the new tablesize
void HashTable::Rehash(int size)
{
	//get all the struct object in the current hashtable
	list<CStruct*> structList;
	list<CStruct*>::iterator itr;

	for(int i=0;i<theLists.size();i++){
		for(itr=theLists[i]->m_DataList.begin();itr != theLists[i]->m_DataList.end();itr++){
			structList.push_back(*itr);
		}
	}

	for( i=0;i<theLists.size();i++){
		theLists[i]->m_DataList.clear();
	}
	theLists.clear();

	//construct the new hashtable
	Create(size);

	
	for(itr=structList.begin();itr!= structList.end();itr++){
		insert((*itr));
	}

}

void HashTable::ShowAllStruct()
{
	list<CStruct*>::iterator itr;
	CStruct* pSt;

	printf("structID age  bestFit  GrowRate  iniFit lastGrow nGrowGen nInds noGrowGen\n");
	for(int i=0;i<theLists.size();i++){
		for(itr=theLists[i]->m_DataList.begin();itr != theLists[i]->m_DataList.end();itr++){
			pSt = (*itr);
			pSt->report();
		}
	}
}

void HashTable::Create(int size)
{
	if( isPrime(size))
		tableSize = size;
	else
		tableSize = nextPrime(size);

	for(int i=0;i<tableSize;i++){
		CBucket* pB= new CBucket();
		theLists.push_back(pB);
	}
}

int HashTable::GetItemSize()
{
	//get all the struct object in the current hashtable
	list<CStruct*> structList;
	list<CStruct*>::iterator itr;

	int n=0;
	for(int i=0;i<theLists.size();i++){
		n+= theLists[i]->m_DataList.size();
	}

	return n;
}
