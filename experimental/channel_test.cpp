

#include "Channel.hpp"
#include "VoltageGatedChannel.hpp"
#include "VoltageGate.hpp"




namespace prinz::NaV {
	double m_inf(double V) {
		return 1.0/(1.0+exp((V+25.5)/-5.29));
	}

	double tau_m(double V) {
		return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));
	}
}




int main() {

	channel NaV = VoltageGatedChannel(100,50);
	NaV.gates.push_back(prinz::NaV::m_inf);
	NaV.gates.push_back(prinz::NaV::tau_m);


	cout << "NaV.m_inf(30) = " << NaV.m_inf(-50) << endl;


	// leads to a runtime error
	cout << "calling undefined h_inf = " << NaV.h_inf(-50) << endl;

}

void channel::set_m_inf(string m) {
	this->m_inf = prinz::NaV_m_inf;
}


double channel::evaluate() {
	return m_inf(1.0);
}