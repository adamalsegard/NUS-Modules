//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : E0025203@u.nus.edu
// COMMENTS TO GRADER:
//
// - GL_TEXTURE_COMPARE_FAIL_VALUE_ARB doesn't work in Media Lab
//
// ============================================================
//
// FILE: assign1.cpp


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "image_io.h"


/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define PI                  3.1415926535897932384626433832795

#define SCENE_RADIUS        6.0     // Mainly for setting far clipping plane distance.

// The room has a square floor, which is centered at the world-space origin.
// The z-axis is pointing up.

#define ROOM_WIDTH          6.0
#define ROOM_HEIGHT         4.0

// The reflective tabletop is a rectangle that is always parallel to the x-y plane.
// The sides of the tabletop are always parallel to the x-axis or y-axis.

#define TABLETOP_X1         -1.0
#define TABLETOP_X2         1.0
#define TABLETOP_Y1         -1.5
#define TABLETOP_Y2         1.5
#define TABLETOP_Z          1.2     // This is the z coordinate of the top-most face of the table.

#define TABLE_THICKNESS     0.1

// The followings are for navigation and setting the view of the (actual) eye.

#define LOOKAT_X            0.0     // Look-at point x coordinate.
#define LOOKAT_Y            0.0     // Look-at point y coordinate.
#define LOOKAT_Z            1.0     // Look-at point z coordinate.

#define EYE_INIT_DIST       5.0     // Initial distance of eye from look-at point.
#define EYE_DIST_INCR       0.2     // Distance increment when changing eye's distance.
#define EYE_MIN_DIST        0.1     // Min eye's distance from look-at point.

#define EYE_MIN_LATITUDE    -88.0   // Min eye's latitude (in degrees) w.r.t. look-at point.
#define EYE_MAX_LATITUDE    88.0    // Max eye's latitude (in degrees) w.r.t. look-at point.
#define EYE_LATITUDE_INCR   2.0     // Degree increment when changing eye's latitude.
#define EYE_LONGITUDE_INCR  2.0     // Degree increment when changing eye's longitude.

// Bonus task
#define SHADOW_MAP_WIDTH	800
#define SHADOW_MAP_HEIGHT	800

// Light 0.
const GLfloat light0Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
const GLfloat light0Diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light0Specular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light0Position[] = { 10.0, -5.0, 8.0, 1.0 };

// Variables for Shadow map camera //TODO: set variables
const GLdouble lightFovy = 53.0;
const GLdouble lightAspect = SHADOW_MAP_WIDTH / SHADOW_MAP_HEIGHT;
const GLdouble lightNear = 5;
const GLdouble lightFar = 20;
GLint viewport[4];

// Light 1.
const GLfloat light1Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
const GLfloat light1Diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light1Specular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light1Position[] = { -2.0, 10.0, -2.0, 1.0 };


// Texture image filenames.
const char woodTexFile[] = "images/wood.jpg";
const char ceilingTexFile[] = "images/ceiling.jpg";
const char brickTexFile[] = "images/brick.jpg";
const char checkerTexFile[] = "images/checker.png";
const char spotsTexFile[] = "images/spots.png";

const char sandTexFile[] = "images/sand.tga";
const char stoneTexFile[] = "images/stone.tga";
const char containerTexFile[] = "images/container.jpg";



/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

// Window's size.
int winWidth = 800;     // Window width in pixels.
int winHeight = 600;    // Window height in pixels.


// Define (actual) eye position.
// Initial eye position is at [EYE_INIT_DIST, 0, 0] + [LOOKAT_X, LOOKAT_Y, LOOKAT_Z]
// in the world space, looking at [LOOKAT_X, LOOKAT_Y, LOOKAT_Z]. 
// The up-vector is always [0, 0, 1].

double eyeLatitude = 0.0;
double eyeLongitude = 0.0;
double eyeDistance = EYE_INIT_DIST;

// The actual eye position in world space.
// This is computed from eyeLatitude, eyeLongitude, eyeDistance, and the look-at point.

double eyePos[3];

// Texture objects.
GLuint reflectionTexObj;
GLuint woodTexObj;
GLuint ceilingTexObj;
GLuint brickTexObj;
GLuint checkerTexObj;
GLuint spotsTexObj;
GLuint containerTexObj;
GLuint shadowTexObj;
GLuint grayTexObj;
GLuint sceneTexObj;

// Others.
bool drawAxes = true;           // Draw world coordinate frame axes if true.
bool drawWireframe = false;     // Draw polygons in wireframe if true, otherwise polygons are filled.
bool hasTexture = true;         // Toggle texture mapping.


// Forward function declarations.
void DrawAxes( double length );
void DrawRoom( float blend );
void DrawTeapot( void );
void DrawSphere( void );
void DrawTable( void );

void drawFullScene(void);

// Task 2 functions
void DrawCube(void);
void drawBoxWithTexCoords(GLfloat size);


/////////////////////////////////////////////////////////////////////////////
// Render the scene from the imaginary viewpoint and capture the image as 
// a texture map.
//
// This texture map is then used to texture map the tabletop rectangle
// to simulate the reflection of the scene from the tabletop.
/////////////////////////////////////////////////////////////////////////////

