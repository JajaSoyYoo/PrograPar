#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "stdio.h"

void handleError(cudaError_t err, const char* file, int line) {
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error at %s:%d: %s\n", file, line, cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

#define HANDLE_ERROR(err) (handleError(err, __FILE__, __LINE__))

int main(void) {
    cudaDeviceProp prop;
    int count = 0;

    // Obtener el número de dispositivos CUDA
    HANDLE_ERROR(cudaGetDeviceCount(&count));

    // Iterar sobre cada dispositivo
    for (int i = 0; i < count; i++) {
        HANDLE_ERROR(cudaGetDeviceProperties(&prop, i));
        printf(" --- General Information for device %d ---\n", i);
        printf("Name: %s\n", prop.name);
        printf("Compute capability: %d.%d\n", prop.major, prop.minor);
        printf("Clock rate: %d kHz\n", prop.clockRate); // Añadido 'kHz' para mayor claridad
        printf("Device copy overlap: %s\n", prop.deviceOverlap ? "Enabled" : "Disabled");
        printf("Kernel execution timeout: %s\n", prop.kernelExecTimeoutEnabled ? "Enabled" : "Disabled");

        printf(" --- Memory Information for device %d ---\n", i);
        printf("Total global mem: %ld bytes\n", prop.totalGlobalMem); // Añadido 'bytes' para mayor claridad
        printf("Total constant mem: %ld bytes\n", prop.totalConstMem);
        printf("Max mem pitch: %ld bytes\n", prop.memPitch);
        printf("Texture Alignment: %ld bytes\n", prop.textureAlignment);

        printf(" --- MP Information for device %d ---\n", i);
        printf("Multiprocessor count: %d\n", prop.multiProcessorCount);
        printf("Shared mem per mp: %ld bytes\n", prop.sharedMemPerBlock);
        printf("Registers per mp: %d\n", prop.regsPerBlock);
        printf("Threads in warp: %d\n", prop.warpSize);
        printf("Max threads per block: %d\n", prop.maxThreadsPerBlock);
        printf("Max thread dimensions: (%d, %d, %d)\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("Max grid dimensions: (%d, %d, %d)\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
        printf("\n");
    }

    return 0;
}