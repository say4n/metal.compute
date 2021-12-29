//
//  metalAddWrapper.hpp
//  metal-test
//
//  Created by Sayan on 29/12/21.
//

#ifndef metalComputeWrapper_hpp
#define metalComputeWrapper_hpp

#include <Metal/Metal.hpp>

const unsigned int ARRAY_LENGTH = 100000000;
const unsigned int BUFFER_SIZE = ARRAY_LENGTH * sizeof(float);

const float DELTA = 1e-6;


class metalComputeWrapper {
public:
    MTL::Device* mDevice;
    
    // The compute pipeline generated from the compute kernel in the .metal shader file.
    MTL::ComputePipelineState* mComputeFunctionPSO;
    
    // The command queue used to pass commands to the device.
    MTL::CommandQueue* mCommandQueue;
    
    // Buffers to hold data.
    MTL::Buffer *mBufferA;
    MTL::Buffer *mBufferB;
    MTL::Buffer *mBufferResult;
    
    void initWithDevice(MTL::Device*);
    void prepareData();
    void sendComputeCommand();
    
private:
    void encodeComputeCommand(MTL::ComputeCommandEncoder*);
    void generateRandomFloatData(MTL::Buffer*);
    void verifyResults();
};

#endif /* metalComputeWrapper_hpp */
