//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : e0025203@u.nus.edu
// COMMENTS TO GRADER:
// -
//
//============================================================
//
// FILE: assign2.cpp


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "shader_util.h"
#include "Timer.h"


/////////////////////////////////////////////////////////////////////////////
// CONSTANTS & GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

// Size of input texture.
static const int texWidth = 200; //200;  // Always an even number.
static const int texHeight = 125; // 125;
// Size of the input array of elements.
static const int numInputElems = texWidth * texHeight;  // Always an even number.

// Shaders' filenames.
static const char vertShaderFilename[] = "assign2.vert";
static const char fragShaderFilename[] = "assign2.frag";

// Shader program object.
static GLuint shaderProg;

// GLUT window handle.
static GLuint glutWindowHandle;




/////////////////////////////////////////////////////////////////////////////
// Perform Odd-Even Sort on the CPU.
// Copy the input array and sort it in non-decreasing order on the CPU.
// Note that the memory for outputArray must be already allocated.
/////////////////////////////////////////////////////////////////////////////
static void CPU_OddEvenSort( const float inputArray[], float outputArray[], int numElems )
{
    for ( int i = 0; i < numElems; i++ ) outputArray[i] = inputArray[i];

    for ( int i = 0; i < numElems; i++ )  // Doing numElems passes.
    {
        for ( int j = i % 2; j < numElems - 1; j += 2 )
            if ( outputArray[j] > outputArray[j + 1] )
            {
                float temp = outputArray[j];
                outputArray[j] = outputArray[j + 1];
                outputArray[j + 1] = temp;
            }
    }
}




/////////////////////////////////////////////////////////////////////////////
// Check framebuffer status.
// Modified from the sample code provided in the 
// GL_EXT_framebuffer_object extension sepcifications.
/////////////////////////////////////////////////////////////////////////////
static bool CheckFramebufferStatus() 
{
    GLenum status = (GLenum) glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
    switch( status ) 
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            return true;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            fprintf( stderr, "Framebuffer incomplete, incomplete attachment\n" );
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            fprintf( stderr, "Framebuffer incomplete, missing attachment\n" );
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            fprintf( stderr, "Framebuffer incomplete, attached images must have same dimensions\n" );
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            fprintf( stderr, "Framebuffer incomplete, attached images must have same format\n" );
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            fprintf( stderr, "Framebuffer incomplete, missing draw buffer\n" );
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            fprintf( stderr, "Framebuffer incomplete, missing read buffer\n" );
            return false;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            fprintf( stderr, "Unsupported framebuffer format\n" );
            return false;
    }
    return false;
}




/////////////////////////////////////////////////////////////////////////////
// Perform Odd-Even Sort on the GPU (using OpenGL and GLSL).
// Copy the input array and sort it in non-decreasing order on the GPU.
// Note that the memory for outputArray must be already allocated.
/////////////////////////////////////////////////////////////////////////////
static void GPU_OddEvenSort( const float inputArray[], float outputArray[], int texWidth, int texHeight )
{
//-----------------------------------------------------------------------------
// Create two floating-point textures. 
// Use texture rectangle and texture internal format GL_ALPHA32F_ARB.
//-----------------------------------------------------------------------------
    // Texture A.
    glActiveTexture( GL_TEXTURE0 );
    GLuint texA;
    glGenTextures( 1, &texA );
    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, texA );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_ALPHA32F_ARB,
                  texWidth, texHeight, 0, GL_ALPHA, GL_FLOAT, inputArray );
    printOpenGLError();

    // Texture B.
    glActiveTexture( GL_TEXTURE1 );
    GLuint texB;
    glGenTextures( 1, &texB );
    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, texB );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_ALPHA32F_ARB,
                  texWidth, texHeight, 0, GL_ALPHA, GL_FLOAT, NULL );
    printOpenGLError();

//-----------------------------------------------------------------------------
// Attach the two textures to a FBO.
//-----------------------------------------------------------------------------
    GLuint fbo;
    glGenFramebuffersEXT( 1, &fbo ); 
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo );

    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
                               GL_TEXTURE_RECTANGLE_ARB, texA, 0 );
    CheckFramebufferStatus();
    printOpenGLError();

    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, 
                               GL_TEXTURE_RECTANGLE_ARB, texB, 0 );
    CheckFramebufferStatus();
    printOpenGLError();

//-----------------------------------------------------------------------------
// Deploy user-defined shaders.
//-----------------------------------------------------------------------------
    glUseProgram( shaderProg );

    GLint uniInputTexLoc = glGetUniformLocation( shaderProg, "InputTex" );
    GLint uniTexWidthLoc = glGetUniformLocation( shaderProg, "TexWidth" );
    GLint uniTexHeightLoc = glGetUniformLocation( shaderProg, "TexHeight" );
    GLint uniPassCountLoc = glGetUniformLocation( shaderProg, "PassCount" );

    // These uniform variables' values do not change in all rendering passes.
    glUniform1i( uniTexWidthLoc, texWidth );
    glUniform1i( uniTexHeightLoc, texHeight );


