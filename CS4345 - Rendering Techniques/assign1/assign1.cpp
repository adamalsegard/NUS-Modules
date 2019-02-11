//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : e0025203@u.nus.edu
// COMMENTS TO GRADER:
// <comments to grader, if any>
//
// ============================================================
//
// FILE: assign1.cpp


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "image_io.h"
#include "shader_util.h"
#include "trackball.h"
#include "gltext.h"
#include "Timer.h"



/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define OBJECT_RADIUS       100.0

// Material properties for all objects.
const GLfloat materialAmbient[] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat materialDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat materialSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat materialShininess[] = { 128.0 };
const GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1.0 };

// Light 0.
const GLfloat light0Ambient[] = { 0.25, 0.25, 0.25, 1.0 };
const GLfloat light0Diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light0Specular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light0Position[] = { OBJECT_RADIUS*10.0, OBJECT_RADIUS*10.0, OBJECT_RADIUS*10.0, 1.0 };

// Shaders' filenames.
const char vertShaderFilename[] = "assign1.vert";
const char fragShaderFilename[] = "assign1.frag";

// Cubemap images' filenames.
const char *cubemapImage[6] = { "images/cm2_right.png", "images/cm2_left.png", 
                                "images/cm2_top.png", "images/cm2_bottom.png", 
                                "images/cm2_back.png", "images/cm2_front.png" };

// Diffuse texture images' filenames.
const char diffuseTexImage1[] = "images/wood_bright.png";
const char diffuseTexImage2[] = "images/tweety.jpg";


/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

// Trackball object.
TrackBall tb( GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON );

// Window's size.
int winWidth = 1024;    // Window width in pixels.
int winHeight = 768;    // Window height in pixels.

// Others.
bool showAxes = true;           // Show world coordinate frame axes iff true.
bool showWireframe = false;     // Show polygons in wireframe if true, otherwise polygons are filled.
bool useShaders = true;         // Use shaders iff true.

int objectToDraw = 0;           // Select which object to draw.
int numDifferentObjects = 3;    // Number of different object to choose from.

// Shader program object.
GLuint shaderProgObj;

// Texture objects.
GLuint cubemapTexObj;
GLuint diffuseTexObj1;
GLuint diffuseTexObj2;

// Values for the generic uniform variables in the shaders.
float uniform_TileDensity = 6.0;        // (0.0, inf)
float uniform_TubeRadius = 0.10;        // (0.0, 0.5]
float uniform_StickerWidth = 0.60;      // (0.0, 1.0]

int uniform_EnvMap = 0;
int uniform_DiffuseTex1 = 1;
int uniform_DiffuseTex2 = 2;




/////////////////////////////////////////////////////////////////////////////
// A callback function called by makeShaderProgramFromFiles() for 
// assigning locations to attribute variables declared in the 
// vertex shader attached to prog.
/////////////////////////////////////////////////////////////////////////////
void MyBindAttribLocFunc( GLuint prog )
{
    glBindAttribLocation( prog, 1, "Tangent" );
    printOpenGLError();
}



/////////////////////////////////////////////////////////////////////////////
// This function is called after glUseProgram(prog) to set the
// values of the uniform variables in the shaders attached to prog.
/////////////////////////////////////////////////////////////////////////////
void MyInitUniformFunc( GLuint prog )
{
    glUniform1f( getUniLoc(prog, "TileDensity"), uniform_TileDensity );
    printOpenGLError();

    glUniform1f( getUniLoc(prog, "TubeRadius"), uniform_TubeRadius );
    printOpenGLError();

    glUniform1f( getUniLoc(prog, "StickerWidth"), uniform_StickerWidth );
    printOpenGLError();

    glUniform1i( getUniLoc(prog, "EnvMap"), uniform_EnvMap );
    printOpenGLError();

    glUniform1i( getUniLoc(prog, "DiffuseTex1"), uniform_DiffuseTex1 );
    printOpenGLError();

    glUniform1i( getUniLoc(prog, "DiffuseTex2"), uniform_DiffuseTex2 );
    printOpenGLError();
}



