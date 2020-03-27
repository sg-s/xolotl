

/*
This method "connects" a conductance object to a compartment
object. This sets the `container` property of the conductance,
so the channel knows which compartment contains it. 
*/
void conductance::connect(compartment *pcomp_) {
    if (isnan(gbar)) {
        // gbar unset, set it to a default
        gbar = 0;
    }
    if (gbar < 0) {
        mexErrMsgTxt("gbars cannot be negative for any conductance \n");
    }
    container = pcomp_;

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
is to be calculated (i.e.,`approx_m = 0` and `approx_h=0`)
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
    switch (approx_m) {
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
    } // switch approx_m
    
    g = gbar*fast_pow(m,p);

    switch (q) {
        case 0:
            break;
        default:
            switch (approx_h) {
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
    switch (approx_m) {
        case 0:
            minf = m_inf(V,Ca);
            taum = tau_m(V,Ca);
            m += (dt/taum)*(minf - m) + sqrt((dt/(taum*N))*(m + minf - 2*m*minf))*gaussrand();
            break;

        default:
            m += (dt/tau_m_cache[V_idx])*(m_inf_cache[V_idx] - m) + sqrt((dt/(tau_m_cache[V_idx]*N))*(m + m_inf_cache[V_idx] - 2*m*m_inf_cache[V_idx]))*gaussrand();
            break;
    } // switch approx_m

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
            switch (approx_h) {
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

