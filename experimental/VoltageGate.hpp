// This class describes an abstract
// voltage gate. You can include
// multiple voltage gates in a VoltageGatedChannel
// 


#include <functional>
#include <string>
#include <cmath>


class VoltageGate {
	

public:
	std::function<double(double)> x_inf;
	std::function<double(double)> tau_x;

	int p;

	double x = 1; 

	VoltageGate(){};

	VoltageGate(std::function<double(double)> x_inf, std::function<double(double)> tau_x, int p) {
		this->x_inf = x_inf;
		this->tau_x = tau_x;
		this->p = p;

	};

}; 