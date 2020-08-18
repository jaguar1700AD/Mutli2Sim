#include "lut.h"

lut::lut(unsigned int size, unsigned int epsilon)
{
	this->size = size;
	this->epsilon = epsilon;
	num_hits = 0;
	num_misses = 0;
}

set<lut::value>::iterator lut::get_closest(elem V)
{
	if (table.size() == 0) return table.end();
	
	auto candidate = table.begin();
	for(auto it = table.begin(); it != table.end(); it++)
	{
		if (abs_diff(*it, V) < abs_diff(*candidate, V)) candidate = it;
	}
	return candidate;
}

set <lut::value>::iterator lut::get_LRU()
{
	return table.begin();
}

void lut::change_elem_time(set<elem>::iterator it, unsigned int new_time)
{
	elem e = *it; e.last_used = new_time;
        table.erase(it);
        table.insert(e);
}

void lut::make_old()
{
	for(auto it = table.begin(); it != table.end(); it++) 
	{
		elem e = *it; e.last_used += 1;
       		table.erase(it);
       		table.insert(it, e); // Hint provided for fast O(1) insertion
	}
}

bool lut::find(unsigned int a, unsigned int b, unsigned int &store1, unsigned int &store2)
{
	elem val = new elem(a, b, 0);
	auto it = get_closest(val);
	
	if (it == table.end() || abs_diff(*it, val) > epsilon)
	{
		// Miss
		
		if (table.size() == size) table.erase(table.get_LRU());
		table.insert(table.end(), val);
		num_misses += 1;
		
		make_old();
		recent_hit = false;
		return false; 
	}
	
	// Hit	
	num_hits += 1;
	change_elem_time(it, 0);

	make_old();	
	recent_hit = true;
	store1 = (*it).as_uint[0]; store2 = (*it).as_uint[1];
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
