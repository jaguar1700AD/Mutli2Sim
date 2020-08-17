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
	if (table.size() == 0) return table.end();

	auto it = table.lower_bound(V);
	if (it == table.begin()) return it;
	auto it1 = prev(it);

	if (abs_diff(*it, V) < abs_diff(*it1, V)) return it;
	else return it1;
}

bool lut::find(unsigned int V, unsigned int &store)
{
	value val; val.as_uint = V;
	auto it = get_closest(val);
	
	if (it == table.end() || abs_diff(*it, val) > epsilon)
	{
		// Miss
		
		if (table.size() == size) table.erase(it);
		table.insert(val);
		num_misses += 1;
		return false; 
	}
	
	// Hit	

	num_hits += 1;
	store = (*it).as_uint;
	return true;
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
