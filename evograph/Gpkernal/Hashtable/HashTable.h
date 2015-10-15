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



#ifndef _SEPARATE_CHAINING_H_
#define _SEPARATE_CHAINING_H_

#include <vector>
#include <list>
#include <string>

#include "struct.h"
#include "bucket.h"
using namespace std;

// SeparateChaining Hash table class
//
// CONSTRUCTION: an initialization for ITEM_NOT_FOUND
//               and an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Hashable find( x )     --> Return item that matches x
// void makeEmpty( )      --> Remove all items
// int hash( string str, int tableSize )
//                        --> Global method to hash strings

class HashTable
{
  public:
	  void Create(int size);
	  void ShowAllStruct();
	  void Rehash(int newSize);
	  HashTable(HashTable& ht);
	  void remove(int structID);
	  int GetSize();
	HashTable(int size);
    explicit HashTable( );
	~HashTable();

    CStruct*  find( int structID ) const;

    void makeEmpty( );
    int insert(  CStruct*  x );
    void remove(  CStruct*  x );

	HashTable& operator=(HashTable&  rhs );
 private:
	int tableSize;
 public:
	 int GetItemSize();
    vector<CBucket*> theLists;   // The array of Lists
};

int hash( const string & key, int tableSize );
int hash( int key, int tableSize );
#endif
