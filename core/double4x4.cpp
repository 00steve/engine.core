
#include "double4x4.h"


double4x4::double4x4(double a1,double a2,double a3,double a4,
                 double b1,double b2,double b3,double b4,
                 double c1,double c2,double c3,double c4,
                 double d1,double d2,double d3,double d4) {
    f = new double[16];
    f[0] = a1;f[1] = a2;f[2] = a3;f[3] = a4;
    f[0] = b1;f[1] = b2;f[2] = b3;f[3] = b4;
    f[0] = c1;f[1] = c2;f[2] = c3;f[3] = c4;
    f[0] = d1;f[1] = d2;f[2] = d3;f[3] = d4;
}


double4x4::double4x4(double vals[]) {
    f = vals;
}

double double4x4::operator [] (int index) {
    return f[index];
}
