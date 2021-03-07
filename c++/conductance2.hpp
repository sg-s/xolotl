

/*
This method "connects" a conductance object to a compartment
object. This sets the `container` property of the conductance,
so the channel knows which compartment contains it. 
*/
void conductance::connect(compartment *pcomp_) {
    container = pcomp_;
}



/*
This virtual method is a placeholder initialization method. Initialization 
methods are called after the model has been constructed, and all components
have been connected, but before the model starts the simulation. This is a
good place to put code that you need to run once before the simulation. 
*/
void conductance::init() {

    if (isnan(gbar)) {
        // gbar unset, set it to a default
        gbar = 0;
    }
    if (gbar < 0) {
        mexErrMsgTxt("gbars cannot be negative for any conductance \n");
    }

    // make sure m and h are not NaN
    if (isnan(m)) {
        m = m_inf(container->V, container->Ca);
    }

    if (isnan(h)) {
        h = h_inf(container->V, container->Ca);
    }

    if (tau_m(container->V, container->Ca) == 0) {instantaneous_m = 1;}
    if (tau_h(container->V, container->Ca) == 0) {instantaneous_h = 1;}

}



/*
This method integrates the conductance object using
the exponential Euler method. This is the default
integration method used by xolotl. If an exact solution
is to be calculated (i.e.,`AllowMInfApproximation = 0` and `AllowHInfApproximation=0`)
then `m` and `h` are updated using the exponential Euler
equation using function evaluations of the activation 
functions at this voltage and Calcium.

Otherwise, the lookup table is used to update `m` and `h`
in this channel. 

Note that this method is defined as virtual, so it can be
overridden by integration methods specified in a specific
conductance. 

**See Also** 

* [virtual methods in C++](http://www.cplusplus.com/doc/tutorial/polymorphism/)
*/
void conductance::integrate(double V, double Ca) {   


    if (is_calcium) {
        E = container->E_Ca;
    }

    V_idx = (int) round((V*10)+999);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 2000) {V_idx = 2000;};

    // assume that p > 0
    switch (UseMInfApproximation) {
        case 0:
            minf = m_inf(V,Ca);

            switch (instantaneous_m) {
                case 0:
                    m = minf + (m - minf)*exp(-dt/tau_m(V,Ca));
                    break;
                default:
                    m = minf;
                    break;
            } // end switch instantaneous_m

            
            break;

        default:
            switch (instantaneous_m) {
                case 0:
                    m = m_inf_cache[V_idx] + (m - m_inf_cache[V_idx])*fast_exp(-(dt/tau_m_cache[V_idx]));
                    break;
                default:
                    m = m_inf_cache[V_idx];
                    break;
            } // end switch instantaneous_m
            
            break;
    } // switch AllowMInfApproximation
    
    g = gbar*fast_pow(m,p);

    switch (q) {
        case 0:
            break;
        default:
            switch (UseHInfApproximation) {
                case 0:
                    switch (instantaneous_h) {
                        case 0:
                            hinf = h_inf(V,Ca);
                            h = hinf + (h - hinf)*exp(-dt/tau_h(V,Ca));
                            break;
                        default:
                            h = h_inf(V,Ca);
                            break;
                    } // end switch instantaneous_h
                    break;
                default:
                    switch (instantaneous_h) {
                        case 0: 
                            h = h_inf_cache[V_idx] + (h - h_inf_cache[V_idx])*fast_exp(-(dt/tau_h_cache[V_idx]));
                            break;
                        default:
                            h = h_inf_cache[V_idx];
                            break;
                    } // end switch instantaneous_h
                    
                    break;
            }

            g = g*fast_pow(h,q);            
            break;
    } // switch q


    if (is_calcium) {
        container->i_Ca += getCurrent(V);
    }

}


