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
    NUM_SORT_BY
};
class Pixel{
    public:
    double r = 0;
    double g = 0;
    double b = 0;
    double a = 0;
    double x = 0;
    double y = 0;
    double hue = 0;
    double sat = 0;
    double val = 0;
    
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

std::vector<Pixel> dataToPixels(ImageData& imageData){
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


void pixelsToData(ImageData& imageData, std::vector<Pixel>& pixels ){
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

//DRY :)
void shiftHue(ImageData& imageData, double shiftValue){ //FIX incurs a huge amount of data loss
    std::vector<Pixel> pixels = dataToPixels(imageData);
    for(int i = 0; i < pixels.size(); i++){
        pixels[i].hue += shiftValue;
        fmod(pixels[i].hue,360.0);
        setPixelRGBFromHSV(&pixels[i]);
    }
    pixelsToData(imageData,pixels);
}
void shiftSat(ImageData& imageData,double shiftValue){
    std::vector<Pixel> pixels = dataToPixels(imageData);
    for(int i = 0; i < pixels.size(); i++){
        pixels[i].sat += shiftValue;
        setPixelRGBFromHSV(&pixels[i]);
    }
    pixelsToData(imageData,pixels);
}
void shiftVal(ImageData& imageData,int shiftValue){
    std::vector<Pixel> pixels = dataToPixels(imageData);
    for(int i = 0; i < pixels.size(); i++){
        pixels[i].val += shiftValue;
        setPixelRGBFromHSV(&pixels[i]);
    }
    pixelsToData(imageData,pixels);
}


void testPixelEncoding(ImageData& imageData){
    std::vector<Pixel> pixels = dataToPixels(imageData);
    pixelsToData(imageData,pixels);
}



void sortPixels(ImageData& imageData){
    std::vector<Pixel> pixels = dataToPixels(imageData);

    //quickSort(pixels,0,pixels.size()-1);
    //bubbleSort(pixels);
    std::sort(pixels.begin()+1000,pixels.end()-1000); //not sure why custom sorting functions are failing

    pixelsToData(imageData, pixels);
    //return imageData;
}

}//namespace shb