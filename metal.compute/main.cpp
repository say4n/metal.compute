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


int main(int argc, const char * argv[]) {
    NS::AutoreleasePool* pPool   = NS::AutoreleasePool::alloc()->init();
    MTL::Device* pDevice = MTL::CreateSystemDefaultDevice();
    
    // Create the custom object used to encapsulate the Metal code.
    // Initializes objects to communicate with the GPU.
    metalComputeWrapper* computer = new metalComputeWrapper();
    computer->initWithDevice(pDevice);
    
    // Create buffers to hold data
    computer->prepareData();
    
    // Send a command to the GPU to perform the calculation.
    computer->sendComputeCommand();
    
    pPool->release();
    
    std::cout << "Computation complete.\n";
    
    return 0;
}