/*
This method integrates the conductance object using
the Euler-Maruyama method. The integration method
used here is consistent with the methods used in
[Goldwyn and Shea-Brown 2011](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247)
and with [Sengupta, Laughlin and Niven](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.81.011918)

Briefly, this method follows the approximate Langevin
formulation of the underlying stochastic system
formed by N independent channels that have 
independent gating kinetics. It can be thought of
as the deterministic ODE, with an additive noise term
whose variance scales with the inverse square root of
the number of channels. The number of channels is 
computed automatically from the channel density
and the area of the compartment. 

*/
void conductance::integrateLangevin(double V, double Ca) {


    if (is_calcium) {
        E = container->E_Ca;
    }


    V_idx = (int) round((V*10)+999);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 2000) {V_idx = 2000;};

    // calculate the number of channels 
    N = round((gbar*(container->A))/unitary_conductance);

    // if there are no channels, abort
    if (N < 1) { return;}

    // mexPrintf("N = %i\n",N);

    // assume that p > 0
    switch (UseMInfApproximation) {
        case 0:
            minf = m_inf(V,Ca);
            taum = tau_m(V,Ca);
            m += (dt/taum)*(minf - m) + sqrt((dt/(taum*N))*(m + minf - 2*m*minf))*gaussrand();
            break;

        default:
            m += (dt/tau_m_cache[V_idx])*(m_inf_cache[V_idx] - m) + sqrt((dt/(tau_m_cache[V_idx]*N))*(m + m_inf_cache[V_idx] - 2*m*m_inf_cache[V_idx]))*gaussrand();
            break;
    } // switch AllowMInfApproximation

    // stay within bounds!
    // mexPrintf("m = %f\n", m);
    if (isnan(m)) {
        mexPrintf("m is NaN, N = %i\n", N);
        mexPrintf("m is NaN, taum = %f\n", taum);
        mexPrintf("m is NaN, minf = %f\n", minf);
        mexPrintf("m is NaN, V = %f\n", V);
        mexErrMsgTxt("[error] stopping because NaNs appeared in integration!");
    }
    if (m<0) {m = -m;}
    if (m>1) {m = 2-m;}
    
    g = gbar*fast_pow(m,p);

    switch (q) {
        case 0:
            break;
        default:
            switch (UseHInfApproximation) {
                case 0:
                    hinf = h_inf(V,Ca);
                    tauh = tau_h(V,Ca);
                    h += (dt/tauh)*(hinf - h) + sqrt((dt/(tauh*N))*(h + hinf - 2*h*hinf))*gaussrand();
                    break;
                default:
                    h += (dt/tau_h_cache[V_idx])*(h_inf_cache[V_idx] - h) + sqrt((dt/(tau_h_cache[V_idx]*N))*(h + h_inf_cache[V_idx] - 2*h*h_inf_cache[V_idx]))*gaussrand();
                    break;
            }

            // stay within bounds!
            if (h<0) {h = -h;}
            if (h>1) {h = 2-h;}

            g = g*fast_pow(h,q);  

    
            break;

    } // switch q


    if (is_calcium) {
        container->i_Ca += getCurrent(V);
    }

}




/*

This method integrates a channel object using a multi-step
solver (MS = "multi-step"). The "sub-step" is indicated in 
the integer k, which is the first input to this method. 

The multi-step solver that is used here is a Runge-Kutta 4th
order solver. Thus, k can have values up to 4. 

Based on `k`, different elements of the arrays `k_m` and `k_h`
are calculated and stored. At each step, the derivative functions
`mdot` and `hdot` are computed. 

**See Also**

* [The Runge Kutta Method](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods)

*/
void conductance::integrateMS(int k, double V, double Ca) {


    if (is_calcium) {
        E = container->E_Ca;
    }


    if (q == 0) {
        // q= 0 means channel does not inactivate
        switch (k)
        {
            case 0:
                k_m[0] = dt*(mdot(V, Ca, m));
                g = gbar*fast_pow(m,p);
                break;
            case 1:
                k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
                g = gbar*fast_pow(m + k_m[0]/2,p);
                break;
            case 2:
                k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
                g = gbar*fast_pow(m + k_m[1]/2,p);
                break;
            case 3:
                k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
                g = gbar*fast_pow(m + k_m[2],p);
                break;
            case 4:
                // last step
                m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
                break;
        }
            
    } else {

        switch (k)
        {
            case 0:
                k_m[0] = dt*(mdot(V, Ca, m));
                k_h[0] = dt*(hdot(V, Ca, h));
                g = gbar*fast_pow(m,p)*fast_pow(h,q);
                break;
            case 1:
                k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
                k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
                g = gbar*fast_pow(m + k_m[0]/2,p)*fast_pow(h + k_h[0]/2,q);
                break;
            case 2:
                k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
                k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
                g = gbar*fast_pow(m + k_m[1]/2,p)*fast_pow(h + k_h[1]/2,q);
                break;
            case 3:
                k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
                k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
                g = gbar*fast_pow(m + k_m[2],p)*fast_pow(h + k_h[2],q);
                break;
            case 4:
                m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
                h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
                break;
        }

    }

    if (is_calcium) {
        container->i_Ca += getCurrent(V);
    }

}



