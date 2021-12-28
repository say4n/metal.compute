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
    
    metalComputeWrapper* computer = new metalComputeWrapper();
    computer->initWithDevice(pDevice);
    computer->prepareData();
    computer->sendComputeCommand();
    
    pPool->release();
    
    std::cout << "Computation complete.\n";
    
    return 0;
}
