// Gokhan Demir 26562 CS300 Data Structures HW3 Sabanci University 2020-2021 Fall
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "mpq.h"
using namespace std;

int main()
{
	ifstream inputFile; // For reading from the file.
	int k = 0, l = 0, indexval = 1, right, left, size, num, tag, height, last, id; // Needed variables.
	inputFile.open("input.txt"); // Since the file is named input.txt
	cout << "-----------" << endl;
	if(inputFile.is_open() == false) // Could not open it.
	{
		cout << "Unable to open the file. Try again later." << endl;

	}
	else // Could open it, proceed.
	{
		inputFile >> id;
		if (id == 0) // If there are no buildings, it is terminating.
		{
			inputFile.close(); // Closing the file.
			cout << 0 << " " << 0 << endl;
			return 0;
	    }
		size = (1+(2*id));
		coord* bps = new coord[size];
		coord* bpfirst = new coord(0, 0, 0, true);
		bps[0] = *bpfirst;
		for (k = 0; k < id; k++) // Taking the inputs from the file and inserting into the array.
		{
			// Extracting coordinates and height of the skyscraper.
			tag = indexval;
			inputFile >> left >> num >> right;
			bpfirst = new coord(right, tag, num, false);
			bps[indexval + 1] = *bpfirst;
			bpfirst = new coord(left, tag, num, true);
			bps[indexval] = *bpfirst;
			indexval=indexval+2;
		}
		sort(bps, (bps + size)); // Sorting the array from starting coordinates to ending coordinates.
		MPQ myheap(size);

		for (l = 0; l < size; l++) // This is the loop for printing the building values.
		{
			if (bps[l].begin)
			{
				height = bps[l].point;
				last = myheap.maximum();
				myheap.insertion(height, bps[l].tag);

				/**
				* Output the building if the maximum value has changed.
				*/
				if (last != myheap.maximum())
				{
					cout << bps[l].var << " " << height << "\n";
				}
			}
			else 
			{
				last = myheap.maximum();
				height = bps[l].point;
				myheap.Remove(bps[l].tag);

				/**
				* Output the building if the maximum value has changed.
				*/
				if (myheap.maximum() != last)
				{
					cout << bps[l].var << " " << myheap.maximum() << "\n";
				}
			}
		}
	}
	inputFile.close(); // Closing the file.
	cout << "-----------" << endl;
	return 0;
}