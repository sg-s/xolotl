

// this C++ file contains all the method
// definitions for classes defined in header files



#include "xolotl.hpp"
#include "compartment.hpp"
#include "conductance.hpp"

compartment::compartment(string Name) {
  this->Name = Name;
}


void compartment::integrate() {
  // do nothing
}

void compartment::add(conductance* cond_) {
	channels.push_back(cond_);
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
    if (i < this->n_compartments) {
        return this->allcompartments[i];
    }
    return nullptr;
}


void conductance::integrate() {   


    V = container->V_prev;
    Ca = container->Ca_prev;


    minf = m_inf(V,Ca);
    hinf = h_inf(V,Ca);

    m = minf + (m - minf)*exp(-dt/tau_m(V,Ca));
    h = hinf + (h - hinf)*exp(-dt/tau_h(V,Ca));

    g = gbar*pow(m,p)*pow(h,q);  
}




void compartment::init() {
    arr = new int[n_steps];
    for (int i = 0; i<n_steps; i++) {
        arr[i] = 0;
    }
}

void compartment::set_n_steps(int n) {
    n_steps = n;
}

// const int * const compartment::getMat(size_t len2) const
// {
//     len2 = (size_t) n_steps;
//     return this->arr;
// }