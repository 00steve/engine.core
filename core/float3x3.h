#ifndef FLOAT3x3_H
#define FLOAT3x3_H

#include <math.h>

/*a generic matrix class with ambitions to output arrays of
3x4 doubles that are compatible with ODE physics engine*/
class float3x3{
private:

	float m[9];

public:


	float* toDMatrix3();


	static float3x3 identity();

	static float3x3 xRotation(float r);

	static float3x3 yRotation(float r);

	static float3x3 zRotation(float r);


};





#endif // DOUBLE3x3_H