/////////////////////////////////////////////////////////////////////////////
// Draw the x, y, z axes. Each is drawn with the input length.
// The x-axis is red, y-axis green, and z-axis blue.
/////////////////////////////////////////////////////////////////////////////
void DrawAxes( double length )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glDisable( GL_LIGHTING );
    glLineWidth( 1.5 );
    glBegin( GL_LINES );
        // x-axis.
        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( length, 0.0, 0.0 );
        // y-axis.
        glColor3f( 0.0, 1.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( 0.0, length, 0.0 );
        // z-axis.
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, length );
    glEnd();
    glPopAttrib();
}



/////////////////////////////////////////////////////////////////////////////
// Draw a single square with the appropriate vertex normals,
// tangent vectors, and texture coordinates.
/////////////////////////////////////////////////////////////////////////////
void DrawSquare( void )
{
    glBegin( GL_QUADS );
        glNormal3f( 0.0, 0.0, 1.0 ); // Normal vector.
        glVertexAttrib3f( 1, 1.0, 0.0, 0.0 ); // Tangent vector.
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( -1.0, -1.0, 0 );
        glTexCoord2f( 1.0, 0.0 );
        glVertex3f( 1.0, -1.0, 0 );
        glTexCoord2f( 1.0, 1.0 );
        glVertex3f( 1.0, 1.0, 0 );
        glTexCoord2f( 0.0, 1.0 );
        glVertex3f( -1.0, 1.0, 0 );
    glEnd();
}



/////////////////////////////////////////////////////////////////////////////
// Draw a cube with the appropriate vertex normals,
// tangent vectors, and texture coordinates for each face.
/////////////////////////////////////////////////////////////////////////////
void DrawCube( void )
{
    glPushMatrix();
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();

    glPushMatrix();
    glRotatef( 90.0, 0.0, 1.0, 0.0 );
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();

    glPushMatrix();
    glRotatef( 180.0, 0.0, 1.0, 0.0 );
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();

    glPushMatrix();
    glRotatef( 270.0, 0.0, 1.0, 0.0 );
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();

    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();

    glPushMatrix();
    glRotatef( 270.0, 1.0, 0.0, 0.0 );
    glTranslatef( 0.0, 0.0, 1.0 );
    DrawSquare();
    glPopMatrix();
}



/////////////////////////////////////////////////////////////////////////////
// Draw a single square with the appropriate vertex normals,
// tangent vectors, and texture coordinates.
/////////////////////////////////////////////////////////////////////////////
void DrawTube( void )
{
    const double PI = 3.1415926535897932384626433832795;
    const double radius = 3.0 / PI;
    const int numSectors = 36;
    const double angleIncr = 2.0 * PI / numSectors;
    const double texCoordIncr = 3.0 / numSectors;

    glBegin( GL_QUADS );
        for ( int i = 0; i < numSectors; i++ )
        {
            double angle0 = i * angleIncr;
            double angle1 = (i+1) * angleIncr;
            double ux0 = cos( angle0 );
            double uz0 = -sin( angle0 );
            double ux1 = cos( angle1 );
            double uz1 = -sin( angle1 );

            glNormal3f( ux0, 0.0, uz0 ); // Normal vector.
            glVertexAttrib3f( 1, uz0, 0.0, -ux0 ); // Tangent vector.
            glTexCoord2f( i * texCoordIncr, 1.0 );
            glVertex3f( ux0 * radius, 1.0, uz0 * radius );  //v0
            glTexCoord2f( i * texCoordIncr, 0.0 );
            glVertex3f( ux0 * radius, -1.0, uz0 * radius );  //v1
            glNormal3f( ux1, 0.0, uz1 ); // Normal vector.
            glVertexAttrib3f( 1, uz1, 0.0, -ux1 ); // Tangent vector.
            glTexCoord2f( (i+1) * texCoordIncr, 0.0 );
            glVertex3f( ux1 * radius, -1.0, uz1 * radius );  //v2
            glTexCoord2f( (i+1) * texCoordIncr, 1.0 );
            glVertex3f( ux1 * radius, 1.0, uz1 * radius );  //v3
        }
    glEnd();
}



