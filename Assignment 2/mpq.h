// Gokhan Demir 26562 CS300 Data Structures HW3 Sabanci University 2020-2021 Fall
#ifndef MPQ_H
#define MPQ_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>

struct element // This is for elements of the modified priority queue.
{
	int tag;
	int num;
	element(){} // Default constructor
	element(int numm, int tagg)
	{
		num=numm;
		tag=tagg;
	}
};

struct coord // This is for arrays which store coordinates.
{
	int tag;
	int var;
	int point;
	bool begin;
	coord(){}
	coord(int num1, int num2, int num3, bool condition)
	{
		var=num1;
		tag=num2;
		point=num3;
		begin=condition;
	}
};

class MPQ // Modified Priority Queue Class
{
	public:
		MPQ(const int size):mpq(new element[size]), loc(new int[size]), size(size), curr(0)
		{}
		~MPQ() // Destructor
		{
			delete [] loc; // Deleting dynamically allocated area which is pointed by loc pointer.
		}
		void insertion(int, int);
		int Remove(int);
		void percolateDown(int);
		int maximum();
		bool check();
	private:
		const int size;
		int* loc; // It is a pointer to integer(s).
		int tmp;
		int curr;
		element* mpq; // It is a pointer to element structure(s).
};

bool MPQ::check() // Checks if the heap is empty.
{
	return(curr==0); // If the current size is 0, then it means that the heap is empty and it is returning true.
}

int MPQ::maximum() // Returns the maximum value if there exists any.
{
	if(check()==false) // If the heap isn't empty
	{
		return mpq[1].num;
	}
	else // Heap is empty.
	{
		return -1;
	}
}

int MPQ::Remove(int tag) // Function that removes the element with tag and returns its value.
{
	int tagloc = loc[ tag ];
	int tmp = mpq[ tagloc ].num;

	if ( check( ) || !( loc[ tag ] ) )
	{
		return 0;
	}

	mpq[ tagloc ] = mpq[ curr-- ];
	loc[ mpq[ curr + 1 ].tag ] = tagloc;
    percolateDown( tagloc );
	tmp = mpq[ 1 ].num;	// 'tmp' stores the maximum value.

	return tmp;
}

void MPQ::percolateDown(int val) // Function which perlocates down the modified heap.
{
	int descendant;
	int tmp = mpq[ val ].num;
	int tmptag = mpq[ val ].tag;

	do
	{
		descendant = ( val * 2 );

		if( ( descendant != curr ) && ( mpq[ descendant + 1 ].num > mpq[ descendant ].num ) )
		{ 
			descendant++;
		}	
			
		else if( mpq[ descendant ].num > tmp )
		{ 
			mpq[ val ] = mpq[ descendant ];
			loc[ mpq[ descendant ].tag ] = val;
		}

		else
		{ 
			break;
		}

		val = descendant;
	} while( ( val * 2 ) <= curr );

	mpq[ val ].tag =  tmptag;					// After each operation, update the values accordingly.
	loc[ tmptag ] = val;
	mpq[ val ].num = tmp;
}

/**
* Function that inserts the current element into the modified priority queue.
* Creates a hole value and starts perlocation, updates the location variable after each perlocate.
* After execution, sets the hole values appropriately and updates the location.
*/
void MPQ::insertion(int t, int tag)
{
	curr=curr+1;
	int newval = curr; // Integer that holds the current hole value.

	while (( t > mpq[ newval / 2 ].num) && ( newval > 1 ))
	{
		mpq[ newval ] = mpq[ newval / 2 ];
		loc[ mpq[ newval / 2 ].tag ] = newval;
		newval /= 2;
	}
	mpq[ newval ].num = t;
	mpq[ newval ].tag = tag;
	loc[ tag ] = newval;
	// Returns nothing, since it is void.
}

bool operator<(const coord & left, const coord & right) // Operator overloading for comparing the start/end points of skyscrapers.
{
	if ( left.var < right.var )
	{ 
		return true;
	}

	else if ( left.var == right.var )
	{	
		/**
		* Condition for putting the taller building before the shorter one (for comparing their start points).
		*/
		if ( ( right.begin ) && ( left.begin ) )
		{ 
			return ( right.point < left.point );
		}

		/**
		* Condition for putting the shorter building before the taller one (for comparing their end points).
		*/
		else if ( !( right.begin ) && !( left.begin ) )
		{ 
			return ( right.point > left.point );
		}
		
		/**
		* Condition for comparing the start and end points of two buildings.
		* Places the building with start point before the one with the end point.
		*/
		else
		{
			return ( left.begin );
		}
	}
	else
	{
		return false; // Returning false if the left hand side is taller than the right hand size.
	}
}
#endif