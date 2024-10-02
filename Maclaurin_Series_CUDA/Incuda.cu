#include <stdio.h>
#include <math.h>


__global__ void ln_function(float y, int n, float* result){
    float x = y -1.0f;
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < n){
        int sign = 1;
        if(idx%2 != 0){
            sign = -1;
        }
        result[idx]= sign * powf(x, idx+1) / (idx+1); 
    }
}


int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage ./Incuda <num_terms> <values>\n");
        return 0;
    }

    int numTerms = atoi(argv[1]);
    float value = atof(argv[2]);

    if (numTerms <= 0 || value<= 1 || value>2){
        printf("Invalid input\n");
        printf("%d, %f", numTerms, value);
        return 1;
    }

    int N = numTerms;

    float *c;
    c = (float*)malloc(sizeof(float)*N);
   
    float *dc;
    cudaMalloc((void**) &dc, sizeof(float)*N);

    //vector add
    ln_function<<<1, N>>>(value, numTerms, dc);
    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(error));
    }
    //cudaDeviceSynchronize();
    cudaDeviceSynchronize();

    //transfer data from device to host
    cudaMemcpy(c, dc, sizeof(float)*N, cudaMemcpyDeviceToHost);
    float result = 0.0f;
    for(int i = 0; i < N; i++){
        result += c[i];
        
        //printf("Term %d: %f\n", i, c[i]);
    }

    printf("%f\n", result);

    free(c); 
    cudaFree(dc); 

    return 0;
}