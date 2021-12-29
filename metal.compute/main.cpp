//
//  main.cpp
//  metal-test
//
//  Created by Sayan on 28/12/21.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include "metalComputeWrapper.hpp"
#include <iostream>
#include <chrono>


int main(int argc, const char * argv[]) {
    NS::AutoreleasePool* pPool   = NS::AutoreleasePool::alloc()->init();
    MTL::Device* pDevice = MTL::CreateSystemDefaultDevice();
    
    // Create the custom object used to encapsulate the Metal code.
    // Initializes objects to communicate with the GPU.
    metalComputeWrapper* computer = new metalComputeWrapper();
    computer->initWithDevice(pDevice);
    
    // Create buffers to hold data
    computer->prepareData();
    
    // Time the compute phase.
    auto start = std::chrono::steady_clock::now();
    
    // Send a command to the GPU to perform the calculation.
    computer->sendComputeCommand();
    
    // End of compute phase.
    auto end = std::chrono::steady_clock::now();
    auto delta_time = end - start;
    
    pPool->release();
    
    std::cout << "Computation completed in "
            << std::chrono::duration <double, std::milli> (delta_time).count()
            << " ms for array of size "
            << ARRAY_LENGTH
            <<".\n";
    
    return 0;
}
