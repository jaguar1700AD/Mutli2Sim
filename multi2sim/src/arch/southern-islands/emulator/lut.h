#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

class lut
{
	struct row
	{
		float data[2];
	};
	
	
	list <row> table;
	
	unsigned int size;	
	float epsilon;

	unsigned long int num_hits;
	unsigned long int num_misses;
	
	vector <bool> recent_hits; 
	
	float abs_diff(row v1, row v2) { return abs(v1.data[0] - v2.data[0]) + abs(v1.data[1] - v2.data[1]); }
	list<row>::iterator get_closest(row V);
	void print_value(list<row>::iterator it) { cout << (*it).data[0] << ", " << (*it).data[1] << endl;}

	

public:

	lut(unsigned int size, float epsilon);
	
	// Return true and closest value for a hit	
	bool find(float a, float b, float &store1, float &store2);

	void print();

	double get_hit_rate() { return ((double) num_hits) / (num_hits + num_misses) ; }
	unsigned long int get_num_hits() { return num_hits; }
	unsigned long int get_num_misses() { return num_misses; }
	unsigned long int get_num_ops() { return num_hits + num_misses; }
	vector <bool> get_recent_hits() {vector <bool> vec = recent_hits; recent_hits.clear(); return vec; } // Automatically clear stored recent_hits when they are read

};
