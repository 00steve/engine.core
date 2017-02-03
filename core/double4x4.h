/*holds a rotation matrix*/
#ifndef DOUBLE4X4_H
#define DOUBLE4X4_H

class double4x4 {
    private:
        double *f;

    public:
        double4x4(double a1,double a2,double a3,double a4,
                 double b1,double b2,double b3,double b4,
                 double c1,double c2,double c3,double c4,
                 double d1,double d2,double d3,double d4);

        double4x4(double vals[]);

        double operator [] (int index);
    /*
        GLdouble *GLMatrix()
        {
            //return {(GLdouble)f[0],(GLdouble)f[1],(GLdouble)f[2],(GLdouble)f[3],;
        }*/
};


#endif

