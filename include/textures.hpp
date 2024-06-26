#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace shb{

//loads and holds the data in m_Data as an array of bytes.
class ImageData{
  public:
    ImageData(const std::string& fileName) : m_FileName(fileName) {
        
       loadFile(fileName);
    }
    ~ImageData(){
       clearData();
    }
    ImageData(){};
    
    int width() const { return m_ImageWidth; }
    int height() const { return m_ImageHeight; }
    int numChannels() const  { return m_NumChannels; }
    std::string fileName() const { return m_FileName; }

    void clearData() {
        //stbi_image_free(imageData); ??
        if(m_Data){
            delete m_Data;
            m_Data = nullptr;
        }
    }

    void loadFile(const std::string& fileName){
        m_FileName = fileName;
        clearData();
        m_Data = stbi_load(fileName.c_str(), &m_ImageWidth, &m_ImageHeight, &m_NumChannels, 0);
    }

    //TODO:: make access functions like indexing into an array of pixels
    unsigned char* m_Data = nullptr;

  private:
    std::string m_FileName = "";
    int m_ImageWidth = 0;
    int m_ImageHeight = 0;
    int m_NumChannels = 0;
};

void loadTexture(const ImageData& imageData, unsigned int* textureHandle){
    glGenTextures(1, textureHandle);
    glBindTexture(GL_TEXTURE_2D, *textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (imageData.m_Data) {
        if(imageData.numChannels() == 3){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width(), imageData.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.m_Data); 
        }
        if(imageData.numChannels() == 4){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.width(), imageData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.m_Data); 
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture " + imageData.fileName() << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}


// struct ImageData{
//     unsigned char* data = nullptr;
//     int width = 0;
//     int height = 0;
//     int numChannels = 0;
//     std::string fileName = "";
// };

// void loadTexture(const ImageData& imageData, unsigned int* textureHandle){
//     glGenTextures(1, textureHandle);
//     glBindTexture(GL_TEXTURE_2D, *textureHandle);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     if (imageData.data) {
//         if(imageData.numChannels == 3){
//             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageData.width, imageData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data); 
//         }
//         if(imageData.numChannels == 4){
//             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.width, imageData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data); 
//         }
//         glGenerateMipmap(GL_TEXTURE_2D);
//     } else {
//         std::cerr << "Failed to load texture " + imageData.fileName << std::endl;
//     }
// }

}//namespace shb