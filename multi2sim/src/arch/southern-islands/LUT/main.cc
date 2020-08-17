#include "lut.h"

int main()
{
	lut table(10, 2);
	unsigned int store;

	table.find(10, store); cout << store << endl;
	table.find(15, store); cout << store << endl;
	table.find(11, store); cout << store << endl;
	table.find(20, store); cout << store << endl;
	table.find(30, store); cout << store << endl;
	table.find(21, store); cout << store << endl;
	table.find(10, store); cout << store << endl;
		
	table.print();
	cout << table.get_num_hits() << " " << table.get_num_misses() << endl;
}
