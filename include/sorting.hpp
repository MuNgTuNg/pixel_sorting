#pragma once
#include <vector>

template <typename T> 
int partition(std::vector<T>& pixels,int L,int R)
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

template <typename T> 
void quickSort(std::vector<T>& pixels,int L,int R)
{
  if(L<R)
  {
    int pi=partition(pixels,L,R);
    quickSort(pixels,L,pi-1);
    quickSort(pixels,pi+1,R);
  }
}

template <typename T> 
void bubbleSort(std::vector<T>& pixels) {
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

void swap(T& l, T& r){
    T temp = l;
    l = r; 
    r = temp;
}
void swap(int& l, int& r){
    int temp = l;
    l = r; 
    r = temp;
}

