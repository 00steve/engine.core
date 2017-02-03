#include "quaternion.h"


void quaternion::normalize() {
    if((magnitude = x*x+y*y+z*z+w*w) > 1.01) {
        magnitude = sqrt(magnitude);
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
        w /= magnitude;
    }
}

quaternion::quaternion() :
    x(0),
    y(0),
    z(0),
    w(1),
    magnitude(1) {
}

quaternion::quaternion(double3 offset) :
    x(offset.x),
    y(offset.y),
    z(offset.z),
    w(1) {
    normalize();
}

quaternion::quaternion(double x,double y,double z,double w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    normalize();
}

quaternion::quaternion(double3 axis,double angle){
    w  = cosf( angle/2);
    x = axis.x * sinf( angle/2 );
    y = axis.y * sinf( angle/2 );
    z = axis.z * sinf( angle/2 );
    normalize();
}

quaternion quaternion::operator *(const quaternion &b){
    quaternion result;
    result.w = (b.w * w) - (b.x * x) - (b.y * y) - (b.z * z);
    result.x = (b.w * x) + (b.x * w) + (b.y * z) - (b.z * y);
    result.y = (b.w * y) + (b.y * w) + (b.z * x) - (b.x * z);
    result.z = (b.w * z) + (b.z * w) + (b.x * y) - (b.y * x);
    result.normalize();
    return(result);
}

quaternion quaternion::conjugate(){
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

quaternion quaternion::getConjugate(){
    return quaternion(-x,-y,-z,w);
}

double3 quaternion::operator *(double3 v){
    quaternion vf = quaternion(v);
    //apply multiplications
    vf = vf * getConjugate();
    vf = *this * vf;
    return double3(vf.x, vf.y, vf.z).normalize();
}

double3 quaternion::operator +(double3 v){
    double3 t = double3(x,y,z).normalize();
    return t+v;
}

float* quaternion::toMatrix(float* matrix){
    // First row
    matrix[0]	= 1.0f - 2.0f * ( y * y + z * z );
    matrix[1]	= 2.0f * (x * y + z * w);
    matrix[2]	= 2.0f * (x * z - y * w);
    matrix[3]	= 0.0f;
    // Second row
    matrix[4]	= 2.0f * ( x * y - z * w );
    matrix[5]	= 1.0f - 2.0f * ( x * x + z * z );
    matrix[6]	= 2.0f * (z * y + x * w );
    matrix[7]	= 0.0f;
    // Third row
    matrix[8]	= 2.0f * ( x * z + y * w );
    matrix[9]	= 2.0f * ( y * z - x * w );
    matrix[10]	= 1.0f - 2.0f * ( x * x + y * y );
    matrix[11]	= 0.0f;
    // Fourth row
    matrix[12]	= 0;
    matrix[13]	= 0;
    matrix[14]	= 0;
    matrix[15]	= 1.0f;
    return matrix;
}


float* quaternion::toRotationMatrix(float* matrix){
    matrix[0]=1;
    matrix[1]=0;//4
    matrix[2]=0;//8
    matrix[3]=0;

    matrix[4]=0;//1
    matrix[5]=1;
    matrix[6]=0;//9
    matrix[7]=0;

    matrix[8]=0;//2
    matrix[9]=0;//6
    matrix[10]=1;
    matrix[11]=0;
    double3 forw = getForwardVector();
    matrix[12]= -forw.x * 57.29578f;
    matrix[13]= -forw.y * 57.29578f;
    matrix[14]= -forw.z * 57.29578f;
    matrix[15]=1;

    return matrix;
}

quaternion quaternion::getDelta(quaternion q){
    return getConjugate() * q;
}

double3 quaternion::getForwardVector(){
    return double3( 2 * (x * z + w * y), 2 * (y * x - w * x), 1 - 2 * (x * x + y * y));
}

double3 quaternion::getUpVector(){
    return double3( 2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x));
}

double3 quaternion::getRightVector(){
    return double3( 1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y));
}

quaternion quaternion::interpolate(quaternion q,double t){
    return quaternion(x*(1-t)+q.x*t, y*(1-t)+q.y*t, z*(1-t)+q.z*t, w*(1-t)+q.w*t);
}

/*
void quaternion::print(){
    cout << "(" << x << "," << y << "," << z << "," << w << ")\n";
}*/

quaternion quaternion::backward(){
    return quaternion(0,1,0,0);
}
quaternion quaternion::down(){
    return quaternion(.7068,0,0,.7068);
}
quaternion quaternion::up(){
    return quaternion(-.7068,0,0,.7068);
}
quaternion quaternion::right(){
    return quaternion(0,-.7068,0,.7068);
}
quaternion quaternion::left(){
    return quaternion(0,.7068,0,.7068);
}

quaternion quaternion::fromMatrix(float *r){
    return quaternion();
}

quaternion quaternion::rotateX(double x){
    return quaternion(sin(x/2),0,0,cos(x/2));
}
quaternion quaternion::rotateY(double y){
    return quaternion(0,sin(y/2),0,cos(y/2));
}
quaternion quaternion::rotateZ(double z){
    return quaternion(0,0,sin(z/2),cos(z/2));
}


