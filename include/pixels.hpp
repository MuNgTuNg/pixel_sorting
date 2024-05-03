#pragma once
#include <gui.hpp>
#include <algorithm>
#include <vector>
#include <textures.hpp>


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

    double originalHue = 0.0;
    double originalSat = 0.0;
    double originalVal = 0.0;

    
    Pixel() = default;
    Pixel(const Pixel& rhs){
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        a = rhs.a;
        x = rhs.x;
        y = rhs.y;
        hue = rhs.hue;
        sat = rhs.sat;
        val = rhs.val;
    }

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

Pixel getPixelHSV(Pixel* input){
    //normalise rgb values
    double r = input->r/255.0;
    double g = input->g/255.0;
    double b = input->b/255.0;
    

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
    input->hue = hue;
    input->sat = sat;
    input->val = val;
    input->originalHue = hue;
    input->originalSat = sat;
    input->originalVal = val;

    Pixel ret = *input;
    return ret;
}

void setPixelRGBFromHSV(Pixel* input){
    double val = input->val;
    double sat = input->sat;
    double hue = input->hue;

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

    input->r = newR;
    input->g = newG;
    input->b = newB;
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

       
        pixel.r = imageData.m_Data[i];
        pixel.g = imageData.m_Data[i+1];
        pixel.b = imageData.m_Data[i+2];
        if(imageData.numChannels() > 3){
            pixel.a = imageData.m_Data[i+3];
        }
        getPixelHSV(&pixel);
        pixels.push_back(pixel);
    }
    return pixels;
}

void setDataFromPixels(ImageData& imageData, std::vector<Pixel>& pixels ){
    int numEntries = imageData.width() * imageData.height() * imageData.numChannels();

    for(int i = 0, j = 0; i < numEntries, j < pixels.size(); i += imageData.numChannels(), j++){
        imageData.m_Data[i] = pixels[j].r;
        imageData.m_Data[i+1] = pixels[j].g;
        imageData.m_Data[i+2] = pixels[j].b;
        if(imageData.numChannels() > 3){
            imageData.m_Data[i+3] = pixels[j].a;
        }
    }
}


class ImageDataPixels : public ImageData{
    public:
    ImageDataPixels(const std::string& fileName) : ImageData(fileName){
        pixels = mapPixelsToData(*this);
        originalPixels = pixels;
    }
    ImageDataPixels(const ImageData& imageData) : ImageData(imageData){}

    void reset(){
        setDataFromPixels(*this,originalPixels);
    }
    void updateData(){
        setDataFromPixels(*this,pixels);
    }
    std::vector<Pixel> pixels;
    std::vector<Pixel> originalPixels;
};

void shiftHue(ImageDataPixels& imageData, int offset){ //not sure if breaks?
    for(int i = 0; i < imageData.pixels.size(); i++){
        double newHue = imageData.pixels[i].originalHue + offset;
        newHue = fmod(newHue,360.0);
        if(newHue < 0){
            newHue = 360;
        }
        imageData.pixels[i].hue = newHue;
        setPixelRGBFromHSV(&imageData.pixels[i]);
    }
    imageData.updateData();
}

void shiftSat(ImageDataPixels& imageData,double shiftValue){ //FIX:: this breaks
    for(int i = 0; i < imageData.pixels.size(); i++){
        imageData.pixels[i].sat += shiftValue;
        if(imageData.pixels[i].sat < 0.0){
            imageData.pixels[i].sat = 0.0;
        }
        if(imageData.pixels[i].sat > 1.0){
            imageData.pixels[i].sat = 1.0;
        }
        setPixelRGBFromHSV(&imageData.pixels[i]);
    }
    imageData.updateData();
}

void shiftVal(ImageDataPixels& imageData,int shiftValue){ //FIX:: this breaks
    for(int i = 0; i < imageData.pixels.size(); i++){
        imageData.pixels[i].val += shiftValue;
        if(imageData.pixels[i].val < 0.0){
            imageData.pixels[i].val = 0.0;
        }
        if(imageData.pixels[i].val > 1.0){
            imageData.pixels[i].val = 1.0;
        }
        setPixelRGBFromHSV(&imageData.pixels[i]);
    }
    imageData.updateData();
}



void resetHSV(ImageDataPixels& imageData){  //TODO:: reset not working as intended
    for(int i = 0; i < imageData.pixels.size(); i++){
        imageData.pixels[i].hue = imageData.pixels[i].originalHue;
        imageData.pixels[i].sat = imageData.pixels[i].originalSat;
        //imageData.pixels[i].val = imageData.pixels[i].originalVal;
        setPixelRGBFromHSV(&imageData.pixels[i]);
    }
    imageData.reset();
}



void testPixelEncoding(ImageData& imageData){
    std::vector<Pixel> pixels = mapPixelsToData(imageData);
    setDataFromPixels(imageData,pixels);
}

void sortPixels(ImageDataPixels& imageData){
    //quickSort(pixels,0,pixels.size()-1);
    //bubbleSort(pixels);
    std::sort(imageData.pixels.begin()+1000,imageData.pixels.end()-1000); //not sure why custom sorting functions are failing
    imageData.updateData(); 
}

void pickSortingMethod(){
    //items must be in the same order as the enum EnumSortBy
     const char* items[] = { 
        "Sort by red", "Sort by green","Sort by blue" , "Sort by alpha", "Sort by hue",
        "Sort by saturation", "Sort by value" };
        static int item_current_idx = 0;
        if (ImGui::BeginListBox("Sort color:\n  "))
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