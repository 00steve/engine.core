#ifndef QUATERNION_H
#define QUATERNION_H

/*my custom quaternion class. I wanted to know how they work and
wanted to use my own utility classes to build it.

The getForwardVector, getUpVector and getRightVector were pretty
much ripped from Nic Foster at http://nic-gamedev.blogspot.com/
2011/11/quaternion-math-getting-local-axis.html
*/

#include <math.h>
#include "double3.h"
#include "double4x4.h"

class quaternion {
private:

    double x,y,z,w;
    double magnitude;

    /*if the magnitude of the quaternion falls out of
    the acceptable range, renormalize it to make it a
    unit quaternion.*/
    void normalize();

public:
    quaternion();

    /*generate a quaternion from an offset*/
    quaternion(double3 offset);

    /*create a new quaternion with all of the values
    already set.*/
    quaternion(double x,double y,double z,double w);

    /*create a new quaternion from a unit vector
    rotation.*/
    quaternion(double3 axis,double angle);

    /*multiply this quaternion by another quaternion. This is
    NON-COMMUTATIVE, which means Q1 x Q2 != Q2 x Q1. To rotate
    PLAYER_ROT using another quaternion, the code would look
    like FINAL_ROATAION = TURN_AMOUNT * PLAYER_ROT*/
    quaternion operator *(const quaternion &b);

    /*conjugate the current quaternion.*/
    quaternion conjugate();

    /*return conjugate copy of current quaternion*/
    quaternion getConjugate();

    /*define vector multiplication operator*/
    double3 operator *(double3 v);

    /*allow a quaternion to have a double3 added and return a
    new double3 that combines the quaternion's offset and
    the double3*/
    double3 operator +(double3 v);

    /*converts the quaternion to a 4x4 rotation matrix. It can
    be used by openGL to rotate something*/
    float* toMatrix(float* matrix);


	float* toRotationMatrix(float* matrix);


    /*gets the quaternion representation of the difference between
    this quaternion and another quaternion.*/
    quaternion getDelta(quaternion q);

    double3 getForwardVector();

    double3 getUpVector();

    double3 getRightVector();

    quaternion interpolate(quaternion q,double t);

    //void print();

	/*some static functions that generate quaternions in
	default orientations*/
    static quaternion backward();
    static quaternion down();
    static quaternion up();
    static quaternion right();
    static quaternion left();

    static quaternion fromMatrix(float *r);

	/*some static functions to generate quaternions at
	arbitrary rotations. They are generally used multipled
	with existing quaternions to rotate them.*/
    static quaternion rotateX(double x);
    static quaternion rotateY(double y);
    static quaternion rotateZ(double z);
};
#endif