/*
This method returns the previous state of the value stored in this 
mechanism by pulling it out of the mech_states array stored in the compartment that this mechanism belongs to 
*/
double mechanism::getPrevState(int i) {
    return comp->mech_states.at(mech_state_offset + i);
}




/*
THis method reads out the full state from the mechanism 
and writes it to a provided array, returning the index of 
the next position to write to.

This method is used in two scenarios:

1. In reading out mechanism state to output to the user
2. In reading out mechanism state and storing it in the compartment's mech_state property to allow for synchronous updates. 
*/
int mechanism::getFullState(double* A, int idx) {
    for (int i = 0; i < fullStateSize; i++) {
        A[idx] = getState(i);
        idx++;
    } 
    return idx;   
}




/*
This helper function find ONE mechanism with a certain name, 
that also control a certain thing, in
the current compartment and returns a pointer to it.
*/

mechanism* mechanism::findMechanismNamedControlling(std::string get_this_name, std::string should_control) {


    mechanism* this_mech = nullptr;
     
    int n_mech = comp->n_mech;
    mechanism * temp_mech = nullptr;

    string original_mech_name = this->name.c_str();

    // connect to any mechanism identifying itself as an "mRNA_RHS"
    // mechanism 
    for (int i = 0; i < n_mech; i++) {

        temp_mech = comp->getMechanismPointer(i);

        if (!temp_mech) {
            continue;
        }

        string this_mech_name = temp_mech->name.c_str();
        string this_mech_controls = comp->getMechanismPointer(i)->controlling_class.c_str();

        if (this_mech_name == get_this_name && this_mech_controls == should_control && temp_mech != this) {
            if (verbosity==0) {
                mexPrintf("%s",original_mech_name.c_str());
                mexPrintf("(controlling %s) connected to ",controlling_class.c_str());
                mexPrintf("[%s]\n",this_mech_name.c_str());
            }

            this_mech = temp_mech;
            return this_mech;
        }
    }


    return this_mech;


}




/*
This helper function find all mechanisms of a requested type, 
that also control a requested class of mechanism, in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 
*/

std::vector<mechanism*> mechanism::findMechanismsOfTypeControlling(std::string get_this_type, std::string should_control) {


    std::vector<mechanism*> these_mechs;
     
    int n_mech = comp->n_mech;
    mechanism * temp_mech = nullptr;

    string original_mech_name = this->name.c_str();

    // connect to any mechanism identifying itself as an "mRNA_RHS"
    // mechanism 
    for (int i = 0; i < n_mech; i++) {

        temp_mech = comp->getMechanismPointer(i);

        if (!temp_mech) {
            continue;
        }


        string this_mech_type = temp_mech->mechanism_type.c_str();
        string this_mech_name = temp_mech->name.c_str();
        string this_mech_controls = comp->getMechanismPointer(i)->controlling_class.c_str();

        if (this_mech_type == get_this_type && this_mech_controls == should_control && temp_mech != this) {
            if (verbosity==0) {
                mexPrintf("%s",original_mech_name.c_str());
                mexPrintf("(controlling %s) connected to ",controlling_class.c_str());
                mexPrintf("[%s]\n",this_mech_name.c_str());
            }

            these_mechs.push_back(temp_mech);
        }
    }


    return these_mechs;


}



