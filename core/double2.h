
#ifndef DOUBLE2_H
#define DOUBLE2_H
#include <math.h>

class double2 {
    public:
        double x,y;

        double2();
        double2(double newX,double newY);

        double2 normalize();

        double length();

        /*assignment operators*/
        double2 operator += (const double2 &f);
        double2 operator -= (const double2 &f);

		double2 operator *= (const double &f);

        /*simple arithmatic operators*/
        double2 operator + (const double2 &i);
        double2 operator - (const double2 &i);

        double2 operator * (const double &f);

        /*comparison operators*/
        bool operator < (const double &i);
        bool operator > (const double &i);
};
#endif

