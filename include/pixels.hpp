#pragma once
#include <gui.hpp>
#include <algorithm>
#include <vector>
#include <textures.hpp>
#include <math.h>
#include <cstdlib> 
namespace shb{

enum EnumSortBy{
    SORT_BY_RED,
    SORT_BY_GREEN,
    SORT_BY_BLUE,
    SORT_BY_ALPHA,
    SORT_BY_HUE,
    SORT_BY_SATURATION,
    SORT_BY_VALUE,
    NUM_SORT_BY
};
class Pixel{
    public:
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    double a = 0.0;
    double x = 0.0;
    double y = 0.0;
    double hue = 0.0;
    double sat = 0.0;
    double val = 0.0;
    
    Pixel() = default;

    friend bool operator<(const Pixel& lhs, const Pixel& rhs){
        switch(m_SortBy){
            case SORT_BY_RED:
                return lhs.r < rhs.r;
                break;
            case SORT_BY_GREEN:
                return lhs.g < rhs.g;
                break;
            case SORT_BY_BLUE:
                return lhs.b < rhs.b;
                break;
            case SORT_BY_ALPHA:
                return lhs.a < rhs.a;
                break;
            case SORT_BY_HUE:
                return lhs.hue < rhs.hue;
                break;
            case SORT_BY_SATURATION:
                return lhs.sat < rhs.sat;
                break;
            case SORT_BY_VALUE:
                return lhs.val < rhs.val;
                break;
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

void getPixelHSV(Pixel& input){
    //normalise rgb values
    double r = input.r/255.0;
    double g = input.g/255.0;
    double b = input.b/255.0;
    

    double cmax = std::max(r,std::max(g,b));
    double cmin = std::min(r,std::min(g,b));
    double cdiff = cmax-cmin;  
    double hue = -1;
    double sat = -1;

    if(cmax == cmin){
    	hue = 0.0;
    }
    else{
        if(cmax == r){
            hue = 60.0*(g-b)/cdiff;
        }
        if(cmax == g){
            hue = 60.0*(2.0+ (b-r)/cdiff);
        }
        if(cmax == b){
            hue = 60.0*(4.0+ (r-g)/cdiff);
        }
    }

    if(hue < 0.0){
        hue +=360.0;
    }
    hue = fmod(hue,360.0);
    
    if(cmax == 0.0){
    	sat = 0.0;
    }
    else{
    	sat = (cdiff/cmax); 
    }
    
    double val = cmax;
    input.hue = hue;
    input.sat = sat;
    input.val = val;
}



void setPixelRGBFromHSV(Pixel& input){

    double val = input.val;
    double sat = input.sat;
    double hue = input.hue;

    if(sat <= 0.0){
        input.r = input.g = input.b = val * 255.0;
        return;
    }
    if(sat > 1.0){
        sat = 1.0;
    }
    if(val <= 0.0){
        input.r = input.g = input.b = 0.0;
        return;
    }
    if(val > 1.0){
        val = 1.0;
    }

    hue = fmod(hue, 360.0);
    if (hue < 0.0) {
        hue += 360.0;
    }

    double chroma = val * sat;
    double x = chroma*(1.0-(abs(fmod(hue/60.0,2.0)-1.0)));
    double m = val - chroma;

    double newR = 0.0;
    double newG = 0.0;
    double newB = 0.0;

    if(hue >=0.0 && hue < 60.0){
        newR = (chroma+m)*255.0;
        newG = (x+m)*255.0;
        newB = (0.0+m)*255.0; 
    }
    if(hue >= 60.0 && hue < 120.0){
        newR = (x+m)*255.0;
        newG = (chroma+m)*255.0;
        newB = (0.0+m)*255.0; 
    }
    if(hue >=120.0 && hue < 180.0){
        newR = (0.0+m)*255.0;
        newG = (chroma+m)*255.0;
        newB = (x+m)*255.0; 
    }
    if(hue >=180.0 && hue < 240.0){
        newR = (0.0+m)*255.0;
        newG = (x+m)*255.0;
        newB = (chroma+m)*255.0; 
    }
    if(hue >= 240.0 && hue < 300.0){
        newR = (x+m)*255.0;
        newG = (0.0+m)*255.0;
        newB = (chroma+m)*255.0; 
    }
    if(hue >= 300.0 && hue < 360.0){
        newR = (chroma+m)*255.0;
        newG = (0.0+m)*255.0;
        newB = (x+m)*255.0; 
    }

    input.r = newR;
    input.g = newG;
    input.b = newB;
}

void setPixelsRGBFromHSV(std::vector<Pixel>& pixels){
    for(int i = 0; i < pixels.size(); ++i){
        setPixelRGBFromHSV(pixels[i]);
    }
}

//imageData is passed as reference because if i have to traverse a pointer for every pixel then it's going
//to be very slow
std::vector<Pixel> mapPixelsToData(ImageData& imageData){
    int numEntries = imageData.width() * imageData.height() * imageData.numChannels();
    int width = imageData.width() * imageData.numChannels();
    
    std::vector<Pixel> pixels;
    Pixel pixel;

    //map pixels into rgb struct for sorting
    for(int i = 0; i < numEntries; i += imageData.numChannels()){
        pixel.x = i % width ;
        pixel.y = i/width;

       
        pixel.r = (double)imageData.m_Data[i];
        pixel.g = (double)imageData.m_Data[i+1];
        pixel.b = (double)imageData.m_Data[i+2];
        if(imageData.numChannels() > 3){
            pixel.a = imageData.m_Data[i+3];
        }
        getPixelHSV(pixel);
        pixels.push_back(pixel);
    }
    return pixels;
}

void setDataFromPixels(ImageData& imageData, std::vector<Pixel>& pixels ){
    int numEntries = imageData.width() * imageData.height() * imageData.numChannels();

    for(int i = 0, j = 0; i < numEntries && j < pixels.size(); i += imageData.numChannels(), j++){
        imageData.m_Data[i] = pixels[j].r;
        imageData.m_Data[i+1] = pixels[j].g;
        imageData.m_Data[i+2] = pixels[j].b;
        if(imageData.numChannels() > 3){
            imageData.m_Data[i+3] = pixels[j].a;
        }
    }
}

//not really necessary right now.
// class ImageDataPixels : public ImageData{
//     public:
//     ImageDataPixels(const std::string& fileName) : ImageData(fileName){
//         pixels = mapPixelsToData(*this);
//         originalPixels = pixels;
//     }
//     ImageDataPixels(const ImageData& imageData) : ImageData(imageData){}

//     void reset(){
//         setDataFromPixels(*this,originalPixels);
//     }
//     void updateData(){
//         setDataFromPixels(*this,pixels);
//     }
//     void updatePixels(){
//         pixels = mapPixelsToData(*this);
//     }
//     std::vector<Pixel> pixels;
//     std::vector<Pixel> originalPixels;
// };

double hueOffset = 0.0;
double valOffset = 0.0;
double satOffset = 0.0;

std::vector<Pixel> originalPixels;
void loadOriginalPixels(ImageData& imageData){
    originalPixels = mapPixelsToData(imageData);
}

void resetPixels(ImageData& imageData){
    setDataFromPixels(imageData,originalPixels);
    valOffset = 0;
    hueOffset = 0;
    satOffset = 0;
}

/*
FIX:: 
for some reason these "shift" functions corrupt the data when the offset
reaches zero or below
*/
void shiftHue(ImageData& imageData, double shiftValue){
    hueOffset  = shiftValue;
    std::vector<Pixel> pixels = mapPixelsToData(imageData);

    for(int i = 0; i < pixels.size(); i++){
        double newHue = originalPixels[i].hue + hueOffset;
        pixels[i].hue = newHue;
    }

    setPixelsRGBFromHSV(pixels);
    setDataFromPixels(imageData,pixels);
}


void shiftSat(ImageData& imageData,double shiftValue){
    std::vector<Pixel> pixels = mapPixelsToData(imageData);
    satOffset += shiftValue;
    for(int i = 0; i < pixels.size(); i++){
        double newSat =  originalPixels[i].sat + satOffset;
        if(newSat < 0){
            newSat = 0.1;
        }
        pixels[i].sat = newSat;
    }
    setPixelsRGBFromHSV(pixels);
    setDataFromPixels(imageData,pixels);
}


void shiftVal(ImageData& imageData, double shiftValue){
    std::vector<Pixel> pixels = mapPixelsToData(imageData);
    valOffset += shiftValue;

    for(int i = 0; i < pixels.size(); i++){
        double newVal = originalPixels[i].val + valOffset;
        if(newVal > 1.0){
            newVal = 1.0;
        }
        if(newVal < 0.0){
            newVal = 0.1;
        }
        pixels[i].val = newVal;
    }

    setPixelsRGBFromHSV(pixels);
    setDataFromPixels(imageData,pixels);
}



void testPixelEncoding(ImageData& imageData){
    std::vector<Pixel> pixels = mapPixelsToData(imageData);
    setDataFromPixels(imageData,pixels);
}


struct FromTo{
    int from =0;
    int to = 0;
};
void sortPixels(ImageData& imageData){
    //quickSort(pixels,0,pixels.size()-1);
    //bubbleSort(pixels);
    std::vector<Pixel> pixels = mapPixelsToData(imageData);
    std::vector<FromTo> fromTos;
    fromTos.push_back({pixels.size()/3,pixels.size()/3});
    fromTos.push_back({pixels.size()/2,pixels.size()/2});
    fromTos.push_back({pixels.size()/8,pixels.size()/8});


    for(int i = 0; i < fromTos.size(); ++i){
        std::sort(pixels.begin()+fromTos[i].from,pixels.end()-fromTos[i].to); //not sure why custom sorting functions are failing
    }
    setDataFromPixels(imageData,pixels);
}

void pickSortingMethod(){
    //items must be in the same order as the enum EnumSortBy
     const char* items[] = { 
        "Sort by red", "Sort by green","Sort by blue" , "Sort by alpha", "Sort by hue",
        "Sort by saturation", "Sort by value" };
        static int item_current_idx = 0;
        if (ImGui::BeginListBox("Sort"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                    item_current_idx = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
                Pixel::m_SortBy = (EnumSortBy)(SORT_BY_RED+item_current_idx);
            }
            ImGui::EndListBox();
        }
}



}//namespace shb