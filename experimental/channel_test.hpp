
#include <functional>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

class channel {

int p = 3;
int q = 1;
public:
	string name = "unset";

	double E = 50;
	double g = 0;
	double gbar = 0;

	std::function<double(double)> m_inf;
	std::function<double(double)> h_inf;

	channel(double gbar, double E, int p, int q) {
		this->gbar = gbar;
		this->E = E;
		this->p = p;
		this->q = q;
	}

	void set_m_inf(string);

	double evaluate(void);

};

