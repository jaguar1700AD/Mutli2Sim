#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

class lut
{
	union value
	{
		unsigned int as_uint[2];
		int as_int[2];
	};
	
	class elem
	{
	public:
		value data; unsigned int last_used;
		elem(unsigned int a, unsigned int b, unsigned int last_used)
		{
			data.as_uint = {a, b};
			this->last_used = last_used;
		}
	};
	
	struct comp {bool operator() (elem e1, elem e2) {return e1.last_used > e2.last_used; } };	

	set <elem, comp> table;
	unsigned int size;	
	unsigned int epsilon;

	unsigned long int num_hits;
	unsigned long int num_misses;
	
	bool recent_hit; // True if most recent access was a hit
	
	unsigned int abs_diff(elem v1, elem v2) { return abs(v1.data.as_int[0] - v2.data.as_int[0]) + abs(v1.data.as_int[1] - v2.data.as_int[1]); }
	set<value>::iterator get_closest(value V);
	set<value>::iterator get_LRU();
	void make_old(); // Increase the last_used of all elements by 1
	void print_value(set<value>::iterator it) { cout << "Data: " << (*it).data.as_uint[0] << ", " << (*it).data.as_uint[1] << " Last Used: " << (*it).last_used << endl;}

	

public:

	lut(unsigned int size, unsigned int epsilon);
	
	// Return true and closest value for a hit	
	bool find(unsigned int a, unsigned int b, unsigned int &store1, unsigned int &store2);

	void print();

	double get_hit_rate() { return ((double) num_hits) / (num_hits + num_misses) ; }
	unsigned long int get_num_hits() { return num_hits; }
	unsigned long int get_num_misses() { return num_misses; }
	unsigned long int get_num_ops() { return num_hits + num_misses; }
	bool get_recent_hit() {return recent_hit; }

};
