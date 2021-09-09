
#include <functional>
#include <string>
#include <cmath>
#include <iostream>

#pragma once

using namespace std;

class Channel {


public:
	string name = "unset";

	double E = 50;
	double g = 0;
	double gbar = 0;

	channel(double gbar, double E) {
		this->gbar = gbar;
		this->E = E;
	}


};
