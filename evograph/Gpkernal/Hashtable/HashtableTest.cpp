// HashtableTest.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "hashtable.h"


int main(int argc, char* argv[])
{
	
	printf("Hello World!\n");

	HashTable  ht(7);

	for(int i=0;i<20;i++){
		CStruct* pS = new CStruct;
		pS->structID = i;
		ht.insert(pS);
	}
	ht.ShowAllStruct();

	CStruct* pSt = ht.find(5);
	printf("find  struct %d\n", pSt->structID);

//	ht.ShowAllStruct();


	ht.Rehash(25);
	ht.ShowAllStruct();

	pSt = ht.find(5);
	printf("find  struct %d  after rehash\n", pSt->structID);

	ht.ShowAllStruct();

//	HashTable ht1(ht);
//	pSt = ht1.find(56);
	if(pSt==NULL)
		printf(" struct not found\n");
	else
		printf(" find struct %d !\n", pSt->structID);

//	ht.remove(pSt);
	//ht.remove(56);
/*	pSt = ht.find(56);
	if(pSt==NULL)
		printf(" struct not found\n");
	else
		printf(" find struct %d !\n", pSt->structID);

	ht.ShowAllStruct();
*/
	HashTable h1(ht);
	pSt = h1.find(5);
	printf("find  struct %d  in h1 \n", pSt->structID);


	return 0;
}

