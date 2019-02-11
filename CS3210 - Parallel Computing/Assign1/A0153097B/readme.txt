## Compiling instructions

### Compile on Jetson GPU with:

nvcc –arch=sm_32 mm-cuda.cu –o mm-cuda –lcuda -lcudart

### Run with:

./mm-cuda <matrix-size>

Code benchmarked with matrix sizes 512, 600 & 1024