void MakeReflectionImage( void )
{
    //********************************************************
    //*********** TASK #1: WRITE YOUR CODE BELOW *************
    //********************************************************
    // This function does the followings:
    // STEP 1: Clears the correct buffers.
    // STEP 2: Sets up the correct view volume for the imaginary viewpoint.
    // STEP 3: Sets up the imaginary viewpoint.
    // STEP 4: Sets up the light source positions in the world space.
    // STEP 5: Draws the scene (may not need to draw all objects).
    // STEP 6: Read the correct color buffer into the correct texture object.
    //********************************************************

	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Set up refl. view volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(TABLETOP_Y1 - eyePos[1], TABLETOP_Y2 - eyePos[1], TABLETOP_X1-eyePos[0], TABLETOP_X2-eyePos[0], eyePos[2]-TABLETOP_Z, eyePos[2]+ROOM_HEIGHT);
	
	// Set up refl. viewpoint
	double reflPos[3];
	reflPos[0] = eyePos[0];
	reflPos[1] = eyePos[1];
	reflPos[2] = -(eyePos[2] - 2*TABLETOP_Z); // Equal length from reflective plane in negative direction

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(reflPos[0], reflPos[1], reflPos[2], eyePos[0], eyePos[1], eyePos[2], 1.0, 0.0, 0.0);
	// z_cam = n = norm(eye - at) => norm(0, 0, 2*TABLETOP_Z - 2*eyePos[2]) = -z
	// x_cam = u = normalize(up) x n => (x x -z) = y
	// y_cam = v = n x u => (-z x y) = x
	// This corresponds to how the table is set up

	// Set up light sources positions (world-space)
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Position);

	glDrawBuffer(GL_BACK);

	// Draw objects
	DrawRoom(1.0f);
	DrawTeapot();
	DrawSphere();
	DrawCube(); // Task 2
	// No need to draw table

	// Using framebuffer Data as Texture image
	glReadBuffer(GL_BACK); // Read from second framebuffer
	glBindTexture(GL_TEXTURE_2D, reflectionTexObj); // Bind object to copy texture to
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, winWidth, winHeight, 0); // Copy image to framebuffer
}


/////////////////////////////////////////////////////////////////////////////
// BONUS TASK: Generate Shadow map
/////////////////////////////////////////////////////////////////////////////

void genShadowMap(void) {

	// Store original viewport values
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Set new viewport size
	glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

	// Clear depth buffer (TEXTURE0)
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set up shadow map camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(lightFovy, lightAspect, lightNear, lightFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(light0Position[0], light0Position[1], light0Position[2], LOOKAT_X, LOOKAT_Y, ROOM_HEIGHT/2, 0.0, 0.0, 1.0);

	glCullFace(GL_FRONT);
	glDisable(GL_LIGHTING); // We don't need any light for depth
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, 0); // Don't render textures

	glDrawBuffer(GL_BACK);

	// Draw objects
	drawFullScene();

	glReadBuffer(GL_BACK); 

	glActiveTexture(GL_TEXTURE1); // Store in TEXTURE1
	glBindTexture(GL_TEXTURE_2D, shadowTexObj); // Bind object to copy texture to - still bound so don't really need?
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0); // Copy image to framebuffer

	// Restore viewport
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	// Restore states
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
}

// Create transformation matrix from eye space to light space
void genTextureMatrix(void) {

	GLfloat tmpMatrix[16];
	glMatrixMode(GL_MODELVIEW); // Already by default, not needed

	// "Think backwards"
	glPushMatrix();
		glLoadIdentity();

		// Transform [-1, 1] to [0, 1], will be performed as Scale -> translate
		glTranslatef(0.5, 0.5, 0.5); // We also need to transform R value!
		glScalef(0.5, 0.5, 0.5);

		gluPerspective(lightFovy, lightAspect, lightNear, lightFar);
		gluLookAt(light0Position[0], light0Position[1], light0Position[2], LOOKAT_X, LOOKAT_Y, ROOM_HEIGHT / 2, 0.0, 0.0, 1.0);
		// TextureMatrix = Trans*Scale*lightProj*lightView*Identity;
		// Stores the top-most matrix on MV-stack, aka these transformations!
		glGetFloatv(GL_MODELVIEW_MATRIX, tmpMatrix);
	glPopMatrix();

	// Transpose matrix, built-in function doesn't work
	GLfloat transpose[16];
	for (int i = 0; i < 16; i++) {
		transpose[i] = tmpMatrix[(i%4)*4+(i/4)];
		//printf("Old value [%i]: %f \n", i, tmpMatrix[i]);
		//printf("New value [%i]: %f \n", i, transpose[i]);
	}
	//glLoadTransposeMatrixf(tmpMatrix);

	// Generate texture coordinates for shadow map (only for Tex1)
	glActiveTexture(GL_TEXTURE1);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, &transpose[0]);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, &transpose[4]);
	glTexGenfv(GL_R, GL_OBJECT_PLANE, &transpose[8]);
	glTexGenfv(GL_Q, GL_OBJECT_PLANE, &transpose[12]);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

