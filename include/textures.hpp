#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <gui.hpp>
#include <algorithm>

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

    //void pixel(int x, int y);
    //TODO:: make access functions like indexing into an array of pixels
    unsigned char* m_Data = nullptr;

  private:
    std::string m_FileName = "";
    int m_ImageWidth = 0;
    int m_ImageHeight = 0;
    int m_NumChannels = 0;
};

void loadTexture(const ImageData& imageData, unsigned int* textureHandle){
    if(textureHandle== 0){
        glGenTextures(1, textureHandle);
    }
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

enum EnumSortBy{
    SORT_BY_RED,
    SORT_BY_GREEN,
    SORT_BY_BLUE,
    SORT_BY_ALPHA,
    NUM_SORT_BY
};
class Pixel{
    public:
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;
    int x = 0;
    int y = 0;
    
    Pixel() = default;
    Pixel(const Pixel& rhs){
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        a = rhs.a;
        x = rhs.x;
        y = rhs.y;
    }

    friend bool operator<(const Pixel& lhs, const Pixel& rhs){
        if(m_SortBy == SORT_BY_RED){
            return lhs.r < rhs.r;
        }
        if(m_SortBy == SORT_BY_GREEN){
            return lhs.g < rhs.g;
        }
        if(m_SortBy == SORT_BY_BLUE){
            return lhs.b < rhs.b;
        }
        if(m_SortBy == SORT_BY_ALPHA){
            return lhs.a < rhs.a;
        }
        
    }
    friend bool operator>(const Pixel& lhs, const Pixel& rhs){
        return rhs < lhs;
    }
    friend bool operator==(const Pixel& lhs, const Pixel& rhs) {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a && lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend bool operator!=(const Pixel& lhs, const Pixel& rhs) {
        return !(lhs == rhs);
    }

    static EnumSortBy m_SortBy;
};
EnumSortBy Pixel::m_SortBy = SORT_BY_GREEN;

void swap(Pixel& l, Pixel& r){
    Pixel temp = l;
    l = r; 
    r = temp;
}
void swap(int& l, int& r){
    int temp = l;
    l = r; 
    r = temp;
}

int partition(std::vector<Pixel>& pixels,int L,int R)
{
  Pixel pivot=pixels[R];
  int i=(L-1);
  
  for(int j=L;j<=R;j++)
  {

    if(pixels[j]<pivot)
    {
      i++;
      swap(pixels[i],pixels[j]);
    }
  }
  swap(pixels[i+1],pixels[R]);
  return (i+1);
}


void quickSort(std::vector<Pixel>& pixels,int L,int R)
{
  if(L<R)
  {
    int pi=partition(pixels,L,R);
    quickSort(pixels,L,pi-1);
    quickSort(pixels,pi+1,R);
  }
}
void bubbleSort(std::vector<Pixel>& pixels) {
    int n = pixels.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            // Compare adjacent elements
            if (pixels[j] > pixels[j + 1]) {
                // Swap if they are in the wrong order
                swap(pixels[j], pixels[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped in the inner loop, the array is sorted
        if (!swapped) {
            break;
        }
    }
}

void sortPixels(ImageData& imageData){
    int numEntries = imageData.width() * imageData.height() * imageData.numChannels();
    
    int numChannels = imageData.numChannels();
    int width = imageData.width() * imageData.numChannels();
    int height = imageData.height() * imageData.numChannels();
    
    if(g_ImGuiFrameBegin){
        ImGui::Text("Num int values: %d\nInt values X: %d\nInt values Y: %d\n",numEntries,width,height);
    }

    std::vector<Pixel> pixels;
    Pixel pixel;

    //map pixels into rgb struct for sorting
    for(int i = 0; i < numEntries; i += numChannels){
        pixel.x = i % width ;
        pixel.y = i/width;

       
        pixel.r = imageData.m_Data[i];
        pixel.g = imageData.m_Data[i+1];
        pixel.b = imageData.m_Data[i+2];
        if(numChannels > 3){
            pixel.a = imageData.m_Data[i+3];
        }
        pixels.push_back(pixel);
    }

    //quickSort(pixels,0,pixels.size()-1);
    //bubbleSort(pixels);
    std::sort(pixels.begin(),pixels.end()); //not sure why custom sorting functions are failing

    for(int i = 0, j = 0; i < numEntries, j < pixels.size(); i += numChannels, j++){
        imageData.m_Data[i] = pixels[j].r;
        imageData.m_Data[i+1] = pixels[j].g;
        imageData.m_Data[i+2] = pixels[j].b;
        if(numChannels > 3){
            imageData.m_Data[i+3] = pixels[j].a;
        }
    }
    //return imageData;
}

void pickSortingMethod(){
     const char* items[] = { "Sort by red", "Sort by blue", "Sort by green", "Sort by alpha" };
        static int item_current_idx = 0; // Here we store our selection data as an index.
        if (ImGui::BeginListBox("Sort color:\n  "))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();

                if(items[item_current_idx] == "Sort by red"){
                    Pixel::m_SortBy = SORT_BY_RED;
                }
                if(items[item_current_idx] == "Sort by blue"){
                    Pixel::m_SortBy = SORT_BY_BLUE;
                }
                if(items[item_current_idx] == "Sort by green"){
                    Pixel::m_SortBy = SORT_BY_GREEN;
                }
                if(items[item_current_idx] == "Sort by alpha"){
                    Pixel::m_SortBy = SORT_BY_ALPHA;
                }
                
                
            }
            ImGui::EndListBox();
        }
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