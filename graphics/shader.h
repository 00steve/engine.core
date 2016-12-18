#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H


#include <string>
#include <engine/assetlibrary/assetlibrary.h>

using namespace std;

class Shader{
private:
    //GLuint vertShader;
   // GLuint fragShader;

    string fragmentCode;
    string vertexCode;

public:
    Shader(string name);

};


#endif // GLSL_SHADER_H