//-----------------------------------------------------------------------------
// Set some OpenGL states and projection.
//-----------------------------------------------------------------------------
    glDisable( GL_DITHER );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_BLEND );
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_COLOR_LOGIC_OP );
    glDisable( GL_SCISSOR_TEST );
    glDisable( GL_STENCIL_TEST );
    glPolygonMode( GL_FRONT, GL_FILL );

    // Set up projection, modelview matrices and viewport.

	//*********** EDITED **************

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, texWidth, 0, texHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, texWidth, texHeight);

//-----------------------------------------------------------------------------
// Perform the rendering passes.
// Use the ping-pong technique.
//-----------------------------------------------------------------------------

	// Why do we start with the wrong numbers?
	// Edit: because we start by switching instead of starting by rendering in ping-pong...
    GLint inputTextureUnit = 1;
    GLenum outputFBOAttachement = GL_COLOR_ATTACHMENT0_EXT;

    glFinish();  // For timing purpose only.
    Timer timer2;
    timer2.start();

    // Note that the first pass is passCount == 0.
    for ( int passCount = 0; passCount < texWidth * texHeight; passCount++ )
    {

    //---------------------------------------------------------
    // Prepare for next rendering pass.
    //---------------------------------------------------------
        // Swap input and output textures.

		//*********** EDITED **************
		if (inputTextureUnit == 0)
			inputTextureUnit = 1;
		else
			inputTextureUnit = 0;

		if (outputFBOAttachement == GL_COLOR_ATTACHMENT1_EXT)
			outputFBOAttachement = GL_COLOR_ATTACHMENT0_EXT;
		else
			outputFBOAttachement = GL_COLOR_ATTACHMENT1_EXT;

    //---------------------------------------------------------
    // Perform a rendering pass.
    //---------------------------------------------------------
        glDrawBuffer( outputFBOAttachement );

        glUniform1i( uniInputTexLoc, inputTextureUnit );
        glUniform1i( uniPassCountLoc, passCount );

        // Draw a filled quad with size equal to the output size.

		//*********** EDITED **************
		//glPolygonMode(GL_FRONT, GL_FILL); // Already defined above (line 191)
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(texWidth, 0);
			glVertex2f(texWidth, texHeight);
			glVertex2f(0, texHeight);
		glEnd();

        CheckFramebufferStatus();
        printOpenGLError();
    }

    glFinish();  // For timing purpose only.
    timer2.stop();
    printf( "Time elapsed (exclude CPU-GPU data transfer) = %.3f sec\n", timer2.getElapsedTimeInSec() );


//-----------------------------------------------------------------------------
// Read output buffer/texture to CPU memory.
//-----------------------------------------------------------------------------
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    glReadBuffer( outputFBOAttachement );
    glReadPixels( 0, 0, texWidth, texHeight, GL_ALPHA, GL_FLOAT, outputArray );
    printOpenGLError();

//-----------------------------------------------------------------------------
// Clean up.
//-----------------------------------------------------------------------------
    glDeleteFramebuffersEXT( 1, &fbo );
    glDeleteTextures( 1, &texA );
    glDeleteTextures( 1, &texB );
}




/////////////////////////////////////////////////////////////////////////////
// Create a OpenGL rendering context. 
// Check for OpenGL 2.0 and the necessary OpenGL extensions. 
// Read in the shaders from files to create a shader program object.
/////////////////////////////////////////////////////////////////////////////
static void PrepareGPUExecution( int argc, char** argv )
{
// Initialize GLUT.
    glutInit( &argc, argv );

    // This creates a OpenGL rendering context so that
    // we can start to issue OpenGL commands after this.
    glutWindowHandle = glutCreateWindow( "" );  
 
// Initialize GLEW.
    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        fprintf( stderr, "Error: %s.\n", glewGetErrorString( err ) );
        char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
        exit( 1 );
    }

// Make sure OpenGL 2.0 is supported.
    if ( !GLEW_VERSION_2_0 )
    {
        fprintf( stderr, "Error: OpenGL 2.0 is not supported.\n" );
        char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
        exit( 1 );
    }

// Make sure necessary OpenGL extensions are supported.
    if ( !GLEW_ARB_texture_float || 
         !GLEW_EXT_framebuffer_object || 
         !GLEW_ARB_texture_rectangle ||
         !GLEW_EXT_gpu_shader4 )
    {
        fprintf( stderr, "Error: Some necessary OpenGL extensions are not supported.\n" );
        char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
        exit( 1 );
    }

