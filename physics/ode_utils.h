#ifndef ODE_UTILS_H
#define ODE_UTILS_H
#define dSINGLE
#include <ode/ode.h>
#include "../core/double3.h"

dReal* dCross(dReal* a,dReal* b);

double3 to_double3(const dReal *d);

#endif // ODE_UTILS_H


