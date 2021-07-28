
#include <iostream>
#include "thing.hpp"
#include "mycontainer.hpp"
#include "method_definitions.cpp"

using namespace std;


int main()
{

	mycontainer X;
	thing T;
	X.addThing(&T);

	cout << "thing value before init =  " <<  T.value << endl;

	X.init();
	
	cout << "thing value after init = " << T.value << endl;


	cout << "asking thing to reset container...\n";
	T.resetContainerValue();

	cout << "Containers value now = " << X.value << endl;
}