#include "lut.h"

lut::lut(unsigned int size, float epsilon)
{
	this->size = size;
	this->epsilon = epsilon;
	num_hits = 0;
	num_misses = 0;
}

set<lut::elem>::iterator lut::get_closest(elem V)
{
	if (table.size() == 0) return table.end();
	
	auto candidate = table.begin();
	for(auto it = table.begin(); it != table.end(); it++)
	{
		if (abs_diff(*it, V) < abs_diff(*candidate, V)) candidate = it;
	}
	return candidate;
}

set <lut::elem>::iterator lut::get_LRU()
{
	return table.begin();
}

void lut::change_elem_time(set<elem>::iterator it, unsigned int new_time)
{
	elem e = *it; e.last_used = new_time;
        table.erase(it);
        table.insert(e);
}

void lut::change_elem_time(set<elem>::iterator it, unsigned int new_time, set<elem>::iterator hint)
{
        elem e = *it; e.last_used = new_time;
	if (hint == it) hint = next(it);
        table.erase(it);
        table.insert(hint, e);
}


void lut::make_old()
{
	for(auto it = table.begin(); it != table.end(); it++) 
	{
		elem e((*it).data.as_float[0], (*it).data.as_float[1], (*it).last_used + 1);
		auto hint = next(it);
       		table.erase(it);
       		table.insert(hint, e); // Hint provided for fast O(1) insertion
	}
}

bool lut::find(float a, float b, float &store1, float &store2)
{
	elem val(a, b, 0);
	auto it = get_closest(val);
	
	if (it == table.end() || abs_diff(*it, val) > epsilon)
	{
		// Miss
		if (table.size() == size) table.erase(get_LRU());
		table.insert(table.end(), val);
		num_misses += 1;
		
		make_old();
		recent_hits.push_back(false);
		return false; 
	}
	
	// Hit	
	num_hits += 1;
	store1 = (*it).data.as_float[0]; store2 = (*it).data.as_float[1];
	change_elem_time(it, 0, table.end());
	make_old();	
	recent_hits.push_back(true);

	return true;
}

void lut::print()
{	
	cout << "\n...... LUT Values ......" << endl;
	for(auto it = table.begin(); it != table.end(); it++)
	{
		print_value(it);
	}
	cout << endl;
}	