// Try for gray shadows...
// Render gray texture("shadowed regions")
void genGrayImage(void)
{
	glDrawBuffer(GL_BACK);

	// Draw objects
	DrawRoom(0.3f);
	DrawTeapot();
	DrawSphere();
	DrawCube();
	DrawTable();

	glReadBuffer(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, grayTexObj);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, winWidth, winHeight, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

// Render original scene
void renderScene(void)
{
	glDrawBuffer(GL_BACK);

	// Draw objects
	DrawRoom(1.0f);
	DrawTeapot();
	DrawSphere();
	DrawCube(); // Task 2
	DrawTable();

	glReadBuffer(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, sceneTexObj);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, winWidth, winHeight, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////

void MyDisplay( void )
{
	// Loop always start in Texture0
    if ( hasTexture )
        glEnable( GL_TEXTURE_2D );
    else
        glDisable( GL_TEXTURE_2D );

    // Compute world-space eye position from eyeLatitude, eyeLongitude, eyeDistance, and look-at point.
    eyePos[2] = eyeDistance * sin( eyeLatitude * PI / 180.0 ) + LOOKAT_Z;
    double xy = eyeDistance * cos( eyeLatitude * PI / 180.0 );
    eyePos[0] = xy * cos( eyeLongitude * PI / 180.0 ) + LOOKAT_X;
    eyePos[1] = xy * sin( eyeLongitude * PI / 180.0 ) + LOOKAT_Y;

	// Create reflection image
	MakeReflectionImage();

	// Generate shadow map
	genShadowMap();
	genTextureMatrix();
	
	// Clear framebuffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Define camera
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (double)winWidth/winHeight, EYE_MIN_DIST, eyeDistance + SCENE_RADIUS );
	
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( eyePos[0], eyePos[1], eyePos[2], LOOKAT_X, LOOKAT_Y, LOOKAT_Z, 0.0, 0.0, 1.0 );
	
    // Set world-space positions of the two lights.
    glLightfv( GL_LIGHT0, GL_POSITION, light0Position );
    glLightfv( GL_LIGHT1, GL_POSITION, light1Position );

	// Draw axes.
	if (drawAxes) DrawAxes(SCENE_RADIUS);

	// Switch back to Tex0 (changed in genShadowMap() and genTextureMatrix())
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Draw scene.
	DrawSphere();
	DrawTeapot();
	DrawCube();
	DrawTable();
	DrawRoom(1.0f);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);

    glutSwapBuffers();
}




/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////

void MyKeyboard( unsigned char key, int x, int y )
{
    switch ( key )
    {
        // Quit program.
        case 'q':
        case 'Q': 
            exit(0);
            break;

        // Toggle between wireframe and filled polygons.
        case 'w':
        case 'W': 
            drawWireframe = !drawWireframe;
            if ( drawWireframe ) 
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            else
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glutPostRedisplay();
            break;

        // Toggle axes.
        case 'x':
        case 'X': 
            drawAxes = !drawAxes;
            glutPostRedisplay();
            break;

        // Toggle texture mapping.
        case 't':
        case 'T': 
            hasTexture = !hasTexture;
            glutPostRedisplay();
            break;

       // Reset to initial view.
        case 'r':
        case 'R': 
            eyeLatitude = 0.0;
            eyeLongitude = 0.0;
            eyeDistance = EYE_INIT_DIST;
            glutPostRedisplay();
            break;
    }
}




/////////////////////////////////////////////////////////////////////////////
// The special key callback function.
/////////////////////////////////////////////////////////////////////////////

void MySpecialKey( int key, int x, int y )
{
    int modi = glutGetModifiers();

    switch ( key )
    {
        case GLUT_KEY_LEFT:
            eyeLongitude -= EYE_LONGITUDE_INCR;
            if ( eyeLongitude < -360.0 ) eyeLongitude += 360.0 ;
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT: 
            eyeLongitude += EYE_LONGITUDE_INCR;
            if ( eyeLongitude > 360.0 ) eyeLongitude -= 360.0 ;
            glutPostRedisplay();
            break;

        case GLUT_KEY_UP:
            if ( modi != GLUT_ACTIVE_SHIFT )
            {
                eyeLatitude += EYE_LATITUDE_INCR;
                if ( eyeLatitude > EYE_MAX_LATITUDE ) eyeLatitude = EYE_MAX_LATITUDE;
            }
            else
            {
                eyeDistance -= EYE_DIST_INCR;
                if ( eyeDistance < EYE_MIN_DIST ) eyeDistance = EYE_MIN_DIST;
            }
            glutPostRedisplay();
            break;  
        
        case GLUT_KEY_DOWN:
            if ( modi != GLUT_ACTIVE_SHIFT )
            {
                eyeLatitude -= EYE_LATITUDE_INCR;
                if ( eyeLatitude < EYE_MIN_LATITUDE ) eyeLatitude = EYE_MIN_LATITUDE;
            }
            else
            {
                eyeDistance += EYE_DIST_INCR;
            }
            glutPostRedisplay();
            break;
    }
}




/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
/////////////////////////////////////////////////////////////////////////////

void MyReshape( int w, int h )
{
    winWidth = w;
    winHeight = h;
    glViewport( 0, 0, w, h );
}




/////////////////////////////////////////////////////////////////////////////
// Initialize some OpenGL states.
/////////////////////////////////////////////////////////////////////////////

void GLInit( void )
{
	glActiveTexture(GL_TEXTURE0); // Just to be overly clear, not needed really
	glEnable(GL_TEXTURE_2D);

    glClearColor( 0.0, 0.0, 0.0, 1.0 ); // Set black background color.

    glShadeModel( GL_SMOOTH ); // Enable Gouraud shading.
    glEnable( GL_DEPTH_TEST ); // Use depth-buffer for hidden surface removal.
    glEnable( GL_CULL_FACE );  // Enable back-face culling.

    glDisable( GL_DITHER );
    glDisable( GL_BLEND );

    // Set Light 0.
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0Ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0Diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0Specular );
    glEnable( GL_LIGHT0 );

    // Set Light 1.
    glLightfv( GL_LIGHT1, GL_AMBIENT, light1Ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light1Diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, light1Specular );
    glEnable( GL_LIGHT1 );

    glEnable( GL_LIGHTING );

    // Set some global light properties.
    GLfloat globalAmbient[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, globalAmbient );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

    // Set initial material properties.
    GLfloat initMaterialAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat initMaterialDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat initMaterialSpecular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat initMaterialShininess[] = { 16.0 };
    GLfloat initMaterialEmission[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, initMaterialAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, initMaterialDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, initMaterialSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, initMaterialShininess );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, initMaterialEmission );

    // Let OpenGL automatically renomarlize all normal vectors.
    // This is important if objects are to be scaled.
    glEnable( GL_NORMALIZE ); 
}




