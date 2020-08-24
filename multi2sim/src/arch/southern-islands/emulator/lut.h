#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

class lut
{
	union value
	{
		unsigned int as_uint[2];
		float as_float[2];
	};
	
	class elem
	{
	public:
		value data; unsigned int last_used;
		elem(float a, float b, unsigned int last_used)
		{
			data.as_float[0] = a; data.as_float[1] = b;
			this->last_used = last_used;
		}
	};
	
	struct comp {bool operator() (elem e1, elem e2) {return e1.last_used > e2.last_used; } };	

	set <elem, comp> table;
	unsigned int size;	
	float epsilon;

	unsigned long int num_hits;
	unsigned long int num_misses;
	
	vector <bool> recent_hits; 
	
	float abs_diff(elem v1, elem v2) { return abs(v1.data.as_float[0] - v2.data.as_float[0]) + abs(v1.data.as_float[1] - v2.data.as_float[1]); }
	set<elem>::iterator get_closest(elem V);
	set<elem>::iterator get_LRU();
	void change_elem_time(set<elem>::iterator it, unsigned int new_time); // Change the last_used of an lut element
	void change_elem_time(set<elem>::iterator it, unsigned int new_time, set<elem>::iterator hint);
	void make_old(); // Increase the last_used of all elements by 1
	void print_value(set<elem>::iterator it) { cout << "Data: " << (*it).data.as_float[0] << ", " << (*it).data.as_float[1] << " Last Used: " << (*it).last_used << endl;}

	

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