/*
This helper function find all mechanisms of a requested type in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 
*/

std::vector<mechanism*> mechanism::findMechanismsOfType(std::string get_this_type) {


    std::vector<mechanism*> these_mechs;
     
    int n_mech = comp->n_mech;
    mechanism * temp_mech = nullptr;

    string original_mech_name = this->name.c_str();

    // connect to any mechanism identifying itself as an "mRNA_RHS"
    // mechanism 
    for (int i = 0; i < n_mech; i++) {

        temp_mech = comp->getMechanismPointer(i);

        if (!temp_mech) {
            continue;
        }

        string this_mech_type = temp_mech->mechanism_type.c_str();
        string this_mech_name = temp_mech->name.c_str();

        if (this_mech_type == get_this_type && this != temp_mech) {
            if (verbosity==0) {
                mexPrintf("%s",original_mech_name.c_str());
                mexPrintf("(controlling %s) connected to ",controlling_class.c_str());
                mexPrintf("[%s]\n",this_mech_name.c_str());
            }

            these_mechs.push_back(temp_mech);
            
        }
    }
    return these_mechs;

}




/*
This helper function find all mechanisms that control a specified
class of mechanisms  in
the current compartment and returns a vector of pointers to them.
You are free to do whatever you like with that vector. A typical
use case would be to find mechanisms of a required type, then connect to them from another mechanism. 
*/

std::vector<mechanism*> mechanism::findMechanismsControlling(std::string should_control) {


    std::vector<mechanism*> these_mechs;
     
    int n_mech = comp->n_mech;
    mechanism * temp_mech = nullptr;

    string original_mech_name = this->name.c_str();

    // connect to any mechanism identifying itself as an "mRNA_RHS"
    // mechanism 
    for (int i = 0; i < n_mech; i++) {
        temp_mech = comp->getMechanismPointer(i);

        if (!temp_mech) {
            continue;
        }

        string this_mech_name = temp_mech->name.c_str();
        string this_mech_controls = temp_mech->controlling_class.c_str();

        if (this_mech_controls == should_control && temp_mech != this) {
            if (verbosity==0) {
                mexPrintf("%s",original_mech_name.c_str());
                mexPrintf("(controlling %s) connected to ",controlling_class.c_str());
                mexPrintf("[%s]\n",this_mech_name.c_str());
            }

            these_mechs.push_back(temp_mech);
            
        }
    }


    return these_mechs;


}




/*
This virtual method is a placeholder method of mechanism connects 
a mechanism to a compartment. It sets the "comp" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
compartment, override this method with something that 
throws a runtime error. 
*/
void mechanism::connectCompartment(compartment* comp_) {

    comp = comp_;

    // make sure the compartment that we are in knows about us
    comp->addMechanism(this);
    controlling_class = "compartment";
}

/*
This virtual method is a placeholder method of mechanism connects 
a mechanism to a conductance. It sets the "channel" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
conductance, override this method with something that 
throws a runtime error. 
*/
void mechanism::connectConductance(conductance* channel_) {

    // connect to a channel
    channel = channel_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);
    controlling_class = (channel_->name).c_str();

}

/*
This virtual method is a placeholder method of mechanism connects 
a mechanism to a conductance. It sets the "channel" pointer 
in the mechanism, and tells the parent compartment about this 
mechanism (via addMechanism). 

If you want to prevent your mechanism from connecting to a 
conductance, override this method with something that 
throws a runtime error. 
*/
void mechanism::connectSynapse(synapse* syn_) {

    // connect to a channel
    syn = syn_;

    // make sure the compartment that we are in knows about us
    (syn->post_syn)->addMechanism(this);
    controlling_class = (syn->name).c_str();

}