/////////////////////////////////////////////////////////////////////////////
// Set up texture maps.
/////////////////////////////////////////////////////////////////////////////

void SetUpTextureMaps( void )
{

    unsigned char *imageData = NULL;
    int imageWidth, imageHeight, numComponents;

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );


// This texture object is for the wood texture map.

    glGenTextures( 1, &woodTexObj );
    glBindTexture( GL_TEXTURE_2D, woodTexObj );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    if ( ReadImageFile( woodTexFile, &imageData, 
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 1 );
    if ( numComponents != 3 )
    {
        fprintf( stderr, "Error: Texture image is not in RGB format.\n" );
        exit( 1 );
    }

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );


// This texture object is for the ceiling texture map.

    glGenTextures( 1, &ceilingTexObj );
    glBindTexture( GL_TEXTURE_2D, ceilingTexObj );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    if ( ReadImageFile( ceilingTexFile, &imageData, 
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 1 );
    if ( numComponents != 3 )
    {
        fprintf( stderr, "Error: Texture image is not in RGB format.\n" );
        exit( 1 );
    }

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );


// This texture object is for the brick texture map.

    glGenTextures( 1, &brickTexObj );
    glBindTexture( GL_TEXTURE_2D, brickTexObj );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    if ( ReadImageFile( brickTexFile, &imageData, 
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 1 );
    if ( numComponents != 3 )
    {
        fprintf( stderr, "Error: Texture image is not in RGB format.\n" );
        exit( 1 );
    }

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );


// This texture object is for the checkered texture map.

    glGenTextures( 1, &checkerTexObj );
    glBindTexture( GL_TEXTURE_2D, checkerTexObj );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    if ( ReadImageFile( checkerTexFile, &imageData, 
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 1 );
    if ( numComponents != 3 )
    {
        fprintf( stderr, "Error: Texture image is not in RGB format.\n" );
        exit( 1 );
    }

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );


// This texture object is for the spots texture map.

    glGenTextures( 1, &spotsTexObj);
    glBindTexture( GL_TEXTURE_2D, spotsTexObj);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    if ( ReadImageFile(spotsTexFile, &imageData,
                        &imageWidth, &imageHeight, &numComponents ) == 0 ) exit( 1 );
    if ( numComponents != 3 )
    {
        fprintf( stderr, "Error: Texture image is not in RGB format.\n" );
        exit( 1 );
    }

    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight,
                       GL_RGB, GL_UNSIGNED_BYTE, imageData );

    DeallocateImageData( &imageData );



	/**** TASK 2 ****/