/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////

void MyDisplay( void )
{
    Timer t;
    t.start();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (double)winWidth/winHeight, OBJECT_RADIUS, 10.0 * OBJECT_RADIUS );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Put point light source in eye space. 
    glLightfv( GL_LIGHT0, GL_POSITION, light0Position);

    glTranslated( 0.0, 0.0, -3.0 * OBJECT_RADIUS );

    glPushMatrix();
        tb.applyTransform();

        // Draw axes.
        if ( showAxes ) DrawAxes( 2.0 * OBJECT_RADIUS );

        if ( useShaders ) 
        {
            glUseProgram( shaderProgObj );
            MyInitUniformFunc( shaderProgObj ); // Pass values to the uniform variables.
        }

        glScalef( OBJECT_RADIUS, OBJECT_RADIUS, OBJECT_RADIUS );

        // Draw object.
        switch ( objectToDraw )
        {
            case 0: DrawSquare(); break;
            case 1: DrawCube(); break;
            case 2: DrawTube(); break;
        }
        
        if ( useShaders ) glUseProgram( 0 );  // Restore to use fixed functionalities.

    glPopMatrix();

    glFinish();
    t.stop();
    glColor3f( 0.0, 1.0, 1.0 );
    gltext( 5, 5, "FPS = %.1f", 1.0/t.getElapsedTimeInSec() );

    glutSwapBuffers();
}




/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////
void MyKeyboard( unsigned char key, int x, int y )
{
    switch ( key )
    {
        // Toggle shaders.
        case 's':
        case 'S': 
            useShaders = !useShaders;
            glutPostRedisplay();
            break;

        // Change 3D object.
        case 'o':
        case 'O': 
            objectToDraw = (objectToDraw + 1) % numDifferentObjects;
            glutPostRedisplay();
            break;

        // Toggle axes.
        case 'x':
        case 'X': 
            showAxes = !showAxes;
            glutPostRedisplay();
            break;

        // Toggle between wireframe and filled polygons.
        case 'w':
        case 'W': 
            showWireframe = !showWireframe;
            if ( showWireframe ) 
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            else
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glutPostRedisplay();
            break;

        // Reset to initial view.
        case 'r':
        case 'R': 
            tb.reset();
            glutPostRedisplay();
            break;

        // Quit program.
        case 'q':
        case 'Q': 
            exit(0);
            break;
    }
}



/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
/////////////////////////////////////////////////////////////////////////////
void MyReshape( int w, int h )
{
    tb.reshape( w, h );
    winWidth = w;
    winHeight = h;
    glViewport( 0, 0, w, h );
}



/////////////////////////////////////////////////////////////////////////////
// The mouse callback function.
/////////////////////////////////////////////////////////////////////////////
static void MyMouse( int button, int state, int x, int y )
{
    tb.mouse( button, state, x, y );
    glutPostRedisplay();
}



/////////////////////////////////////////////////////////////////////////////
// The mouse motion callback function.
/////////////////////////////////////////////////////////////////////////////
static void MyMotion( int x, int y )
{
    tb.motion(x, y);
    glutPostRedisplay();
} 



/////////////////////////////////////////////////////////////////////////////
// Initialize some OpenGL states.
/////////////////////////////////////////////////////////////////////////////
void MyGLInit( void )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Set black background color.
    glEnable( GL_DEPTH_TEST ); // Use depth-buffer for hidden surface removal.
    glDisable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    glDisable(GL_CULL_FACE);

    // Set Light 0.
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0Ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0Diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0Specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHTING );

    // Set some global light properties.
    GLfloat globalAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

    // Set the material properties.
    glMaterialfv( GL_FRONT, GL_AMBIENT, materialAmbient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, materialDiffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, materialSpecular );
    glMaterialfv( GL_FRONT, GL_SHININESS, materialShininess );
    glMaterialfv( GL_FRONT, GL_EMISSION, materialEmission );

    glEnable( GL_NORMALIZE ); // Let OpenGL automatically renomarlize all normal vectors.
}



