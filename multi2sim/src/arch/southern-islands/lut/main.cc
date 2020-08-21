#include <bits/stdc++.h>
#include "lut.h"

int main()
{
	lut table(4, 1);
	float store1, store2;

	vector <float> A {9.63, 7.9, 6.1, 8.0, 20.01, 11.37};
	vector <float> B {2.57, 3.03, 9.7, 3.03, 20.66, 1.0};
	
	for(int i = 0; i < A.size(); i++)
	{
		//cout << i << " iteration" << endl;
		table.find(A[i], B[i], store1, store2);
		cout << A[i] << " " << B[i] << " " << store1 << " " << store2 << " Hit/Miss: " << table.get_recent_hit() << endl;	
	}	

	table.print();
	cout << table.get_num_hits() << " " << table.get_num_misses() << endl;
}
