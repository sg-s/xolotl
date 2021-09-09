// VoltageGatedChannel.hpp
// describes an abstract voltage-gated channel


#include <vector>

#pragma once

class VoltageGatedChannel: public Channel {


public:
	vector<VoltageGate> gates;

	VoltageGatedChannel(double gbar, double E) {
		this->gbar = gbar;
		this->E = E;
	}

};