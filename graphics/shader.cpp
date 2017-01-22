#include "shader.h"

Shader::Shader(string name) :
        valid(true){
    string baseShaderName = AssetLibrary::RootDirectory() + "shaders/" + name;
    string vertexShaderName = baseShaderName + "_vertex.txt";
    string fragmentShaderName = baseShaderName + "_fragment.txt";
    //cout << " - Load vertex shader : " + vertexShaderName << endl;
    //cout << " - Load fragment shader : " + fragmentShaderName << endl;

    string vertexSourceString = AssetLibrary::LoadString(vertexShaderName);
    string fragmentSourceString = AssetLibrary::LoadString(fragmentShaderName);

    const char* vertexSource = vertexSourceString.c_str();
    const char* fragmentSource = fragmentSourceString.c_str();
    int vertexSourceLength = vertexSourceString.length();
    int fragmentSourceLength = fragmentSourceString.length();
    //cout << vertexSource << "\n\n";
    //cout << "length : " << vertexSourceLength << endl;
    //cout << fragmentSource << "\n\n";
    //cout << "length : " << fragmentSourceLength << endl;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSource, &vertexSourceLength);
    glShaderSource(fragmentShader, 1, &fragmentSource, &fragmentSourceLength);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);


    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
    if (!vertexShaderCompiled){
       cout << " - failed to compile vertex shader!\n\t- " << vertexShaderName << endl;
        valid = false;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
    if (!fragmentShaderCompiled){
       cout << " - failed to compile fragment shader!\n\t- " << fragmentShaderName << endl;
       valid = false;
    }
    if(!valid){
        return;
    }

    program = glCreateProgram();

    //Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    //Link our program
    glLinkProgram(program);


    isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        //The maxLength includes the NULL character
        //GLchar* infoLog;
        //glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        //We don't need the program anymore.
        glDeleteProgram(program);
        //Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        //Use the infoLog as you see fit.
        //cout << infoLog << endl;
        cout << " - failed to link gl program\n";
        //In this simple program, we'll just leave
        valid = false;
        return;
    }




}


Shader::~Shader(){
    if(vertexShaderCompiled) glDeleteShader(vertexShader);
    if(fragmentShaderCompiled) glDeleteShader(fragmentShader);
}


bool Shader::Valid(){ return valid; }
GLuint Shader::Program(){ return program; }
