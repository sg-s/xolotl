





void conductance::readV() {
    double temp = container->V;
}


// uses the Euler-Maruyama method
// and is consistent with Goldwyn, Shea-Brown and with
// Sengupta, Laughlin and Niven
void conductance::integrateLangevin(double V, double Ca) {


    V_idx = (int) round((V*10)+999);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 2000) {V_idx = 2000;};

    // calculate the number of channels 
    N = round((gbar*(container->A))/unitary_conductance);

    // mexPrintf("N = %i\n",N);

    // assume that p > 0
    switch (approx_m) {
        case 0:
            minf = m_inf(V,Ca);
            taum = tau_m(V,Ca);
            m += (dt/taum)*(minf - m) + sqrt((dt/(taum*N))*(m + minf - 2*m*minf))*distribution(generator);
            break;

        default:
            m += (dt/tau_m_cache[V_idx])*(m_inf_cache[V_idx] - m) + sqrt((dt/(tau_m_cache[V_idx]*N))*(m + m_inf_cache[V_idx] - 2*m*m_inf_cache[V_idx]))*distribution(generator);
            break;
    } // switch approx_m

    // stay within bounds!
    if (m<0) {m = 0;}
    if (m>1) {m=1;}
    
    g = gbar*fast_pow(m,p);

    switch (q) {
        case 0:
            break;
        default:
            switch (approx_h) {
                case 0:
                    hinf = h_inf(V,Ca);
                    tauh = tau_h(V,Ca);
                    h += (dt/tauh)*(hinf - h) + sqrt((dt/(tauh*N))*(h + hinf - 2*h*hinf))*distribution(generator);
                    break;
                default:
                    h += (dt/tau_h_cache[V_idx])*(h_inf_cache[V_idx] - m) + sqrt((dt/(tau_h_cache[V_idx]*N))*(h + h_inf_cache[V_idx] - 2*h*h_inf_cache[V_idx]))*distribution(generator);
                    break;
            }

            // stay within bounds!
            if (h<0) {h = 0;}
            if (h>1) {h = 1;}

            g = g*fast_pow(h,q);  

    
            break;

    } // switch q

    gbar = gbar_next;


}