// This texture object is for the container texture map.

	glGenTextures(1, &containerTexObj);
	glBindTexture(GL_TEXTURE_2D, containerTexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // Deprecated in Core >3.0 but still works for Legacy OpenGL

	if (ReadImageFile(containerTexFile, &imageData, &imageWidth, &imageHeight, &numComponents) == 0) exit(1);

	if (numComponents != 3)
	{
		fprintf(stderr, "Error: Texture image is not in RGB format.\n");
		exit(1);
	}

	// Let me quote the Khronos group: "Never use this" https://www.khronos.org/opengl/wiki/Common_Mistakes#Automatic_mipmap_generation
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	// So instead I use GL_GENERATE_MIPMAP and this for automatically generating mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	DeallocateImageData(&imageData);


// This texture object is for storing the reflection image read from the color buffer.

    //********************************************************
    //*********** TASK #1: WRITE YOUR CODE BELOW *************
    //********************************************************
    // Sets up the texture object reflectionTexObj for storing the reflection 
    // texture map that is to be mapped to the tabletop.
    // You must make sure that mipmapping is used.
    // You must also make sure that whenever a new image is copied to the texture object,
    // all other lower-resolution mipmap levels are automatically generated.
    //********************************************************

	glGenTextures(1, &reflectionTexObj);
	glBindTexture(GL_TEXTURE_2D, reflectionTexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Will automatically generate mipmaps every time mipmap lvl 0 of object is updated
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	

	//********************************************************
	//*********** BONUS TASK *********************************
	//********************************************************
// This texture object is for storing the shadow map texture read from the depth buffer.
	
	// Change Active Texture
	glActiveTexture(GL_TEXTURE1); 
	glEnable(GL_TEXTURE_2D);

	// Generate shadow texture
	glGenTextures(1, &shadowTexObj);
	glBindTexture(GL_TEXTURE_2D, shadowTexObj); // Bind shadowtexture to texture 1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set shadow mapping functions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE); // Compare depth value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL); // Less or equal is not in shadow
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE); // Alt: GL_INTENSITY, GL_LUMINANCE 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.3f); // Set fail value -> doesn't work!

	// Check if functions works
	GLfloat value = 0.0f;
	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, &value);
	printf("Fail value: %f \n", value); 
	GLint test;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, &test);
	if(test == GL_COMPARE_R_TO_TEXTURE)
		printf("GL_TEXTURE_COMPARE_FAIL_VALUE_ARB works! \n");
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, &test);
	if (test == GL_LEQUAL)
		printf("GL_TEXTURE_COMPARE_FUNC works! \n");
	glGetTexParameteriv(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, &test);
	if (test == GL_LUMINANCE)
		printf("GL_DEPTH_TEXTURE_MODE works! \n");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// Enable automatic generated texture coordinates (So I don't have to call glMultiTexCoord*() )
	// Only affacts active texture 1
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	

	// Switch back to Texture0
	glActiveTexture(GL_TEXTURE0);
	
	// Generate texture for rendered shadowed regions
	glGenTextures(1, &grayTexObj);
	glBindTexture(GL_TEXTURE_2D, grayTexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Generate texture for rendered original scene 
	glGenTextures(1, &sceneTexObj);
	glBindTexture(GL_TEXTURE_2D, sceneTexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	// Unbind textures
	glBindTexture(GL_TEXTURE_2D, 0);
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
    glutSpecialFunc( MySpecialKey );


// Initialize GLEW.
// The followings make sure OpenGL 1.4 is supported and set up the extensions.

    GLenum err = glewInit();
    if ( err != GLEW_OK )
    {
        fprintf( stderr, "Error: %s.\n", glewGetErrorString( err ) );
        exit( 1 );
    }
    printf( "Status: Using GLEW %s.\n\n", glewGetString( GLEW_VERSION ) );

    if ( !GLEW_VERSION_1_4 )
    {
        fprintf( stderr, "Error: OpenGL 1.4 is not supported.\n" );
        exit( 1 );
    }
	
	//Check for necessary extensions
	if (!GL_ARB_depth_texture || !GL_ARB_shadow || !GL_ARB_shadow_ambient)
	{
		printf("Required extensions not available! \n");
		return false;
	}
	if ( glewIsExtensionSupported("GL_ARB_shadow_ambient")) {
		printf("Something wrong with GL_ARB_shadow_ambient \n");
	}
	if (glewIsExtensionSupported("GL_ARB_shadow")) {
		printf("Something wrong with GL_ARB_shadow \n");
	}
	if (glewIsExtensionSupported("GL_ARB_depth_texture")) {
		printf("Something wrong with GL_ARB_depth_texture \n");
	}
	
	int MaxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxTextureUnits);
	printf("MaxTextureUnits: %i \n", MaxTextureUnits);

// Setup the initial render context.

    GLInit();
    SetUpTextureMaps();


// Display user instructions in console window.

    printf( "Press LEFT to move eye left.\n" );
    printf( "Press RIGHT to move eye right.\n" );
    printf( "Press UP to move eye up.\n" );
    printf( "Press DOWN to move eye down.\n" );
    printf( "Press SHIFT+UP to move closer.\n" );
    printf( "Press SHIFT+DOWN to move further.\n" );
    printf( "Press 'W' to toggle wireframe.\n" );
    printf( "Press 'T' to toggle texture mapping.\n" );
    printf( "Press 'X' to toggle axes.\n" );
    printf( "Press 'R' to reset to initial view.\n" );
    printf( "Press 'Q' to quit.\n\n" );


// Enter GLUT event loop.

    glutMainLoop();
    return 0;
}




//============================================================================
//============================================================================
// Functions below are for modeling the 3D objects.
//============================================================================
//============================================================================


/////////////////////////////////////////////////////////////////////////////
// Draw the x, y, z axes. Each is drawn with the input length.
// The x-axis is red, y-axis green, and z-axis blue.
/////////////////////////////////////////////////////////////////////////////

void DrawAxes( double length )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glLineWidth( 3.0 );
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
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
    glPopAttrib();

	
}




/////////////////////////////////////////////////////////////////////////////
// Subdivide input quad into uSteps x vSteps smaller quads, and draw them.
// The first vertex of the input quad has texture coordinates (s0, t0) and
// vertex position (x0, y0, z0), and so on.
//
// The vertices of the input quad should be given in anti-clockwise order.
//
// The texture coordinates at the input vertices are bilinearly
// interpolated to the newly created vertices.
/////////////////////////////////////////////////////////////////////////////

void SubdivideAndDrawQuad( int uSteps, int vSteps,
                           float s0, float t0, float x0, float y0, float z0,
                           float s1, float t1, float x1, float y1, float z1,
                           float s2, float t2, float x2, float y2, float z2,
                           float s3, float t3, float x3, float y3, float z3 )
{
    float tc0[3] = { s0, t0, 0.0 };  float v0[3] = { x0, y0, z0 };
    float tc1[3] = { s1, t1, 0.0 };  float v1[3] = { x1, y1, z1 };
    float tc2[3] = { s2, t2, 0.0 };  float v2[3] = { x2, y2, z2 };
    float tc3[3] = { s3, t3, 0.0 };  float v3[3] = { x3, y3, z3 };

    glBegin( GL_QUADS );

    for ( int u = 0; u < uSteps; u++ )
    {
        float uu = (float) u / uSteps;
        float uu1 = (float) (u + 1) / uSteps;
        float Atc[3], Btc[3], Ctc[3], Dtc[3];
        float Av[3], Bv[3], Cv[3], Dv[3];

        for ( int i = 0; i < 3; i++ )
        {
            Atc[i] = tc0[i] + uu  * ( tc1[i] - tc0[i] );
            Btc[i] = tc3[i] + uu  * ( tc2[i] - tc3[i] );
            Ctc[i] = tc0[i] + uu1 * ( tc1[i] - tc0[i] );
            Dtc[i] = tc3[i] + uu1 * ( tc2[i] - tc3[i] );
            Av[i] = v0[i] + uu  * ( v1[i] - v0[i] );
            Bv[i] = v3[i] + uu  * ( v2[i] - v3[i] );
            Cv[i] = v0[i] + uu1 * ( v1[i] - v0[i] );
            Dv[i] = v3[i] + uu1 * ( v2[i] - v3[i] );
        }

        for ( int v = 0; v < vSteps; v++ )
        {
            float vv = (float) v / vSteps;
            float vv1 = (float) (v + 1) / vSteps;
            float Etc[3], Ftc[3], Gtc[3], Htc[3];
            float Ev[3], Fv[3], Gv[3], Hv[3];

            for ( int i = 0; i < 3; i++ )
            {
                Etc[i] = Atc[i] + vv  * ( Btc[i] - Atc[i] );
                Ftc[i] = Ctc[i] + vv  * ( Dtc[i] - Ctc[i] );
                Gtc[i] = Atc[i] + vv1 * ( Btc[i] - Atc[i] );
                Htc[i] = Ctc[i] + vv1 * ( Dtc[i] - Ctc[i] );
                Ev[i] = Av[i] + vv  * ( Bv[i] - Av[i] );
                Fv[i] = Cv[i] + vv  * ( Dv[i] - Cv[i] );
                Gv[i] = Av[i] + vv1 * ( Bv[i] - Av[i] );
                Hv[i] = Cv[i] + vv1 * ( Dv[i] - Cv[i] );
            }

            glTexCoord2fv( Etc );  glVertex3fv( Ev );
            glTexCoord2fv( Ftc );  glVertex3fv( Fv );
            glTexCoord2fv( Htc );  glVertex3fv( Hv );
            glTexCoord2fv( Gtc );  glVertex3fv( Gv );
        }
    }

    glEnd();

}