/////////////////////////////////////////////////////////////////////////////
// Set up cubemap texture images from files.
/////////////////////////////////////////////////////////////////////////////
void MySetUpCubemap( GLuint *texObj, const char *imageFilename[6] )
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, texObj );

    glBindTexture( GL_TEXTURE_CUBE_MAP, *texObj );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    //glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    unsigned char *imageData = NULL;
    int imageWidth, imageHeight, numComponents;

    for ( int i = 0; i < 6; i++ )
    {
        if ( ReadImageFile( imageFilename[i], &imageData, 
                            &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 2 );

        gluBuild2DMipmaps( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, GL_RGB, imageWidth, imageHeight,
                           GL_RGB, GL_UNSIGNED_BYTE, imageData );

        DeallocateImageData( &imageData );
    }
}



/////////////////////////////////////////////////////////////////////////////
// Set up cubemap texture images from files.
/////////////////////////////////////////////////////////////////////////////
void MySetUpTextureMap( GLuint *texObj, const char *imageFilename )
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, texObj );

    glBindTexture( GL_TEXTURE_2D, *texObj );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    unsigned char *imageData = NULL;
    int imageWidth, imageHeight, numComponents;

    if ( ReadImageFile( imageFilename, &imageData, 
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 2 );

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );
}




/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
// Initialize GLUT and create window.

    glutInit( &argc, argv );
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( winWidth, winHeight );
    glutCreateWindow( "Assign1" );


// Register the callback functions.

    glutDisplayFunc( MyDisplay ); 
    glutReshapeFunc( MyReshape );
    glutKeyboardFunc( MyKeyboard );
    glutMouseFunc( MyMouse );
    glutMotionFunc( MyMotion );


// Initialize GLEW.

    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        fprintf( stderr, "Error: %s.\n", glewGetErrorString( err ) );
        exit( 1 );
    }
    printf( "Status: Using GLEW %s.\n", glewGetString( GLEW_VERSION ) );

    if ( !GLEW_VERSION_2_0 )
    {
        fprintf( stderr, "Error: OpenGL 2.0 is not supported.\n" );
        exit( 1 );
    }


// Some OpenGL initializations.

    MyGLInit();


// Set up cubemap texture images from files.
    glActiveTexture( GL_TEXTURE0 );
    MySetUpCubemap( &cubemapTexObj, cubemapImage );

// Set up diffuse texture map from file.
    glActiveTexture( GL_TEXTURE1 );
    MySetUpTextureMap( &diffuseTexObj1, diffuseTexImage1 );

// Set up diffuse texture map from file.
    glActiveTexture( GL_TEXTURE2 );
    MySetUpTextureMap( &diffuseTexObj2, diffuseTexImage2 );


// Create shader program object.

    shaderProgObj = makeShaderProgramFromFiles( vertShaderFilename, fragShaderFilename, 
                                                MyBindAttribLocFunc );
    if ( shaderProgObj == 0 )
    {
        fprintf( stderr, "Error: Cannot create shader program object.\n" );
        fflush( stdin ); getchar(); // Prevents the console window from closing.
        exit( 1 );
    }


// Display user instructions in console window.

    printf( "Press 'S' to toggle shaders.\n" );
    printf( "Press 'O' to change 3D object.\n" );
    printf( "Press 'X' to toggle axes.\n" );
    printf( "Press 'W' to toggle wireframe.\n" );
    printf( "Press 'R' to reset to initial view.\n" );
    printf( "Press 'Q' to quit.\n\n" );


// Enter GLUT event loop.

    glutMainLoop();
    return 0;
}
