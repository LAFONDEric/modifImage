#include "SansKernel.h"
#include <iostream>
void filterSansKernel(unsigned char* input_image, unsigned char* output_image, int width, int height, int idSelection){
  if(idSelection == 0){
     blurSansKernel( input_image,  output_image,  width,  height);

  }
  if(idSelection == 1){
     ContourSansKernel( input_image,  output_image,  width,  height);

  }
    std::cout<<"okes";
}

void blurSansKernel(unsigned char* input_image, unsigned char* output_image, int width, int height ) {
 
    int fsize = 5;
    int nbdePixel = 0;
    //for(int x = 0; x < width; x++) {
      //  for(int y = 0; y < height; y ++) {

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y ++) {
        int hits = 0;
  
        nbdePixel++;
        float output_red = 0;
        float output_green = 0;
        float output_blue = 0; 
        int test = 0;
        int testDe = 0;
          for(int ox = -fsize; ox < fsize+1; ++ox) { 
                for(int oy = -fsize; oy < fsize+1; ++oy) {

                   if((x+ox) > -1 && (x+ox) < width && (y+oy) > -1 && (y+oy) < height) {
                        const int currentoffset = ((x + y *width)+ox+oy*width)*3;
                        output_red += input_image[currentoffset]; 
                        output_green += input_image[currentoffset+1];
                        output_blue += input_image[currentoffset+2];
                        hits++;
                   }
                }
          }
        output_image[(x + y *width) * 3] = output_red/hits;
        output_image[(x + y *width) * 3+1] = output_green/hits;
        output_image[(x + y *width) * 3+2] = output_blue/hits;
       
        }
    }
     std::cout<< "nb de pixel : "<< nbdePixel << "\n";
       
}

void ContourSansKernel(unsigned char* input_image, unsigned char* output_image, int width, int height ) {
 
    int fsize = 1;
    int nbdePixel = 0;
    //for(int x = 0; x < width; x++) {
      //  for(int y = 0; y < height; y ++) {

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y ++) {
        int hits = 0;
  
        nbdePixel++;
        float output_red = 0;
        float output_green = 0;
        float output_blue = 0; 
  
          for(int ox = -fsize; ox < fsize+1; ++ox) { 
                for(int oy = -fsize; oy < fsize+1; ++oy) {

                   if((x+ox) > -1 && (x+ox) < width && (y+oy) > -1 && (y+oy) < height) {
                        const int currentoffset = ((x + y *width)+ox+oy*width)*3;
                        if(ox == 0 && oy == 0){
                       
                        output_red += input_image[currentoffset]*8; 
                        output_green += input_image[currentoffset+1]*8;
                        output_blue += input_image[currentoffset+2]*8;
                    }
                    else{
                        output_red += input_image[currentoffset] * (-1); 
                        output_green += input_image[currentoffset+1] * (-1);
                        output_blue += input_image[currentoffset+2] * (-1);
                   }
                    hits++;
                   }
                }
          }
        output_image[(x + y *width) * 3] = output_red;
        output_image[(x + y *width) * 3+1] = output_green;
        output_image[(x + y *width) * 3+2] = output_blue;
       
        }
    }
     std::cout<< "nb de pixel : "<< nbdePixel << "\n";
       
}