// Draw full scene without materials or textures
void drawFullScene(void) {
	
	// Teapot
	double size = 0.45;
	glPushMatrix();
		glTranslated(-0.3, -0.5, size * 0.75 + TABLETOP_Z);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90.0, 1.0, 0.0, 0.0);
		glutSolidTeapot(size); // This function also generates texture coordinates on the teapot.
	glPopMatrix();

	// Sphere
	double radius = 0.35;
	glPushMatrix();
	glTranslated(0.3, 0.5, radius + TABLETOP_Z);
	glutSolidSphere(radius, 32, 16);
	glPopMatrix();
	
	// Cube 
	double boxsize = 0.40;
	glPushMatrix();
		glTranslated(0.5, -0.5, 3 * size + TABLETOP_Z);
		glRotated(45.0, 0.0, 1.0, 0.0); // Put some spinn to it!
		glRotated(45.0, 1.0, 0.0, 0.0);
		drawBoxWithTexCoords(boxsize);
	glPopMatrix();

	// Table
	// Draw tabletop (counter-clockwise order!)
	glNormal3f(0.0, 0.0, 1.0); // Normal vector.
	SubdivideAndDrawQuad(48, 48, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z,
		0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z,
		1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z,
		1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z);

	// Sides.
	// In +y direction.
	glNormal3f(0.0, 1.0, 0.0); // Normal vector.
	SubdivideAndDrawQuad(24, 2, 0.0, 0.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 1.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z,
		0.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z);
	// In -y direction.
	glNormal3f(0.0, -1.0, 0.0); // Normal vector.
	SubdivideAndDrawQuad(24, 2, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 0.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z,
		0.0, 1.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z);
	// In +x direction.
	glNormal3f(1.0, 0.0, 0.0); // Normal vector.
	SubdivideAndDrawQuad(24, 2, 0.0, 0.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 0.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z,
		0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z);
	// In -x direction.
	glNormal3f(-1.0, 0.0, 0.0); // Normal vector.
	SubdivideAndDrawQuad(24, 2, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 1.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z,
		0.0, 1.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z);

	// Bottom.
	glNormal3f(0.0, 0.0, -1.0); // Normal vector.
	SubdivideAndDrawQuad(24, 24, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS,
		0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS);

	// Legs.
	glPushMatrix();
		glTranslated(TABLETOP_X1 + TABLE_THICKNESS, TABLETOP_Y1 + TABLE_THICKNESS, 0.0);
		glScaled(TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS);
		glTranslated(0.0, 0.0, 0.5);
		glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
		glTranslated(TABLETOP_X2 - TABLE_THICKNESS, TABLETOP_Y1 + TABLE_THICKNESS, 0.0);
		glScaled(TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS);
		glTranslated(0.0, 0.0, 0.5);
		glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
		glTranslated(TABLETOP_X2 - TABLE_THICKNESS, TABLETOP_Y2 - TABLE_THICKNESS, 0.0);
		glScaled(TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS);
		glTranslated(0.0, 0.0, 0.5);
		glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
		glTranslated(TABLETOP_X1 + TABLE_THICKNESS, TABLETOP_Y2 - TABLE_THICKNESS, 0.0);
		glScaled(TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS);
		glTranslated(0.0, 0.0, 0.5);
		glutSolidCube(1.0);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// Draw the room. 
// The walls, ceiling and floor are all texture-mapped.
/////////////////////////////////////////////////////////////////////////////
void DrawRoom( float blend )
{
    const float ROOM_HALF_WIDTH = ROOM_WIDTH / 2.0f;

// Ceiling.
    GLfloat matAmbient1[] = { 0.7*blend, 0.7*blend, 0.7*blend, 1.0 };
    GLfloat matDiffuse1[] = { 0.7*blend, 0.7*blend, 0.7*blend, 1.0 };
    GLfloat matSpecular1[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat matShininess1[] = { 32.0 };
	if (blend < 0.9f) {
		for (int i = 0; i<4; i++)
			matSpecular1[i] = 0.0;
		matShininess1[0] = 0.0;
	}

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient1);
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse1);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular1);
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess1);
	
    glBindTexture( GL_TEXTURE_2D, ceilingTexObj );

    glNormal3f( 0.0, 0.0, -1.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 24, 0.0, 0.0, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  ROOM_WIDTH, 0.0, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  ROOM_WIDTH, ROOM_WIDTH, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  0.0, ROOM_WIDTH, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT );

