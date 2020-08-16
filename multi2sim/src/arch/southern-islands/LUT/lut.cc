#include "lut.h"

lut::lut(unsigned int size, unsigned int epsilon)
{
	this->size = size;
	this->epsilon = epsilon;
	num_hits = 0;
	num_misses = 0;
}

set<lut::value>::iterator lut::get_closest(value V)
{
	if (table.size() == 0) return NULL;

	auto it = table.lower_bound(V);
	if (it == V.begin()) return it;
	auto it1 = prev(it);

	if (abs_diff(*it, V) < abs_diff(*it1, V)) return it;
	else return it1;
}

bool lut::find(value V)
{
	auto it = get_closest(V);
	
	if (it == NULL || abs_diff(*it, V) > epsilon)
	{
		// Miss

		if (table.size() == size) table.erase(it);
		table.insert(V);
		num_misses += 1;
		return NULL; 
	}
	
	// Hit	

	num_hits += 1;
	return *it;
}

void lut::print_value(set<lut::table>::iterator it)
{
	cout << *it << endl;
}

void lut::print()
{	
	cout << "...... LUT Values ......" << endl;
	for(auto it = table.begin(); it != table.end(); it++)
	{
		print_value(it);
	}
	cout << endl;
}	
