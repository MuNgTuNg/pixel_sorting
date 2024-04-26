#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <files.hpp>
namespace shb{

//TODO::shader manager class

class Shader{
    public:
    Shader(){}
    Shader(std::string fileName){
        
        int pos = fileName.find(".");
        std::string extension = fileName.substr(pos);
        extension == ".vert" ? m_Type = GL_VERTEX_SHADER : m_Type = GL_FRAGMENT_SHADER;

        m_Handle = glCreateShader(m_Type);
        m_Name = fileName;

        std::string code = readFile(std::string(m_ShaderPath + fileName).c_str());
        const char* source;
        source = code.c_str();
        glShaderSource( m_Handle,1,&source,NULL);
        glCompileShader(m_Handle);
        
        char infoLog[512];
        glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &m_CompileSuccess);
        if(!m_CompileSuccess)
        {
            glGetShaderInfoLog(m_Handle, 512, NULL, infoLog);
            std::cout << fileName <<  " Compilation failed.\n" << infoLog << std::endl;
        };
    }

    ~Shader(){
        glDeleteShader(m_Handle);
    }

    int handle() { return m_Handle; }

    private:
    std::string m_Name;
    GLenum m_Type;
    std::string m_ShaderPath = "../src/shaders/";
    int m_CompileSuccess = 0;
    int m_Handle = 0;

};

class ShaderProgram{
    public:
    //variadic template TODO::
    ShaderProgram(std::vector<std::string> fileNames){
        load(fileNames);
    }

    void load(std::vector<std::string> fileNames){
        m_FileNames = fileNames;
        m_Handle = glCreateProgram();
        for(int i = 0; i < m_FileNames.size(); ++i){
            Shader shader{m_FileNames[i]};
            attach(shader);
        }
        link();
    }

    void reload(){
        load(m_FileNames);
    }

    void attach(Shader shader) {       
        glAttachShader(m_Handle,shader.handle());
    }
    int link(){
        int success;
        char infoLog[512];
        glLinkProgram(m_Handle);
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(m_Handle, 512, NULL, infoLog);
            std::cout << "Shader Program linking failed.\n" << infoLog << std::endl;
        }
    }
    
    int uniformLoc(std::string uniformName){
        return glGetUniformLocation(m_Handle, uniformName.c_str());
    }

    void use(){
        glUseProgram(m_Handle);
    }

    int handle(){ return m_Handle; }

    private:
    int m_Handle = 0;
    std::vector<std::string> m_FileNames{"Empty"};
};

}//namespace shb