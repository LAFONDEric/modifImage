#include <iostream>
#include <cstdlib>
#include "lodepng.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include "kernels.h"
#include "SansKernel.h"
#include <functional>




__host__ __device__ void donneDonnerRef() {
#ifdef __CUDA_ARCH__
    printf ("\nDevice Thread %d\n", threadIdx.x);
	
	
	  int val;
  cudaError_t err = cudaDeviceGetAttribute(&val, cudaDevAttrMaxThreadsPerBlock, 0);
  printf("err = %d\n", err);
  printf("val = %d\n", val);
  
  int smemSize, numProcs;
  int devId=0;
  cudaDeviceGetAttribute(&smemSize, cudaDevAttrMaxSharedMemoryPerBlock, devId);
  cudaDeviceGetAttribute(&numProcs, cudaDevAttrMultiProcessorCount, devId);
  
  printf("smemSize = %d\n", smemSize);
  printf("numProcs = %d\n", numProcs);

#else
    printf ("\nHost code!\n");

	  int val;
  cudaError_t err = cudaDeviceGetAttribute(&val, cudaDevAttrMaxThreadsPerBlock, 0);
  printf("err = %d\n", err);
  printf("val = %d\n", val);
  
  int smemSize, numProcs;
  int devId=0;
  cudaDeviceGetAttribute(&smemSize, cudaDevAttrMaxSharedMemoryPerBlock, devId);
  cudaDeviceGetAttribute(&numProcs, cudaDevAttrMultiProcessorCount, devId);
  
  printf("smemSize = %d\n", smemSize);
  printf("numProcs = %d\n", numProcs);
  	
	int major = 0, minor = 0, device = 0;
    cudaGetDevice(&device);
    cudaDeviceGetAttribute(&major, cudaDevAttrComputeCapabilityMajor, device);
    cudaDeviceGetAttribute(&minor, cudaDevAttrComputeCapabilityMinor, device);
    printf("Major=%d\n", major);
    printf("Minor=%d\n", minor);

#endif
}




//extern void filter(unsigned char* input_image, unsigned char* output_image, int width, int height);

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << "Run with input and output image filenames." << std::endl;
        return 0;
    }
    donneDonnerRef();
    // Read the arguments
    const char* input_file = argv[1];
    const char* output_file = argv[2];

    std::vector<unsigned char> in_image;
    unsigned int width, height;

    // Load the data
    unsigned error = lodepng::decode(in_image, width, height, input_file);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    // Prepare the data
	int in_imagesize=(in_image.size()*3)/4;
    unsigned char* input_image = new unsigned char[in_imagesize];
    unsigned char* output_image = new unsigned char[in_imagesize];
//    unsigned char* input_image = (unsigned char *)malloc(sizeof( unsigned char) * in_imagesize);
//    unsigned char* output_image = (unsigned char *)malloc(sizeof( unsigned char) * in_imagesize);
	
    int where = 0;
    for(int i = 0; i < in_image.size(); ++i) {
       if((i+1) % 4 != 0) {
           input_image[where] = in_image.at(i);
           output_image[where] = 255;
           where++;
       }
    }
    int idSelection;
    int idChoixKernel;
    // Run the filter on it

    std::cout<< "---------------------------------------------------------\n|      Choisir le mode d'execution u programme           |\n|  0- avec le Cpu                                        |\n|  1- avec le Gpu                                        |\n|  2- avec les deux                                      |\n ---------------------------------------------------------\n";
    std::cin >> idChoixKernel;

   std::cout<< "\n---------------------------------------------------------\n|      Choisir le programme a executer                   |\n|  0- blur                                               |\n|  1- Contour                                            |\n|  2- Netteter                                           |\n ---------------------------------------------------------\n";
     std::cin >> idSelection;

    if(idChoixKernel == 0){
         filterSansKernel(input_image, output_image, width, height, idSelection); 
    }
    else if (idChoixKernel == 1)
    {
        filter(input_image, output_image, width, height,idSelection); 
    }
    else if (idChoixKernel == 2)
    {
        filterSansKernel(input_image, output_image, width, height, idSelection); 
        filter(input_image, output_image, width, height,idSelection); 
    }
    

  
    // Prepare data for output
    std::vector<unsigned char> out_image;

    for(int i = 0; i < in_imagesize; ++i) {
 //   for(int i = 0; i < in_image.size(); ++i) {
        out_image.push_back(output_image[i]);
        if((i+1) % 3 == 0) {
            out_image.push_back(255);
        }
		
	unsigned char valtest=output_image[i];	
//			std::cout << "[i]="<< static_cast<int>(i) << std::endl;
//			std::cout << "output_image[i]="<< static_cast<unsigned>(valtest) << std::endl;
    }
	
    // Output the data
    error = lodepng::encode(output_file, out_image, width, height);

    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    delete[] input_image;
    delete[] output_image;
    return 0;
}




