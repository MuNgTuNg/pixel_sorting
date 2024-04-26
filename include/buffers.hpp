#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace shb{

class Buffer{
  public:
    template<typename T>
    Buffer(const std::vector<T>& vertices, GLenum type){ 
        m_Type = type;
        glGenBuffers(1,&m_Handle);
        glBindBuffer(type,m_Handle);
        glBufferData(type,vertices.size() * sizeof(T),&vertices[0],GL_STATIC_DRAW);
        glBindBuffer(type,0);

        m_NumElements = vertices.size();
    }

    void bind(){
        glBindBuffer(m_Type,m_Handle);
    }

    void unbind(){
        glBindBuffer(m_Type,0);
    }
    ~Buffer(){
        glDeleteBuffers(1,&m_Handle);
    }

    int const numElements(){
        return m_NumElements;
    }

  private:
    GLenum m_Type = 0;
    unsigned int m_Handle = 0;
    int m_NumElements = 0;
};

class VertexArray{
  public:
    VertexArray(){
        glGenVertexArrays(1, &m_Handle);  
        glBindVertexArray(0);
    }
    ~VertexArray(){
        glDeleteVertexArrays(1,&m_Handle);
    }


    void bind(){
        glBindVertexArray(m_Handle);
    }
    void unbind(){
        glBindVertexArray(0);
    }
  private:
    unsigned int m_Handle = 0;
};

}//namespace shb