#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H


#define GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <string>
#include <engine/assetlibrary/assetlibrary.h>

//PFNGLSHADERSOURCEPROC glCreateShader =
 // (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glCreateShader");



using namespace std;

class Shader{
private:
    GLuint vertexShader;
    GLuint fragmentShader;

    GLuint vertexLength;
    GLuint fragmentLength;

    GLint vertexShaderCompiled;
    GLint fragmentShaderCompiled;

    GLuint program;
    GLint isLinked;

    bool valid;

public:
    /**\brief Create and load a glsl shader program, which includes the fragment and vertex
    shaders, linked into a program

    The constructor will use the default AssetLibrary root location, with the subfolder "shaders"
    and automatically adding the _fragment.txt or _vertex.txt to load both scripts. For example,
    if you wanted to load a shader called "fog", you would make sure you have two files placed
    in a directory called "assets/shaders/" relative to the engine executable. Those files should
    be named "fog_fragment.txt" and "fog_vertex.txt".*/
    Shader(string name);


    ~Shader();

    bool Valid();
    GLuint Program();
};


#endif // GLSL_SHADER_H
