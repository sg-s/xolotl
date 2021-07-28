

// this C++ file contains all the method
// definitions for classes defined in header files



#include "xolotl.hpp"
#include "compartment.hpp"
#include "conductance.hpp"

compartment::compartment(string name_) {
  this->name = name_;
  this->init();
}



void compartment::add(conductance* cond_) {
	allchannels.push_back(cond_);
	n_channels++;
	cond_->container = this;
}


void xolotl::add(compartment * comp_) {
    this->allcompartments.push_back(comp_);
    this->n_compartments++;
}

void xolotl::init() {

    // broadcast parameters to children
    if (this->n_compartments == 0) {
        return;
    }

    for (int i = 0; i < this->n_compartments; i++) {
        allcompartments[i]->dt = this->dt;
    }
}


void xolotl::integrate() {
    if (this->n_compartments == 0) {
        return;
    }

    for (int i = 0; i < this->n_compartments; i++) {
        allcompartments[i]->integrate();
    }

}


compartment* xolotl::compartments(int i) {
    return this->allcompartments.at(i);
}



void conductance::step() {   


    V = container->V_prev;

    minf = m_inf(V);
    hinf = h_inf(V);

    m = minf + (m - minf)*exp(-dt/tau_m(V));
    h = hinf + (h - hinf)*exp(-dt/tau_h(V));

    g = gbar*pow(m,p)*pow(h,q);  
}




void compartment::init() {
    delete[] V_data;
    V_data = new double[n_steps];
    for (int i = 0; i<n_steps; i++) {
        V_data[i] = 0;
    }

    // ask all channels to init too
    for (int i = 0; i < n_channels; i++) {
        allchannels[i]->dt = dt;
        allchannels[i]->init();
    }
}

void compartment::set_n_steps(int n) {
    n_steps = n;
}

void compartment::step(void) {


    V_prev = V;


    // ask all allchannels to step

    sigma_g = 0.0;
    sigma_gE = 0.0;


    for (int i = 0; i < n_channels; i++) {
        allchannels[i]->step();
        sigma_g += allchannels[i]->g;
        sigma_gE += (allchannels[i]->g)*(allchannels[i]->E);
    }


    // now update the voltage

    if (sigma_g == 0) {
        V_inf = V_prev;
    }
    else {
        V_inf = (sigma_gE + (I_ext/A))/sigma_g;
    }

    // integrate V
    V = V_inf + (V_prev - V_inf)*exp(-dt/(Cm/(sigma_g)));

}

void compartment::integrate() {

    this->init();

    for (int i = 0; i < n_steps; i++){
        this->step();


        // save data
        V_data[i] = V;
    }
}


conductance* compartment::channels(int i) {
    return this->allchannels.at(i);
}


double conductance::tau_m(double V_) {return 1;};
double conductance::tau_h(double V_) {return 1;};
double conductance::m_inf(double V_) {return 1;};
double conductance::h_inf(double V_) {return 1;};

void conductance::init() {
    if (isnan(m)) {
        m = m_inf(container->V);
    }

    if (isnan(h)) {
        h = h_inf(container->V);
    }
}