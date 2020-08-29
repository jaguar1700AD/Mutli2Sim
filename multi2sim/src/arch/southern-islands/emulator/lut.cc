#include "lut.h"

lut::lut(unsigned int size, float epsilon)
{
	this->size = size;
	this->epsilon = epsilon;
	num_hits = 0;
	num_misses = 0;
}

list<lut::row>::iterator lut::get_closest(row V)
{
	if (table.size() == 0) return table.end();
	
	auto candidate = table.begin();
	for(auto it = table.begin(); it != table.end(); it++)
	{
		if (abs_diff(*it, V) < abs_diff(*candidate, V)) candidate = it;
	}
	return candidate;
}


bool lut::find(float a, float b, float &store1, float &store2)
{
	struct row new_row; new_row.data[0] = a; new_row.data[1] = b;
	auto it = get_closest(new_row);
	
	if (it == table.end() || abs_diff(*it, new_row) > epsilon)
	{
		// Miss
		if (table.size() == size) table.pop_front();
		table.push_back(new_row);
		num_misses += 1;
		
		return false; 
	}
	
	// Hit	
	num_hits += 1;
	store1 = (*it).data[0]; store2 = (*it).data[1];
	
	row hit_row = *it;
	table.erase(it);
	table.push_back(hit_row);
	
	return true;
}

void lut::print()
{	
	cout << "\n...... LUT Data ......" << endl;
	for(auto it = table.begin(); it != table.end(); it++)
	{
		print_value(it);
	}
	cout << endl;
}	
