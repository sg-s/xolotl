
#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"
#include <random>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    double gaussrand(void);

    double * input_N = mxGetPr(prhs[0]);
    int N = (int) input_N[0];


    plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);

    double *rand_values;
    rand_values = mxGetPr(plhs[0]);

    for (int i = 0; i < N; i ++) {
        rand_values[i] = gaussrand();
    }




}




// originally from Knuth and Marsaglia
// see "A Convenient Method for Generating Normal Variables"
// SIAM Rev., 6(3), 260–264.
double gaussrand() {
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
            } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}