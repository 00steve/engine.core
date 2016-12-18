#include "glstructs.h"





rgba::rgba() : r(0), g(0), b(0), a(0) {}
rgba::rgba(float r,float g,float b) : r(r), g(g), b(b), a(1) {}
rgba::rgba(float r,float g,float b,float a) : r(r), g(g), b(b), a(a) {}

point::point() {}

point::point (float x,float y,float z) :
    position(vertex(x,y,z)),
    color(rgba(1,1,1)) {}

point::point(float x,float y,float z,float r,float g,float b) :
    position(vertex(x,y,z)),
    color(rgba(r,g,b)) {}

face::face(){}
face::face(int v1,int v2,int v3){
    vertex[0] = v1;
    vertex[1] = v2;
    vertex[2] = v3;
}


tri::tri() {}
tri::tri(vertex a,vertex b,vertex c) :
    a(a), b(b), c(c) {
}

void vbo::init_data_structures(){
        //build buffers array
        //glGenBuffers(1,buffers);

        //build arrays to hold data
        positionData = new GLdouble[triangle_size*9]();
        //glGenBuffers(1,&positionBuffer);
        colorData = new GLubyte[triangle_size*3]();
        //glGenBuffers(1,&colorBuffer);
        //send the initial data structures to the graphics card in
        //case nothing else is ever sent
        send();
        l_face_index = -1;
    }

vbo::vbo() :
    triangle_size(0)//,
    //positionSize(0),
    //colorSize(0)
    {
    init_data_structures();
}

vbo::vbo(unsigned int startingTriangleSize) :
    triangle_size(startingTriangleSize)//,
    //positionSize(startingTriangleSize * 9 * sizeof(GLdouble)),
    //colorSize(startingTriangleSize * 3 * sizeof(GLubyte))
    {
    init_data_structures();
    for(unsigned int i=0;i<startingTriangleSize *3;i++){
        colorData[i] = (double)(rand()%1000) *.001f;
    }
}



/*Sends the current buffer data to the graphics card*/
bool vbo::send(){
    /*make sure that if any vertices have been edited, they
    have been saved to the array before sending the array to
    graphics*/
    //store_current_face();
    return true;
}

void vbo::draw(){

    /*glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER,positionSize,positionData,GL_STREAM_DRAW);
    glVertexPointer(3, GL_DOUBLE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER,colorSize,colorData,GL_STREAM_DRAW);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLES,0,triangle_size*3);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);*/

}
