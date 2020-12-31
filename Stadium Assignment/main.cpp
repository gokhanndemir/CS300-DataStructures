#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Stadium
{
	public:
		Stadium(vector <string> block, vector <string> row, int colnums) // Constructor with block vector, row vector, and number of columns.
		{
			for (string b:block)
			{
				whole[b];
				for (string r:row)
				{
					whole[b][r];
					for(int i=0;i<colnums;i++)
					{
						whole[b][r][i];
						whole[b][r][i]="---"; // Initializing
					}
				}
			}
			for(int k=0; k<block.size(); k++)
			{
				Order[block[k]] = k; // Ordering blocks from 0 to number of blocks respectively.
			}
			for(int i = 0; i < row.size(); i++)
			{
				vector <pair <string, int>> w;
				for(int j = 0; j < block.size(); j++)
				{
					w.push_back(make_pair(block[j], colnums));
					Order[block[j]] = j;
				}
				PQ[row[i]] = w; // Into the priority queue.
			}
		}
		void print(ofstream & file) // The time complexity of this operation is O(B*R*C).
		{
			file << endl;
			unordered_map<string, unordered_map<string, unordered_map<int, string>>>::iterator ptr1;
			unordered_map<string, unordered_map<int, string>>::iterator ptr2;
			unordered_map<int, string>::iterator ptr3;
			for (ptr1 = whole.begin(); ptr1 != whole.end(); ptr1++)
			{
				file << ptr1->first << endl << "~~~~~~~" << endl;
				for (ptr2 = ptr1->second.begin(); ptr2!=ptr1->second.end(); ptr2++)
				{
					file << ptr2->first << " : ";
					for (ptr3 = ptr2->second.begin(); ptr3 != ptr2->second.end(); ptr3++)
					{
						file << ptr3->second << " ";
					}
					file << endl;
				}
				file << "=======" << endl << endl;
			}
		}
		void reserve_seat_by_row(string name, string row, ofstream & file) // The time complexity of this operation is O(Log B + C ).
		{
			if(PQ[row][0].second == 0 || reservations.find(name) != reservations.end())
			{
				file << name << " could not reserve a seat!" << endl;
				return;
			}

			string block = PQ[row][0].first;

			for(auto i : whole[block][row])
			{
				if(i.second == "---")
				{
					whole[block][row][i.first] = name.substr(0, 3);
					file << name << " has reserved " << block << " " << row << "-" << i.first << " by emptiest block" << endl;
					tuple <string, string, int> temp;
					get<0>(temp) = block;
					get<1>(temp) = row;
					get<2>(temp) = i.first;
					reservations[name] = temp;
					PQ[row][0] = make_pair(block, PQ[row][0].second - 1);
					break;
				}
			}

			int i = 0;
			vector< pair <string, int>> arr = PQ[row];

			// Percolate Down
			while(i < arr.size())
			{
				if(2*i + 1 == arr.size() - 1)
					bool s = false;
				else if((2*i + 1) >= arr.size() || (arr[i].second >= arr[2*i + 1].second && arr[i].second >= arr[2*i + 2].second))
					break;
				int other_index;
				if(2*i + 2 < arr.size())
				{
					if(arr[2*i + 1].second == arr[2*i + 2].second)
						other_index = Order[arr[2*i + 1].first] < Order[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
					else
						other_index = arr[2*i + 2].second > arr[2*i + 1].second ? 2*i + 2 : 2*i + 1;
				}
				else
				other_index = 2*i + 1;

				index[row][arr[i].first] = other_index;
				index[row][arr[other_index].first] = i;
		
				pair <string, int> tmp = arr[i];
				arr[i] = arr[other_index];
				arr[other_index] = tmp;

				i = other_index;
			}
		
			while(i < arr.size())
			{
				if(2*i + 1 == arr.size() - 1)
					bool s = false;
				else if((2*i + 1) >= arr.size() || (arr[i].second > arr[2*i + 1].second && arr[i].second > arr[2*i + 2].second) || (Order[arr[i].first] < Order[arr[2*i + 1].first] && Order[arr[i].first] < Order[arr[2*i + 2].first]))
					break;

				int other_index;
				if(2*i + 2 < arr.size())
				{
					if(arr[2*i + 1].second == arr[2*i + 2].second)
						other_index = Order[arr[2*i + 1].first] < Order[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
					else
						other_index = arr[2*i + 1] == arr[i] ? 2*i + 1 : 2*i + 2;
				}
				else
					other_index = 2*i + 1;

				index[row][arr[i].first] = other_index;
				index[row][arr[other_index].first] = i;
				pair <string, int> tmp = arr[i];
				arr[i] = arr[other_index];
				arr[other_index] = tmp;
				i = other_index;
			}
			PQ[row] = arr;
		}
		void reserve_seat(string name, string block, string row, int column, ofstream & file) // The time complexity of this operation is O(LogB).
		{
			int ind1 = index[row][block]; // Getting index
			if(PQ[row][ind1].second == 0 || reservations.find(name) != reservations.end() || whole[block][row][column] != "---") // Checking conditions.
			{
				file << name << " could not reserve a seat!" << endl; // Couldn't reserve a seat.
				return;
			}
			tuple <string, string, int> temp;
			get<0>(temp) = block;
			get<1>(temp) = row;
			get<2>(temp) = column;
			reservations[name] = temp;
			whole[block][row][column] = name.substr(0, 3);
			PQ[row][ind1] = make_pair(block, PQ[row][ind1].second - 1);
			int i = ind1;
			vector< pair <string, int>> arr = PQ[row];
			while(arr.size()>i)
			{
				if(2*i + 1 == arr.size() - 1)
					bool s = false;
				else if((2*i + 1) >= arr.size() || (arr[i].second >= arr[2*i + 1].second && arr[i].second >= arr[2*i + 2].second))
					break;
				int other_index;
				if(2*i + 2 < arr.size())
				{
					if(arr[2*i + 1].second == arr[2*i + 2].second)
						other_index = Order[arr[2*i + 1].first] < Order[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
					else
						other_index = arr[2*i + 2].second > arr[2*i + 1].second ? 2*i + 2 : 2*i + 1;
				}
				else
					other_index = 2*i + 1;
				index[row][arr[i].first] = other_index;
				index[row][arr[other_index].first] = i;
		
				pair <string, int> tmp = arr[i];
				arr[i] = arr[other_index];
				arr[other_index] = tmp;
				i = other_index;
			}

			while(arr.size()>i)
			{
				if(2*i + 1 == arr.size() - 1)
					bool s = false;
				else if((2*i + 1) >= arr.size() || (arr[i].second > arr[2*i + 1].second && arr[i].second > arr[2*i + 2].second) || (Order[arr[i].first] < Order[arr[2*i + 1].first] && Order[arr[i].first] < Order[arr[2*i + 2].first]))
					break;
				int other_index;
				if(2*i + 2 < arr.size())
				{
					if(arr[2*i + 1].second == arr[2*i + 2].second)
						other_index = Order[arr[2*i + 1].first] < Order[arr[2*i + 2].first] ? 2*i + 1 : 2*i + 2;
					else
						other_index = arr[2*i + 1] == arr[i] ? 2*i + 1 : 2*i + 2;
				}
				else
					other_index = 2*i + 1;

				index[row][arr[i].first] = other_index;
				index[row][arr[other_index].first] = i;

				pair <string, int> tmp = arr[i];
				arr[i] = arr[other_index];
				arr[other_index] = tmp;

				i = other_index;
			}

			PQ[row] = arr;
			file << name << " has reserved " << block << " " << row << "-" << column << endl;
		}
		void get_seat(string name, ofstream & file) // The time complexity of this operation is O(1).
		{
			if(reservations.find(name) != reservations.end()) // Found.
			{
				tuple <string, string, int> tuple1 = reservations[name]; // tuple1 is equal to name's corresponding in unordered_map.
				file << "Found that " << name << " has a reservation in " << get<0>(tuple1) << " " << get<1>(tuple1) << "-" << get<2>(tuple1) << "\n";
	        }
			else // Couldn't find.
			{
				file << "There is no reservation made for " << name << "!" << "\n";
			}
		}
		void cancel_reservation(string name, ofstream & file) // The time complexity of this operation is O(LogB).
		{
			if(reservations.find(name) == reservations.end()) // If cannot find
			{
				file << "Could not cancel the reservation for " << name << "; no reservation found!" << endl;
				return;
			}
			tuple <string, string, int> temp = reservations[name];
			string block = get<0>(temp), row = get<1>(temp);
			int column = get<2>(temp);
			reservations.erase(name);
			whole[block][row][column] = "---";
			file << "Cancelled the reservation of " << name << endl;
	
			int idx = index[row][block];
			PQ[row][idx] = make_pair(block, PQ[row][idx].second + 1);

			vector< pair <string, int>> arr = PQ[row];
			int i = idx, j = (i - 1)/ 2;

			while(j > 0 && arr[j].second < arr[i].second)
			{
				pair <string, int> tmp = arr[j];

				index[row][arr[i].first] = j;
				index[row][arr[j].first] = i;

				arr[j] = arr[i];
				arr[i] = tmp;
				i = j;
				j = (j - 1) / 2;
			}

			if(arr[i].second>arr[j].second)
			{
				pair <string, int> tmp = arr[j];

				index[row][arr[i].first] = j;
				index[row][arr[j].first] = i;

				arr[j] = arr[i];
				arr[i] = tmp;

				i = j;
				j = (j - 1) / 2;
			}
			while(j > 0)
			{
				if(arr[j].second < arr[i].second)
					break;
				else if(arr[i].second == arr[j].second && Order[arr[i].first] > Order[arr[j].first])
					break;
				pair <string, int> tmp = arr[j];
				index[row][arr[i].first] = j;
				index[row][arr[j].first] = i;
				arr[j] = arr[i];
				arr[i] = tmp;
				i = j;
				j = (j - 1) / 2;
			}

			if(arr[j].second == arr[i].second && Order[arr[i].first] < Order[arr[j].first])
			{
				pair <string, int> tmp = arr[j];
				index[row][arr[i].first] = j;
				index[row][arr[j].first] = i;
				arr[j] = arr[i];
				arr[i] = tmp;
				i = j;
				j = (j - 1) / 2;
			}
			PQ[row] = arr;
		}
	private:
		unordered_map <string, tuple <string, string, int>> reservations; // NAME->tuple(BLOCK-ROW-COLUMN)
		unordered_map <string, unordered_map<string, int>> index; // ROW-BLOCK-> INDEX
		unordered_map <string, int> Order; // BLOCK -> NUMBER, for assigning numbers to blocks from 0 to number of blocks.
		unordered_map <string, unordered_map<string, unordered_map<int, string>>> whole; // BLOCK-ROW-COLUMN->NAME (Stadium)
		unordered_map <string, vector< pair <string, int> >> PQ; // My priority queue.
};

int main()
{
	ifstream rfile;
	rfile.open("inputs.txt");
	ofstream wfile;
	wfile.open("output.txt");
	vector<string> blocks, rows;
	if (rfile.is_open())
	{
		string satir,word;
		getline(rfile,satir);
		stringstream f(satir);
		while(f>>word)
		{
			blocks.push_back(word);
		}
		getline(rfile,satir);
		stringstream s(satir);
		while(s>>word)
		{
			rows.push_back(word);
		}
		getline(rfile,satir);
		int NumOfCols=stoi(satir);
		Stadium stadium(blocks,rows,NumOfCols); // The stadium is initialized. Now, it is time for remaining lines in txt file.
		while(!rfile.eof())
		{
			getline(rfile,satir);
			stringstream m1(satir);
			if(satir=="print")
			{
				stadium.print(wfile);
			}
			else if(satir.substr(0,19)=="reserve_seat_by_row")
			{
				string unnecessary, row, name;
				m1>>unnecessary>>name>>row; // Reading name and row.
				stadium.reserve_seat_by_row(name,row,wfile);
			}
			else if(satir.substr(0,12)=="reserve_seat")
			{
				string unnecessary, name, block, row;
				int column;
				m1 >> unnecessary >> name >> block >> row >> column;
				stadium.reserve_seat(name, block, row, column, wfile);
			}
			else if(satir.substr(0,8)=="get_seat")
			{
				string unnecessary, name;
				m1 >> unnecessary >> name;
				stadium.get_seat(name, wfile);
			}
			else if(satir.substr(0,18)=="cancel_reservation")
			{
				string unnecessary, name;
				m1 >> unnecessary >> name;
				stadium.cancel_reservation(name, wfile);				
			}
		}
	}
	else
	{
		cout << "Unable to open the file." << endl;
	}
	wfile.close();
	rfile.close();
	return 0;
}