// Walls.

    glBindTexture( GL_TEXTURE_2D, brickTexObj );

    // In +y direction.
    glNormal3f( 0.0, -1.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 16, 0.0, 0.0, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, 0.0, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, ROOM_HEIGHT/2, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  0.0, ROOM_HEIGHT/2, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT );
    // In -y direction.
    glNormal3f( 0.0, 1.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 16, 0.0, 0.0, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, 0.0, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, ROOM_HEIGHT/2, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  0.0, ROOM_HEIGHT/2, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT );
    // In +x direction.
    glNormal3f( -1.0, 0.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 16, 0.0, 0.0, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, 0.0, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, ROOM_HEIGHT/2, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  0.0, ROOM_HEIGHT/2, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT );
    // In -x direction.
    glNormal3f( 1.0, 0.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 16, 0.0, 0.0, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, 0.0, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH/2, ROOM_HEIGHT/2, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, ROOM_HEIGHT, 
                                  0.0, ROOM_HEIGHT/2, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, ROOM_HEIGHT );

// Floor.

    GLfloat matAmbient2[] = { 0.6*blend, 0.6*blend, 0.6*blend, 1.0 };
    GLfloat matDiffuse2[] = { 0.6*blend, 0.6*blend, 0.6*blend, 1.0 };
    GLfloat matSpecular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat matShininess2[] = { 128.0 };
	if (blend < 0.9f) {
		for(int i = 0; i<4; i++)
			matSpecular2[i] = 0.0;
		matShininess2[0] = 0.0;
	}

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess2 );


    glBindTexture( GL_TEXTURE_2D, checkerTexObj);
    
	glNormal3f( 0.0, 0.0, 1.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 24, 0.0, 0.0, ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH, 0.0, ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  ROOM_WIDTH, ROOM_WIDTH, -ROOM_HALF_WIDTH, ROOM_HALF_WIDTH, 0.0, 
                                  0.0, ROOM_WIDTH, -ROOM_HALF_WIDTH, -ROOM_HALF_WIDTH, 0.0 );


}




/////////////////////////////////////////////////////////////////////////////
// Draw a texture-mapped teapot.
/////////////////////////////////////////////////////////////////////////////

void DrawTeapot( void )
{
    double size = 0.45;

    GLfloat matAmbient[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat matDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat matShininess[] = { 128.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess );

    glBindTexture( GL_TEXTURE_2D, spotsTexObj );

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);
	
    glFrontFace( GL_CW ); // Need to do this because the built-in teapot is modelled using clockwise polygon winding.
    glDisable( GL_CULL_FACE );  // Disable back-face culling.

    glPushMatrix();
		glTranslated( -0.3, -0.5, size * 0.75 + TABLETOP_Z );
		glRotated( 90.0, 0.0, 0.0, 1.0 );
		glRotated( 90.0, 1.0, 0.0, 0.0 );
		glutSolidTeapot( size ); // This function also generates texture coordinates on the teapot.
    glPopMatrix();

    glEnable( GL_CULL_FACE );	// Enable back-face culling.
    glFrontFace( GL_CCW );		// Go back to counter-clockwise polygon winding.

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);
}




/////////////////////////////////////////////////////////////////////////////
// Draw a non-texture-mapped sphere.
/////////////////////////////////////////////////////////////////////////////

void DrawSphere( void )
{
    double radius = 0.35;

    GLfloat matAmbient[] = { 0.7, 0.5, 0.2, 1.0 };
    GLfloat matDiffuse[] = { 0.7, 0.5, 0.2, 1.0 };
    GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat matShininess[] = { 128.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess );

    glBindTexture( GL_TEXTURE_2D, 0 );  // Texture object ID == 0 means no texture mapping.
	
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);
	
    glPushMatrix();
		glTranslated( 0.3, 0.5, radius + TABLETOP_Z );
		glutSolidSphere( radius, 32, 16 );
    glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);

}



/////////////////////////////////////////////////////////////////////////////
// Draw the table.
/////////////////////////////////////////////////////////////////////////////

