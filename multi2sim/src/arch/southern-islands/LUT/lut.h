#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

class lut
{
	union value
	{
		unsigned int as_uint;
		int as_int;
	};
	
	struct classcomp 
	{
  		bool operator() (value v1, value v2) { return v1.as_uint < v2.as_uint; }
	};
	
	
	set <value, classcomp> table;
	unsigned int size;	
	unsigned int epsilon;

	unsigned long int num_hits;
	unsigned long int num_misses;
	
	unsigned int abs_diff(value v1, value v2) { return abs(v1.as_int - v2.as_int); }
	set<value>::iterator get_closest(value V);
	void print_value(set<value>::iterator it) { cout << (*it).as_uint << endl;}

	

public:

	lut(unsigned int size, unsigned int epsilon);
	
	// Return true and closest value for a hit	
	bool find(unsigned int V, unsigned int &store);

	void print();

	double get_hit_rate() { return ((double) num_hits) / (num_hits + num_misses) ; }
	unsigned long int get_num_hits() { return num_hits; }
	unsigned long int get_num_misses() { return num_misses; }
	unsigned long int get_num_ops() { return num_hits + num_misses; }

};
