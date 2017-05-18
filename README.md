# NUS Modules AY16-17

Compilation of all labs, assignments and other coding I did during my exchange year in Singapore! (AY2016-17)

## *Autumn 2016*:


## CS3210 - Parallel Computing

This module included all fundamental principles for parallel computation. Everything from memory architecture and computation models to message passing and basic parallel algorithm designs. Also included visits to NUS computer cluster and NSCC supercomputer (nr 93 in the world in 2016)

**_Assignment 1_** - Optimizing a matrix multiplication algorithm with CUDA. Final solution run on a Nvidia Jetson GPU.

**_Assignment 2_** - Designing a parallel algorithm to search for extraterrestrial life using MPI. Final solution run on cluster with 3 machines and NSCC supercomputer with 48, 200 and 3000 cores. 


## CS4243 - Computer Vision & Pattern Recognition

This module covered the math and heuristic principles behind modern Computer Vision.

**_Lab 1_** - Explore Python and Numpy

**_Lab 2_** - Color conversion and histogram equalization

**_Lab 3_** - Edge detection and thinning. 
Used convolution with Sobel and Prewitt filters for edge detection and non-max supression for thinning.

**_Lab 4_** - Harris corner detector

**_Lab 5_** - Background/foreground separation

**_Lab 6n7_** - Camera projection and homography matrix

**_Project_** - Process 7 video clips from Volleyboll games at the Olympic Games 2016. The assignment was to track the players and ball and project their positions on a top-down view of the court. Also to detect jumps and distance moved by players and to show a stitched video of the whole court. The camera rotated, zoomed and translated in all directions so for the top-down view we needed to track corners on the field, compute a frame-to-frame homography matrix and then a top-down homography matrix.

For code and result see: https://github.com/wojdziw/computer-vision-project


## CS4345 - General-purpose computations on the GPU

A different approach on parallel computation on the GPU than CS3210. First half dealt with GLSL, second half with CUDA. Here is was more focus on general purpose computations, data primitives, coalesced memory and memory conflicts.

**_Assignment 1_** - Computer graphics with OpenGL and GLSL. Included dealing with textures, tiling, bump mapping, cubemaps, lighting and OpenGL pipeline. Followed OpenGL 2.0 standard.

**_Assignment 2_** - Using GLSL shaders for general-purpose computations. In this case an **odd-even transition sort**.
 
**_Assignment 3_** - Write CUDA kernels for 1-D convolution. Kernel 1 **without shared memory** and Kernel 2 **with shared memory** (avoiding uncoalesced shared memory accesses and shared memory conflicts). Aimed at Compute Capability 1.0-1.1.

**_Assignment 4_** - Write a CUDA program that performs **stream compaction** using scan-and-scatter approach. The Thust lib can be used. Aimed at Compute Capability 1.0-1.1. 


## EE3204 - Computer Communication Networks I

The module covers the architecture of internet and other computer communication networks explaining the 7 layers with all functionality. Also encourages to use Wireshark to learn how the communication works.

**_Lab_** - Socket programming for UDP and TCP. Write a simple program using TCP with stop-and-wait with different error probabilities and data unit sizes. 


## *Spring 2017*


## CS4247 - Graphics Rendering Techniques

First part dealt with real-time rendering and the second part with offline rendering techniques for 3D graphics. Topics covered includes real-time mapping and shadows, local reflection models, global illumination, distributed ray tracing, photon mapping, radiosity, volume rendering, image-based rendering and strategies for anti-aliasing and photo-realism. 

**_Assignment 1_** - _Real-time rendering_, with texture mapping and reflection. The bonus assignment was to add shadows with the shadow mapping method.

**_Assignment 2_** - _Whitted Ray Tracing_. I implemented hit detection for spheres and completed the recursive tracing algorithm, for shadows and reflections. The bonus assignment was to extend the algorithm to render transparent objects as well. 

**_Assignment 3_** - _Progressive Refinement Radiosity_. Completed the Radiosity loop and created a new scene. 


## CS3243 - Introduction to Artificial Intelligence
Introductory module with no coding assignment outside the project. 

**_Project_** - The project was to develop an AI agent that played Tetris. We used a genetic algorithm with particle swarm optimization. 

Code and result: https://github.com/wojdziw/intro-to-ai-project


## CS3249 - User Interface Development
UI development focused on React and Redux. The module included 2 assignments, 8 tutorials and a project. 

The assignments and tutorials are uploaded here: https://github.com/adamalsegard/CS3249 

**_Project_** - The project was to develop an application for a real-life cancer screening project in Ethiopia. The application had 9 different UIs with a login/register page and synchronized database. The app was built with React.js, Redux, Node.js and RethinkDB.

Source code not accessible for the time being.
 

## CS4231 - Parallel and Distributed Algorithms
A theoretical module to design and analyse algorithms for parallel and distributed systems. No programming assignments. 


## EE4210 - Computer Communication Networks II
A continuation of EE3204. Topics covered included multimedia streaming, wireless networks, mobility management and network security. No programming assignments. 


