#include "ode_utils.h"



dReal* dCross(dReal* a,dReal* b){
    dReal *c = new dReal[3];
    c[0] = a[1]*b[2] - b[1]*a[2];
    c[1] = b[0]*a[2] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
    return c;
}

double3 to_double3(const dReal *d){
	return double3(d[0],d[1],d[2]);
}

