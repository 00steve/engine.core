#include "shader.h"


Shader::Shader(string name) //:
    //vertShader(glCreateShader(GL_VERTEX_SHADER)),
    //fragShader(glCreateShader(GL_FRAGMENT_SHADER))
        {
    string baseShaderName = AssetLibrary::RootDirectory() + "shaders/" + name;
    string vertexShaderName = baseShaderName + "_vertex.txt";
    string fragmentShaderName = baseShaderName + "_fragment.txt";
    cout << " - Load vert shader : " + vertexShaderName << endl;
    cout << " - Load frag shader : " + fragmentShaderName << endl;

    vertexCode = AssetLibrary::LoadString(vertexShaderName);
    fragmentCode = AssetLibrary::LoadString(fragmentShaderName);
    cout << vertexCode << "\n\n";
    cout << fragmentCode << "\n\n";



}
