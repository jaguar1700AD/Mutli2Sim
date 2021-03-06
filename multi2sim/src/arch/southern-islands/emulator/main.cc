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
		cout << A[i] << " , " << B[i] << " , " << store1 << " ,  " << store2 << endl;	
	}	

	table.print();
	vector <bool> hits = table.get_recent_hits();
	cout << "Recent Hits Size: " << hits.size() << endl;
	for (int i = 0; i < A.size(); i++) cout << hits[i] << " ";
	cout << endl;
	cout << "Recent Hits Size: " << table.get_recent_hits().size() << endl;
	cout << table.get_num_hits() << " " << table.get_num_misses() << endl;
}
