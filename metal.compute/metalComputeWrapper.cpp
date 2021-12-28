//
//  metalAddWrapper.cpp
//  metal-test
//
//  Created by Sayan on 29/12/21.
//

#include "metalComputeWrapper.hpp"
#include <iostream>

void metalComputeWrapper::initWithDevice(MTL::Device* device) {
    mDevice = device;
    NS::Error* error;
    
    auto defaultLibrary = mDevice->newDefaultLibrary();
    
    if (!defaultLibrary) {
        std::cerr << "Failed to find the default library.\n";
        exit(-1);
    }
    
    auto functionName = NS::String::string("work_on_arrays", NS::ASCIIStringEncoding);
    auto computeFunction = defaultLibrary->newFunction(functionName);
    
    if(!computeFunction){
        std::cerr << "Failed to find the compute function.\n";
    }
    
    mComputeFunctionPSO = mDevice->newComputePipelineState(computeFunction, &error);
    
    if (!computeFunction) {
        std::cerr << "Failed to create the pipeline state object.\n";
        exit(-1);
    }
    
    mCommandQueue = mDevice->newCommandQueue();
    
    if (!mCommandQueue) {
        std::cerr << "Failed to find command queue.\n";
        exit(-1);
    }
}

void metalComputeWrapper::prepareData() {
    // Allocate three buffers to hold our initial data and the result.
    mBufferA = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    mBufferB = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    mBufferResult = mDevice->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
    
    generateRandomFloatData(mBufferA);
    generateRandomFloatData(mBufferB);
}

void metalComputeWrapper::generateRandomFloatData(MTL::Buffer * buffer) {
    float* dataPtr = (float*) buffer->contents();
    
    for(unsigned long int index = 0; index < ARRAY_LENGTH; index++)
        dataPtr[index] = float(rand())/float(RAND_MAX);
}

void metalComputeWrapper::sendComputeCommand() {
    // Create a command buffer to hold commands.
    MTL::CommandBuffer* commandBuffer = mCommandQueue->commandBuffer();
    assert(commandBuffer != nullptr);
    
    // Start a compute pass.
    MTL::ComputeCommandEncoder* computeEncoder = commandBuffer->computeCommandEncoder();
    assert(computeEncoder != nullptr);
    
    encodeComputeCommand(computeEncoder);
    
    // End the compute pass.
    computeEncoder->endEncoding();
    
    // Execute the command.
    commandBuffer->commit();
    
    // Normally, you want to do other work in your app while the GPU is running,
    // but in this example, the code simply blocks until the calculation is complete.
    commandBuffer->waitUntilCompleted();
    
    verifyResults();
}

void metalComputeWrapper::encodeComputeCommand(MTL::ComputeCommandEncoder * computeEncoder) {
    // Encode the pipeline state object and its parameters.
    computeEncoder->setComputePipelineState(mComputeFunctionPSO);
    computeEncoder->setBuffer(mBufferA, 0, 0);
    computeEncoder->setBuffer(mBufferB, 0, 1);
    computeEncoder->setBuffer(mBufferResult, 0, 2);
    
    MTL::Size gridSize = MTL::Size(ARRAY_LENGTH, 1, 1);
    
    // Calculate a threadgroup size.
    NS::UInteger threadGroupSize = mComputeFunctionPSO->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > ARRAY_LENGTH)
    {
        threadGroupSize = ARRAY_LENGTH;
    }
    MTL::Size threadgroupSize = MTL::Size(threadGroupSize, 1, 1);

    // Encode the compute command.
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
}

void metalComputeWrapper::verifyResults(){
    float* a = (float*) mBufferA->contents();
    float* b = (float*) mBufferB->contents();
    float* result = (float*) mBufferResult->contents();
    
    for(unsigned long int index = 0; index < ARRAY_LENGTH; index++)
        if(abs(result[index] - (a[index] * b[index])) > DELTA) {
            std::cout << "Compute ERROR: index= " << index << " result= " << result[index] <<  " vs " << a[index] * b[index] << " = a * b\n";
            
            assert(abs(result[index] - (a[index] * b[index])) > DELTA);
        }
    
    std::cout << "Compute results as expected.\n";
}
