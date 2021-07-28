#include <iostream>
using namespace std;


int main()
{
	

	double* V_data;

	int n_steps = 100;
	delete V_data;
	V_data = new double[n_steps];

	for (int i = 0; i < n_steps; i ++) {
		V_data[i] = i;
	}

	cout << "V_data element = " << V_data[99] << endl;

	// delete 
	delete V_data;



	n_steps = 1000;
	V_data = new double[n_steps];


	delete V_data;

};