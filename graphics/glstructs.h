#ifndef GRAPHICS_STRUCTS_H
#define GRAPHICS_STRUCTS_H
#include <cstdlib>
#include <fstream>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <engine/core/double3.h>
typedef double3 vertex;

struct rgba{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
    rgba();
    rgba(float r,float g,float b);
    rgba(float r,float g,float b,float a);
};

struct point{
    vertex position;
    rgba color;
    point();
    point (float x,float y,float z);
    point(float x,float y,float z,float r,float g,float b);
};

struct locking_point : public point{
    bool locked;
};

struct face {
    int vertex[3];
    int texture[3];
    int normal[3];

    face();
    face(int v1,int v2,int v3);

};

/*stores the indexes of vertices in a list. The
short type is used with the *assinine* assumption
that no mesh will ever have more than 65535 unique
vertices. Let's hope so.*/
struct tri {
    vertex a;
    vertex b;
    vertex c;

    tri();
    tri(vertex a,vertex b,vertex c);
};

struct vbo {
private:
    /*keep track of the total number of triangles that
    are being stored in the vbo*/
    unsigned int triangle_size;

    GLuint positionBuffer;
    //GLsizeiptr positionSize;// = 6 * 2 * sizeof(GLfloat);
    GLdouble *positionData;

    GLuint colorBuffer;
    //GLsizeiptr colorSize;// = 6 * 3 * sizeof(GLubyte);
    GLubyte *colorData;

    GLuint *buffers;

    tri l_face;
    unsigned int l_face_index;

    void init_data_structures();



public:
    /*create a blank vbo with all of the data set to
    zero. This is probably not good, so don't do it.*/
    vbo();
    /*construct the vbo giving it a starting
    size. If the size is known ahead of time, this
    should be used to prevent the resizing of the arrays
    that contain the data at runtime.*/
    vbo(unsigned int startingTriangleSize);


    /*Sends the current buffer data to the graphics card*/
    bool send();

    void draw();

};

#endif // GRAPHICS_STRUCTS_H