void DrawTable( void )
{
// Tabletop.

    GLfloat matAmbient1[] = { 0.4, 0.6, 0.8, 1.0 };
    GLfloat matDiffuse1[] = { 0.4, 0.6, 0.8, 1.0 };
    GLfloat matSpecular1[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat matShininess1[] = { 128.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient1 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse1 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular1 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess1 );

    //********************************************************
    //*********** TASK #1: WRITE YOUR CODE BELOW *************
    //********************************************************
    // Must use the SubdivideAndDrawQuad() function to draw the tabletop rectangle.
    // So that the tabletop has small enough quads to show the sharp specular highlight.
    //
    // Correct texture coordinates must be provided at the vertices of the tabletop rectangle,
    // so that the reflection tecture map is correctly mapped on it.
    //
    // The reflection on the tabletop should not be 100% (it is not a perfect mirror), 
    // and the underlying diffuse color and lighting on the tabletop must still be visible.
    //********************************************************

	// Bind reflection texture
	glBindTexture(GL_TEXTURE_2D, reflectionTexObj); 

	// Set up blending environment
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	// Alternative: GL_INTERPOLATE, basically the same result
	
	// Draw tabletop (counter-clockwise order!)
	glNormal3f(0.0, 0.0, 1.0); // Normal vector.
	SubdivideAndDrawQuad(48, 48, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z,
								0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z,
								1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z,
								1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z);


// Sides.
    GLfloat matAmbient2[] = { 0.2, 0.3, 0.4, 1.0 };
    GLfloat matDiffuse2[] = { 0.2, 0.3, 0.4, 1.0 };
    GLfloat matSpecular2[] = { 0.6, 0.8, 1.0, 1.0 };
    GLfloat matShininess2[] = { 128.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular2 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess2 );


    glBindTexture( GL_TEXTURE_2D, 0 ); // Texture object ID == 0 means no texture mapping.

    // In +y direction.
    glNormal3f( 0.0, 1.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 2,  0.0, 0.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 1.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z, 
                                  0.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z );
    // In -y direction.
    glNormal3f( 0.0, -1.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 2,  0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 0.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z, 
                                  0.0, 1.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z );
    // In +x direction.
    glNormal3f( 1.0, 0.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 2,  0.0, 0.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 0.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z, 
                                  0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z );
    // In -x direction.
    glNormal3f( -1.0, 0.0, 0.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 2,  0.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 1.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z, 
                                  0.0, 1.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z );

// Bottom.
    glNormal3f( 0.0, 0.0, -1.0 ); // Normal vector.
    SubdivideAndDrawQuad( 24, 24, 0.0, 0.0, TABLETOP_X1, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 0.0, TABLETOP_X1, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  1.0, 1.0, TABLETOP_X2, TABLETOP_Y2, TABLETOP_Z - TABLE_THICKNESS, 
                                  0.0, 1.0, TABLETOP_X2, TABLETOP_Y1, TABLETOP_Z - TABLE_THICKNESS );

// Legs.
    GLfloat matAmbient3[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat matDiffuse3[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat matSpecular3[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat matShininess3[] = { 64.0 };
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient3 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse3 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular3 );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess3 );

    glPushMatrix();
		glTranslated( TABLETOP_X1 + TABLE_THICKNESS, TABLETOP_Y1 + TABLE_THICKNESS, 0.0 );
		glScaled( TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS );
		glTranslated( 0.0, 0.0, 0.5 );
		glutSolidCube( 1.0 );
    glPopMatrix();

    glPushMatrix();
		glTranslated( TABLETOP_X2 - TABLE_THICKNESS, TABLETOP_Y1 + TABLE_THICKNESS, 0.0 );
		glScaled( TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS );
		glTranslated( 0.0, 0.0, 0.5 );
		glutSolidCube( 1.0 );
    glPopMatrix();

    glPushMatrix();
		glTranslated( TABLETOP_X2 - TABLE_THICKNESS, TABLETOP_Y2 - TABLE_THICKNESS, 0.0 );
		glScaled( TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS );
		glTranslated( 0.0, 0.0, 0.5 );
		glutSolidCube( 1.0 );
    glPopMatrix();

    glPushMatrix();
		glTranslated( TABLETOP_X1 + TABLE_THICKNESS, TABLETOP_Y2 - TABLE_THICKNESS, 0.0 );
		glScaled( TABLE_THICKNESS, TABLE_THICKNESS, TABLETOP_Z - TABLE_THICKNESS );
		glTranslated( 0.0, 0.0, 0.5 );
		glutSolidCube( 1.0 );
    glPopMatrix();

}

/////////////////////////////////////////////////////////////////////////////
// TASK 2: Draw Cube
/////////////////////////////////////////////////////////////////////////////

void DrawCube(void)
{
	double size = 0.40;

	GLfloat matAmbient[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat matDiffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matShininess[] = { 128.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

	// Set environment and bind texture
	glBindTexture(GL_TEXTURE_2D, containerTexObj);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);

	glPushMatrix();
		glTranslated(0.5, -0.5, 3 * size + TABLETOP_Z);
		glRotated(45.0, 0.0, 1.0, 0.0); // Put some spinn to it!
		glRotated(45.0, 1.0, 0.0, 0.0);
		drawBoxWithTexCoords(size); //
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
	glActiveTexture(GL_TEXTURE0);
}


/////////////////////////////////////////////////////////////////////////////
// The only glut draw function with texture coordinates was the teapot, but
// instead of re-using that one I defined my own drawBox function. With
// specified texture coordinates (and normals)
/////////////////////////////////////////////////////////////////////////////
void drawBoxWithTexCoords(GLfloat size)
{
	glBegin(GL_QUADS);
		// Front Face
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, -size/2, size/2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size/2, -size/2, size/2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2, size/2, size/2);
		// Back Face
		glNormal3d(0.0, 0.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2, size/2, -size/2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size/2, size/2, -size/2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size/2, -size/2, -size/2);
		// Top Face
		glNormal3d(0.0, 1.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2, size/2, -size/2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, size/2, size/2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size/2, size/2, -size/2);
		// Bottom Face
		glNormal3d(0.0, -1.0, 0.0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size/2, -size/2, -size/2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size/2, -size/2, size/2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2, size/2);
		// Right face
		glNormal3d(1.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size/2, -size/2, -size/2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size/2, size/2, -size/2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size/2, -size/2, size/2);
		// Left Face
		glNormal3d(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size/2, -size/2, size/2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size/2, size/2, size/2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size/2, size/2, -size/2); 
	glEnd();
}