// Create shader program object.
    shaderProg = makeShaderProgramFromFiles( vertShaderFilename, fragShaderFilename, NULL );
    if ( shaderProg == 0 )
    {
        fprintf( stderr, "Error: Cannot create shader program object.\n" );
        char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
        exit( 1 );
    }
}




/////////////////////////////////////////////////////////////////////////////
// Generate an array of random numbers.
// Note that the memory for outputArray must be already allocated.
/////////////////////////////////////////////////////////////////////////////
static void GenerateInputArray( float outputArray[], int numElems )
{
    for ( int i = 0; i < numElems; i++ )
        outputArray[i] = (float) ( rand() * 1024 + rand() );
}




/////////////////////////////////////////////////////////////////////////////
// Return true iff all corresponding elements in arrays A and B are 
// approximately equal (i.e. the absolute difference is within the
// given threshold).
/////////////////////////////////////////////////////////////////////////////
static bool ArrayEqual( const float A[], const float B[], int numElems, float threshold )
{
    for ( int i = 0; i < numElems; i++ )
        if ( fabs( A[i] - B[i] ) > threshold ) return false;

    return true;
}




/////////////////////////////////////////////////////////////////////////////
// Return true iff all elements in arrays A are in non-decreasing sorted order.
/////////////////////////////////////////////////////////////////////////////
static bool IsSorted_NonDecreasing( const float A[], int numElems )
{
    for ( int i = 0; i < numElems - 1; i++ )
        if ( A[i] > A[i+1] ) return false;

    return true;
}




/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
    Timer timer;

    srand( 123 ); // To use same set of random numbers every run.

//-----------------------------------------------------------------------------
// Allocate memory for input and output arrays.
//-----------------------------------------------------------------------------
    float *inputArray = (float*) malloc( numInputElems * sizeof(float) );
    float *CPU_outputArray = (float*) malloc( numInputElems * sizeof(float) );
    float *GPU_outputArray = (float*) malloc( numInputElems * sizeof(float) );

    if ( inputArray == NULL || CPU_outputArray == NULL || GPU_outputArray == NULL )
    {
        fprintf( stderr, "Error: Not enough memory.\n" );
        char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
        exit( 1 );
    }

//-----------------------------------------------------------------------------
// Generate elements in input array.
//-----------------------------------------------------------------------------
    GenerateInputArray( inputArray, numInputElems );


//-----------------------------------------------------------------------------
// Perform computation on CPU.
//-----------------------------------------------------------------------------
    printf( "CPU COMPUTATION:\n" );
    printf( "================\n" );
    printf( "Number of input elements = %d\n", numInputElems );
    printf( "\n" );

    timer.start();

    CPU_OddEvenSort( inputArray, CPU_outputArray, numInputElems );

    timer.stop();
    printf( "Time elapsed = %.3f sec\n", timer.getElapsedTimeInSec() );
    
    if ( IsSorted_NonDecreasing( CPU_outputArray, numInputElems ) )
        printf( "CORRECT. Output array is in non-decreasing sorted order.\n" );
    else
        printf( "WRONG!!! Output array is NOT in non-decreasing sorted order.\n" );

    printf( "\n\n" );



//-----------------------------------------------------------------------------
// Perform computation on GPU.
//-----------------------------------------------------------------------------
    printf( "GPU COMPUTATION:\n" );
    printf( "================\n" );
    printf( "Number of input elements = %d\n", numInputElems );
    printf( "Texture Width  = %d\n", texWidth );
    printf( "Texture Height = %d\n", texHeight );
    printf( "\n" );

    PrepareGPUExecution( argc, argv );

    timer.start();

    GPU_OddEvenSort( inputArray, GPU_outputArray, texWidth, texHeight );

    timer.stop();
    printf( "Time elapsed = %.3f sec\n", timer.getElapsedTimeInSec() );

    if ( IsSorted_NonDecreasing( GPU_outputArray, numInputElems ) )
        printf( "CORRECT. Output array is in non-decreasing sorted order.\n" );
    else
        printf( "WRONG!!! Output array is NOT in non-decreasing sorted order.\n" );

    printf( "\n\n" );


//-----------------------------------------------------------------------------
// Verify results.
//-----------------------------------------------------------------------------
    if ( ArrayEqual( CPU_outputArray, GPU_outputArray, numInputElems, 0.0f ) )
        printf( "GPU result EQUAL to CPU result.\n" );
    else
        printf( "GPU result NOT EQUAL to CPU result.\n" );


//-----------------------------------------------------------------------------
// Clean up.
//-----------------------------------------------------------------------------	
    free( inputArray );
    free( CPU_outputArray );
    free( GPU_outputArray );

    char ch; scanf( "%c", &ch ); // Prevents the console window from closing immediately.
    return 0;
}
