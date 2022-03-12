#include "kernels.h"
#include "helpers.h"
#include <iostream>
#include <cmath>


__global__
void blur(unsigned char* input_image, unsigned char* output_image, int width, int height) {

    const unsigned int offset = blockIdx.x*blockDim.x + threadIdx.x;
    int x = offset % width;
    int y = (offset-x)/width;
    int fsize = 5; // Filter size
    if(offset < width*height) {

        float output_red = 0;
        float output_green = 0;
        float output_blue = 0;
        int hits = 0;
        for(int ox = -fsize; ox < fsize+1; ++ox) {
            for(int oy = -fsize; oy < fsize+1; ++oy) {
                if((x+ox) > -1 && (x+ox) < width && (y+oy) > -1 && (y+oy) < height) {
                    const int currentoffset = (offset+ox+oy*width)*3;
                    output_red += input_image[currentoffset]; 
                    output_green += input_image[currentoffset+1];
                    output_blue += input_image[currentoffset+2];
                    hits++;
                }
            }
        }
        output_image[offset*3] = output_red/hits;
        output_image[offset*3+1] = output_green/hits;
        output_image[offset*3+2] = output_blue/hits;
    }
}


__global__
void Contour(unsigned char* input_image, unsigned char* output_image, int width, int height) {


    const unsigned int offset = blockIdx.x*blockDim.x + threadIdx.x;
    int x = offset % width;
    int y = (offset-x)/width;
    int fsize = 1; // Filter size
    if(offset < width*height) {

        float output_red = 0;
        float output_green = 0;
        float output_blue = 0;
        int hits = 0;
        for(int ox = -fsize; ox < fsize+1; ++ox) {
            for(int oy = -fsize; oy < fsize+1; ++oy) {
                if((x+ox) > -1 && (x+ox) < width && (y+oy) > -1 && (y+oy) < height) {
                      const int currentoffset = (offset+ox+oy*width)*3;
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

        output_image[offset*3] = output_red;
        output_image[offset*3+1] = output_green;
        output_image[offset*3+2] = output_blue;
    }
}

__global__
void Netteter(unsigned char* input_image, unsigned char* output_image, int width, int height) {


    const unsigned int offset = blockIdx.x*blockDim.x + threadIdx.x;
    int x = offset % width;
    int y = (offset-x)/width;
    int fsize = 1; // Filter size
    if(offset < width*height) {

        float output_red = 0;
        float output_green = 0;
        float output_blue = 0;
        int hits = 0;
        for(int ox = -fsize; ox < fsize+1; ++ox) {
            for(int oy = -fsize; oy < fsize+1; ++oy) {
                if((x+ox) > -1 && (x+ox) < width && (y+oy) > -1 && (y+oy) < height) {
                      const int currentoffset = (offset+ox+oy*width)*3;
                    if(ox == 0 && oy == 0){
                       
                        output_red += input_image[currentoffset]*5; 
                        output_green += input_image[currentoffset+1]*5;
                        output_blue += input_image[currentoffset+2]*5;
                    }
                    else  if(ox == 0 || oy == 0){
                       
                        output_red -= input_image[currentoffset]; 
                        output_green -= input_image[currentoffset+1];
                        output_blue -= input_image[currentoffset+2];
                    }
                    else{
                        output_red += 0; 
                        output_green += 0;
                        output_blue += 0;
                   }
                    hits++;
                }
            }
        }

        output_image[offset*3] = output_red;
        output_image[offset*3+1] = output_green;
        output_image[offset*3+2] = output_blue;
    }
}





//extern "C" 
void filter (unsigned char* input_image, unsigned char* output_image, int width, int height, int idSelection) {

    unsigned char* dev_input;
    unsigned char* dev_output;
    getError(cudaMalloc( (void**) &dev_input, width*height*3*sizeof(unsigned char)));
	
	size_t intval= width*height*3*sizeof(unsigned char);
	
    getError(cudaMemcpy( dev_input, input_image, intval, cudaMemcpyHostToDevice ));
//	CUDA_SAFE_CALL(cudaMemcpy( dev_input, input_image, intval, cudaMemcpyHostToDevice )) //CUDA<=5
///	printf("size=%zd\n",intval);
 
    getError(cudaMalloc( (void**) &dev_output, width*height*3*sizeof(unsigned char)));

///    dim3 blockDims(512,1,1);
    dim3 blockDims(1024,1,1);
    dim3 gridDims((unsigned int) ceil((double)(width*height*3/blockDims.x)), 1, 1 );


    if(idSelection == 0){
        blur<<<gridDims, blockDims>>>(dev_input, dev_output, width, height); 
    }
    if(idSelection == 1){
        Contour<<<gridDims, blockDims>>>(dev_input, dev_output, width, height); 
    }
      if(idSelection == 2){
        Netteter<<<gridDims, blockDims>>>(dev_input, dev_output, width, height); 
    }
      printf ("\n finie \n");
    getError(cudaMemcpy(output_image, dev_output, width*height*3*sizeof(unsigned char), cudaMemcpyDeviceToHost ));

    getError(cudaFree(dev_input));
    getError(cudaFree(dev_output));

}

