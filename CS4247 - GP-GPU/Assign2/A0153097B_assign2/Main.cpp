//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : E0025203@u.nus.edu
// COMMENTS TO GRADER:
// - The rendering of Scene 2 takes ~ 11 sec on my laptop
// 
// ============================================================
//
// FILE: Main.cpp



#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "Util.h"
#include "Vector3d.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Surface.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Scene.h"
#include "Raytrace.h"

using namespace std;


// Constants for Scene 1.
static const int imageWidth1 = 640;
static const int imageHeight1 = 480;
static const int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow1 = true;

// Constants for Scene 2.
static const int imageWidth2 = 640;
static const int imageHeight2 = 480;
static const int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow2 = true;
static const bool renderRubics = true; // If it should be an advance Rubic's cube or not




///////////////////////////////////////////////////////////////////////////
// Raytrace the whole image of the scene and write it to a file.
///////////////////////////////////////////////////////////////////////////

void RenderImage( const char *imageFilename, const Scene &scene, int reflectLevels, bool hasShadow )
{
	int imgWidth = scene.camera.getImageWidth();
	int imgHeight = scene.camera.getImageHeight();

	Image image( imgWidth, imgHeight );	// To store the result of ray tracing.

	double startTime = Util::GetCurrRealTime();
	double startCPUTime = Util::GetCurrCPUTime();

	// Generate image.
	for ( int y = 0; y < imgHeight; y++ )
	{
		double pixelPosY = y + 0.5;

		for ( int x = 0; x < imgWidth; x++ )
		{
			double pixelPosX = x + 0.5;
			Ray ray = scene.camera.getRay( pixelPosX, pixelPosY );
			Color pixelColor = Raytrace::TraceRay( ray, scene, reflectLevels, hasShadow );
			pixelColor.clamp();
			image.setPixel( x, y, pixelColor );
		}
		// printf( "%d ", y );
	}

	double stopCPUTime = Util::GetCurrCPUTime();
	double stopTime = Util::GetCurrRealTime();
	printf( "CPU time taken = %.1f sec\n", stopTime - startTime ); 
	printf( "Real time taken = %.1f sec\n", stopTime - startTime ); 

	// Write image to file.
	image.writeToFile( imageFilename );
}




// Forward declarations. These functions are defined later in the file.

void DefineScene1( Scene &scene, int imageWidth, int imageHeight );
void DefineScene2( Scene &scene, int imageWidth, int imageHeight, bool rubics);




void WaitForEnterKeyBeforeExit( void )
{
    fflush( stdin );
    getchar();
}




int main()
{
	atexit( WaitForEnterKeyBeforeExit );



// Define Scene 1.

	Scene scene1;
	DefineScene1( scene1, imageWidth1, imageHeight1 );

// Render Scene 1.

	printf( "Render Scene 1...\n" );
	RenderImage( "out1.png", scene1, reflectLevels1, hasShadow1 );
	printf( "Image completed.\n" );



// Define Scene 2.

	Scene scene2;
	DefineScene2( scene2, imageWidth2, imageHeight2, renderRubics);

// Render Scene 2.

	printf( "Render Scene 2...\n" );
	RenderImage( "out2.png", scene2, reflectLevels2, hasShadow2 );
	printf( "Image completed.\n" );


	printf( "All done.\n" );
	return 0;
}





///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 1.
///////////////////////////////////////////////////////////////////////////

void DefineScene1( Scene &scene, int imageWidth, int imageHeight )
{
	scene.backgroundColor = Color( 0.2f, 0.3f, 0.5f );

	scene.amLight.I_a = Color( 0.8f, 0.8f, 1.0f ) * 0.25f;

// Define materials.

	scene.numMaterials = 5;
	scene.material = new Material[ scene.numMaterials ];

	// Light red.
	scene.material[0].k_d = Color( 0.8f, 0.4f, 0.4f );
	scene.material[0].k_a = scene.material[0].k_d;
	scene.material[0].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[0].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[0].n = 64.0f;

	// Light green.
	scene.material[1].k_d = Color( 0.4f, 0.8f, 0.4f );
	scene.material[1].k_a = scene.material[0].k_d;
	scene.material[1].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[1].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[1].n = 64.0f;

	// Light blue.
	scene.material[2].k_d = Color( 0.4f, 0.4f, 0.8f ) * 0.9f;
	scene.material[2].k_a = scene.material[0].k_d;
	scene.material[2].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[2].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 2.5f;
	scene.material[2].n = 64.0f;

	// Yellow.
	scene.material[3].k_d = Color( 0.6f, 0.6f, 0.2f );
	scene.material[3].k_a = scene.material[0].k_d;
	scene.material[3].k_r = Color( 0.8f, 0.8f, 0.8f ) / 1.5f;
	scene.material[3].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[3].n = 64.0f;

	// Gray.
	scene.material[4].k_d = Color( 0.6f, 0.6f, 0.6f );
	scene.material[4].k_a = scene.material[0].k_d;
	scene.material[4].k_r = Color( 0.6f, 0.6f, 0.6f );
	scene.material[4].k_rg = Color( 0.8f, 0.8f, 0.8f ) / 3.0f;
	scene.material[4].n = 128.0f;


// Define point light sources.

	scene.numPtLights = 2;
	scene.ptLight = new PointLightSource[ scene.numPtLights ];

	scene.ptLight[0].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
	scene.ptLight[0].position = Vector3d( 100.0, 120.0, 10.0 );

	scene.ptLight[1].I_source = Color( 1.0f, 1.0f, 1.0f ) * 0.6f;
	scene.ptLight[1].position = Vector3d( 5.0, 80.0, 60.0 );


// Define surface primitives.

	scene.numSurfaces = 15;
	scene.surfacep = new SurfacePtr[ scene.numSurfaces ];

	scene.surfacep[0] = new Plane( 0.0, 1.0, 0.0, 0.0, &(scene.material[2]) ); // Horizontal plane.
	scene.surfacep[1] = new Plane( 1.0, 0.0, 0.0, 0.0, &(scene.material[4]) ); // Left vertical plane.
	scene.surfacep[2] = new Plane( 0.0, 0.0, 1.0, 0.0, &(scene.material[4]) ); // Right vertical plane.
	scene.surfacep[3] = new Sphere( Vector3d( 40.0, 20.0, 42.0 ), 22.0, &(scene.material[0]) ); // Big sphere.
	scene.surfacep[4] = new Sphere( Vector3d( 75.0, 10.0, 40.0 ), 12.0, &(scene.material[1]) ); // Small sphere.

	// Cube +y face.
	scene.surfacep[5] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[6] = new Triangle( Vector3d( 50.0, 20.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );

	// Cube +x face.
	scene.surfacep[7] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 20.0, 90.0 ), &(scene.material[3]) );
	scene.surfacep[8] = new Triangle( Vector3d( 50.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 90.0 ),
		                              Vector3d( 50.0, 0.0, 90.0 ), &(scene.material[3]) );

	// Cube -x face.
	scene.surfacep[9] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[10] = new Triangle( Vector3d( 30.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 30.0, 0.0, 70.0 ), &(scene.material[3]) );

	// Cube +z face.
	scene.surfacep[11] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 50.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 20.0, 90.0 ), &(scene.material[3]) );
	scene.surfacep[12] = new Triangle( Vector3d( 50.0, 0.0, 90.0 ), Vector3d( 30.0, 20.0, 90.0 ),
		                              Vector3d( 30.0, 0.0, 90.0 ), &(scene.material[3]) );

	// Cube -z face.
	scene.surfacep[13] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 30.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 20.0, 70.0 ), &(scene.material[3]) );
	scene.surfacep[14] = new Triangle( Vector3d( 30.0, 0.0, 70.0 ), Vector3d( 50.0, 20.0, 70.0 ),
		                              Vector3d( 50.0, 0.0, 70.0 ), &(scene.material[3]) );


// Define camera.

	scene.camera = Camera( Vector3d( 150.0, 120.0, 150.0 ), Vector3d( 45.0, 22.0, 55.0 ), Vector3d( 0.0, 1.0, 0.0 ),
				   (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0, 
				   imageWidth, imageHeight );
}





///////////////////////////////////////////////////////////////////////////
// Modeling of Scene 2.
///////////////////////////////////////////////////////////////////////////

void DefineScene2(Scene &scene, int imageWidth, int imageHeight, bool rubics)
{
	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	scene.backgroundColor = Color(0.2f, 0.2f, 0.2f);

	scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.15f;

	// Define materials.
	scene.numMaterials = 11;
	scene.material = new Material[scene.numMaterials];

	// Red (Cube)
	scene.material[0].k_d = Color(0.8f, 0.0f, 0.0f);
	scene.material[0].k_a = scene.amLight.I_a;
	scene.material[0].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
	scene.material[0].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
	scene.material[0].n = 64.0f;

	// Green (Cube)
	scene.material[1].k_d = Color(0.0f, 0.8f, 0.0f);
	scene.material[1].k_a = scene.amLight.I_a;
	scene.material[1].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
	scene.material[1].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
	scene.material[1].n = 64.0f;

	// Blue (Cube)
	scene.material[2].k_d = Color(0.0f, 0.0f, 0.8f);
	scene.material[2].k_a = scene.amLight.I_a;
	scene.material[2].k_r = Color(0.8f, 0.8f, 0.8f) * 0.7f;
	scene.material[2].k_rg = Color(0.8f, 0.8f, 0.8f) * 0.3f;
	scene.material[2].n = 64.0f;

	// Yellow (Cube)
	scene.material[3].k_d = Color(0.8f, 0.8f, 0.0f);
	scene.material[3].k_a = scene.amLight.I_a;
	scene.material[3].k_r = Color(0.8f, 0.8f, 0.8f) * 0.7f;
	scene.material[3].k_rg = Color(0.8f, 0.8f, 0.8f) * 0.3f;
	scene.material[3].n = 64.0f;

	// Greenish (Table/Plane #0)
	scene.material[4].k_d = Color(0.3f, 0.4f, 0.3f);
	scene.material[4].k_a = scene.amLight.I_a;
	scene.material[4].k_r = Color(0.8f, 0.8f, 0.8f) * 0.7f;
	scene.material[4].k_rg = Color(0.8f, 0.8f, 0.8f) * 0.4f;
	scene.material[4].n = 16.0f;

	// Orange (Cube)
	scene.material[5].k_d = Color(0.8f, 0.4f, 0.0f);
	scene.material[5].k_a = scene.amLight.I_a;
	scene.material[5].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
	scene.material[5].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
	scene.material[5].n = 64.0f;

	// White (Cube)
	scene.material[6].k_d = Color(0.8f, 0.8f, 0.8f);
	scene.material[6].k_a = scene.amLight.I_a;
	scene.material[6].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
	scene.material[6].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
	scene.material[6].n = 64.0f;

	// Purple (Reflectors)
	scene.material[7].k_d = Color(0.0f, 0.8f, 0.8f);
	scene.material[7].k_a = scene.amLight.I_a;
	scene.material[7].k_r = Color(0.8f, 0.8f, 0.8f) * 0.5f;
	scene.material[7].k_rg = Color(0.8f, 0.8f, 0.8f) * 0.3f;
	scene.material[7].n = 16.0f;

	// Turquoise (Reflectors)
	scene.material[8].k_d = Color(0.8f, 0.0f, 0.8f);
	scene.material[8].k_a = scene.amLight.I_a;
	scene.material[8].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
	scene.material[8].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
	scene.material[8].n = 32.0f;

	// Mirror (Plane #1)
	scene.material[9].k_d = Color(1.0f, 0.7f, 0.7f) * 0.4f;
	scene.material[9].k_a = scene.amLight.I_a;
	scene.material[9].k_r = Color(1.0f, 1.0f, 1.0f)*0.7f;
	scene.material[9].k_rg = Color(1.0f, 1.0f, 1.0f)*0.6f;
	scene.material[9].n = 128.0f;

	// Absorb (Plane #2)
	scene.material[10].k_d = Color(1.0f, 1.0f, 1.0f) * 0.35f;
	scene.material[10].k_a = scene.amLight.I_a;
	scene.material[10].k_r = Color(1.0f, 1.0f, 1.0f) * 0.1f;
	scene.material[10].k_rg = Color(1.0f, 1.0f, 1.0f) * 0.1f;
	scene.material[10].n = 0.0f;


	// Define point light sources.

	scene.numPtLights = 3;
	scene.ptLight = new PointLightSource[scene.numPtLights];

	scene.ptLight[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.9f;
	scene.ptLight[0].position = Vector3d(120.0, 250.0, 100.0);

	scene.ptLight[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.7;
	scene.ptLight[1].position = Vector3d(0.0, 220.0, 100.0);

	scene.ptLight[2].I_source = Color(1.0f, 1.0f, 1.0f) * 0.4;
	scene.ptLight[2].position = Vector3d(-90.0, 220.0, 0.0);


	// Define surface primitives.
	if(rubics)
	{
		scene.numSurfaces = 119;
		scene.surfacep = new SurfacePtr[scene.numSurfaces];

		// Basic reflectors
		scene.surfacep[0] = new Plane(Vector3d(0.0, 1.0, 0.0), Vector3d(0.0, 0.0, 0.0), &(scene.material[4])); // Bottom
		scene.surfacep[1] = new Plane(Vector3d(1.0, 0.0, 0.0), Vector3d(-100.0, 0.0, 0.0), &(scene.material[9])); // Mirror (Left)
		scene.surfacep[2] = new Plane(Vector3d(0.0, 0.0, 1.0), Vector3d(0.0, 0.0, -550.0), &(scene.material[10])); // Diffuse plane (Right)
		scene.surfacep[3] = new Sphere(Vector3d(0.0, 25.0, 0.0), 25.0, &(scene.material[7])); // Base sphere.
		scene.surfacep[118] = new Sphere(Vector3d(-50.0, 95.0, 70.0), 15.0, &(scene.material[8])); // Reflective sphere.


		// Pyramid
		scene.surfacep[4] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(35.0, 100.0, 0.0), Vector3d(0.0, 100.0, 35.0), &(scene.material[8]));
		scene.surfacep[5] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(0.0, 100.0, 35.0), Vector3d(-35.0, 100.0, 0.0), &(scene.material[8]));
		scene.surfacep[6] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(-35.0, 100.0, 0.0), Vector3d(0.0, 100.0, -35.0), &(scene.material[8]));
		scene.surfacep[7] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(0.0, 100.0, -35.0), Vector3d(35.0, 100.0, 0.0), &(scene.material[8]));
		scene.surfacep[8] = new Triangle(Vector3d(-35.0, 100.0, 0.0), Vector3d(35.0, 100.0, 0.0), Vector3d(0.0, 100.0, -35.0), &(scene.material[8]));
		scene.surfacep[9] = new Triangle(Vector3d(-35.0, 100.0, 0.0), Vector3d(0.0, 100.0, 35.0), Vector3d(35.0, 100.0, 0.0), &(scene.material[8]));

		// Rubic's cube
		// Corners defined top-down, clockwise order
		Vector3d v0(-30.0, 160.0, 30.0); Vector3d v1(-30.0, 140.0, 30.0); Vector3d v2(-30.0, 120.0, 30.0); Vector3d v3(-30.0, 100.0, 30.0);
		Vector3d v4(-10.0, 160.0, 30.0); Vector3d v5(-10.0, 140.0, 30.0); Vector3d v6(-10.0, 120.0, 30.0); Vector3d v7(-10.0, 100.0, 30.0);
		Vector3d v8(10.0, 160.0, 30.0); Vector3d v9(10.0, 140.0, 30.0); Vector3d v10(10.0, 120.0, 30.0); Vector3d v11(10.0, 100.0, 30.0);
		Vector3d v12(30.0, 160.0, 30.0); Vector3d v13(30.0, 140.0, 30.0); Vector3d v14(30.0, 120.0, 30.0); Vector3d v15(30.0, 100.0, 30.0);
		Vector3d v16(30.0, 160.0, 10.0); Vector3d v17(30.0, 140.0, 10.0); Vector3d v18(30.0, 120.0, 10.0); Vector3d v19(30.0, 100.0, 10.0);
		Vector3d v20(30.0, 160.0, -10.0); Vector3d v21(30.0, 140.0, -10.0); Vector3d v22(30.0, 120.0, -10.0); Vector3d v23(30.0, 100.0, -10.0);
		Vector3d v24(30.0, 160.0, -30.0); Vector3d v25(30.0, 140.0, -30.0); Vector3d v26(30.0, 120.0, -30.0); Vector3d v27(30.0, 100.0, -30.0);
		Vector3d v28(10.0, 160.0, -30.0); Vector3d v29(10.0, 140.0, -30.0); Vector3d v30(10.0, 120.0, -30.0); Vector3d v31(10.0, 100.0, -30.0);
		Vector3d v32(-10.0, 160.0, -30.0); Vector3d v33(-10.0, 140.0, -30.0); Vector3d v34(-10.0, 120.0, -30.0); Vector3d v35(-10.0, 100.0, -30.0);
		Vector3d v36(-30.0, 160.0, -30.0); Vector3d v37(-30.0, 140.0, -30.0); Vector3d v38(-30.0, 120.0, -30.0); Vector3d v39(-30.0, 100.0, -30.0);
		Vector3d v40(-30.0, 160.0, -10.0); Vector3d v41(-30.0, 140.0, -10.0); Vector3d v42(-30.0, 120.0, -10.0); Vector3d v43(-30.0, 100.0, -10.0);
		Vector3d v44(-30.0, 160.0, 10.0); Vector3d v45(-30.0, 140.0, 10.0); Vector3d v46(-30.0, 120.0, 10.0); Vector3d v47(-30.0, 100.0, 10.0);

		Vector3d v48(-10.0, 160.0, 10.0); Vector3d v49(10.0, 160.0, 10.0); Vector3d v50(10.0, 160.0, -10.0); Vector3d v51(-10.0, 160.0, -10.0); // Top inner square
		Vector3d v52(-10.0, 100.0, 10.0); Vector3d v53(10.0, 100.0, 10.0); Vector3d v54(10.0, 100.0, -10.0); Vector3d v55(-10.0, 100.0, -10.0); // Bottom inner suare

		// Random color approach
		// Cube +y face 
		scene.surfacep[10] = new Triangle(v0, v4, v48, &(scene.material[0]));
		scene.surfacep[11] = new Triangle(v0, v48, v44, &(scene.material[0]));
		scene.surfacep[12] = new Triangle(v44, v48, v51, &(scene.material[1]));
		scene.surfacep[13] = new Triangle(v44, v51, v40, &(scene.material[1]));
		scene.surfacep[14] = new Triangle(v40, v51, v32, &(scene.material[5]));
		scene.surfacep[15] = new Triangle(v40, v32, v36, &(scene.material[5]));

		scene.surfacep[16] = new Triangle(v4, v8, v49, &(scene.material[1]));
		scene.surfacep[17] = new Triangle(v4, v49, v48, &(scene.material[1]));
		scene.surfacep[18] = new Triangle(v48, v49, v50, &(scene.material[2]));
		scene.surfacep[19] = new Triangle(v48, v50, v51, &(scene.material[2]));
		scene.surfacep[20] = new Triangle(v51, v50, v28, &(scene.material[5]));
		scene.surfacep[21] = new Triangle(v51, v28, v32, &(scene.material[5]));

		scene.surfacep[22] = new Triangle(v8, v12, v16, &(scene.material[3]));
		scene.surfacep[23] = new Triangle(v8, v16, v49, &(scene.material[3]));
		scene.surfacep[24] = new Triangle(v49, v16, v20, &(scene.material[2]));
		scene.surfacep[25] = new Triangle(v49, v20, v50, &(scene.material[2]));
		scene.surfacep[26] = new Triangle(v50, v20, v24, &(scene.material[3]));
		scene.surfacep[27] = new Triangle(v50, v24, v32, &(scene.material[3]));

		// Cube -y face 
		scene.surfacep[28] = new Triangle(v15, v11, v53, &(scene.material[6]));
		scene.surfacep[29] = new Triangle(v15, v53, v19, &(scene.material[6]));
		scene.surfacep[30] = new Triangle(v19, v53, v54, &(scene.material[1]));
		scene.surfacep[31] = new Triangle(v19, v54, v23, &(scene.material[1]));
		scene.surfacep[32] = new Triangle(v23, v54, v31, &(scene.material[6]));
		scene.surfacep[33] = new Triangle(v23, v31, v27, &(scene.material[6]));

		scene.surfacep[34] = new Triangle(v11, v7, v52, &(scene.material[0]));
		scene.surfacep[35] = new Triangle(v11, v52, v53, &(scene.material[0]));
		scene.surfacep[36] = new Triangle(v53, v52, v55, &(scene.material[6]));
		scene.surfacep[37] = new Triangle(v53, v55, v54, &(scene.material[6]));
		scene.surfacep[38] = new Triangle(v54, v55, v35, &(scene.material[1]));
		scene.surfacep[39] = new Triangle(v54, v35, v31, &(scene.material[1]));

		scene.surfacep[40] = new Triangle(v7, v3, v47, &(scene.material[6]));
		scene.surfacep[41] = new Triangle(v7, v47, v52, &(scene.material[6]));
		scene.surfacep[42] = new Triangle(v52, v47, v43, &(scene.material[1]));
		scene.surfacep[43] = new Triangle(v52, v43, v55, &(scene.material[1]));
		scene.surfacep[44] = new Triangle(v55, v43, v39, &(scene.material[6]));
		scene.surfacep[45] = new Triangle(v55, v39, v35, &(scene.material[6]));

		// Cube +x face (Swedish flag!)
		scene.surfacep[46] = new Triangle(v15, v19, v18, &(scene.material[2]));
		scene.surfacep[47] = new Triangle(v15, v18, v14, &(scene.material[2]));
		scene.surfacep[48] = new Triangle(v14, v18, v17, &(scene.material[3]));
		scene.surfacep[49] = new Triangle(v14, v17, v13, &(scene.material[3]));
		scene.surfacep[50] = new Triangle(v13, v17, v16, &(scene.material[2]));
		scene.surfacep[51] = new Triangle(v13, v16, v12, &(scene.material[2]));

		scene.surfacep[52] = new Triangle(v19, v23, v22, &(scene.material[3]));
		scene.surfacep[53] = new Triangle(v19, v22, v18, &(scene.material[3]));
		scene.surfacep[54] = new Triangle(v18, v22, v21, &(scene.material[3]));
		scene.surfacep[55] = new Triangle(v18, v21, v17, &(scene.material[3]));
		scene.surfacep[56] = new Triangle(v17, v21, v20, &(scene.material[3]));
		scene.surfacep[57] = new Triangle(v17, v20, v16, &(scene.material[3]));

		scene.surfacep[58] = new Triangle(v23, v27, v26, &(scene.material[2]));
		scene.surfacep[59] = new Triangle(v23, v26, v22, &(scene.material[2]));
		scene.surfacep[60] = new Triangle(v22, v26, v25, &(scene.material[3]));
		scene.surfacep[61] = new Triangle(v22, v25, v21, &(scene.material[3]));
		scene.surfacep[62] = new Triangle(v21, v25, v24, &(scene.material[2]));
		scene.surfacep[63] = new Triangle(v21, v24, v20, &(scene.material[2]));

		// Cube -x face
		scene.surfacep[64] = new Triangle(v39, v43, v42, &(scene.material[1]));
		scene.surfacep[65] = new Triangle(v39, v42, v38, &(scene.material[1]));
		scene.surfacep[66] = new Triangle(v38, v42, v41, &(scene.material[2]));
		scene.surfacep[67] = new Triangle(v38, v41, v37, &(scene.material[2]));
		scene.surfacep[68] = new Triangle(v37, v41, v40, &(scene.material[0]));
		scene.surfacep[69] = new Triangle(v37, v40, v36, &(scene.material[0]));

		scene.surfacep[70] = new Triangle(v43, v47, v46, &(scene.material[0]));
		scene.surfacep[71] = new Triangle(v43, v46, v42, &(scene.material[0]));
		scene.surfacep[72] = new Triangle(v42, v46, v45, &(scene.material[5]));
		scene.surfacep[73] = new Triangle(v42, v45, v41, &(scene.material[5]));
		scene.surfacep[74] = new Triangle(v41, v45, v44, &(scene.material[6]));
		scene.surfacep[75] = new Triangle(v41, v44, v40, &(scene.material[6]));

		scene.surfacep[76] = new Triangle(v47, v3, v2, &(scene.material[3]));
		scene.surfacep[77] = new Triangle(v47, v2, v46, &(scene.material[3]));
		scene.surfacep[78] = new Triangle(v46, v2, v1, &(scene.material[1]));
		scene.surfacep[79] = new Triangle(v46, v1, v45, &(scene.material[1]));
		scene.surfacep[80] = new Triangle(v45, v1, v0, &(scene.material[3]));
		scene.surfacep[81] = new Triangle(v45, v0, v44, &(scene.material[3]));

		// Cube +z face
		scene.surfacep[82] = new Triangle(v3, v7, v6, &(scene.material[6]));
		scene.surfacep[83] = new Triangle(v3, v6, v2, &(scene.material[6]));
		scene.surfacep[84] = new Triangle(v2, v6, v5, &(scene.material[2]));
		scene.surfacep[85] = new Triangle(v2, v5, v1, &(scene.material[2]));
		scene.surfacep[86] = new Triangle(v1, v5, v4, &(scene.material[6]));
		scene.surfacep[87] = new Triangle(v1, v4, v0, &(scene.material[6]));

		scene.surfacep[88] = new Triangle(v7, v11, v10, &(scene.material[1]));
		scene.surfacep[89] = new Triangle(v7, v10, v6, &(scene.material[1]));
		scene.surfacep[90] = new Triangle(v6, v10, v9, &(scene.material[0]));
		scene.surfacep[91] = new Triangle(v6, v9, v5, &(scene.material[0]));
		scene.surfacep[92] = new Triangle(v5, v9, v8, &(scene.material[2]));
		scene.surfacep[93] = new Triangle(v5, v8, v4, &(scene.material[2]));

		scene.surfacep[94] = new Triangle(v11, v15, v14, &(scene.material[5]));
		scene.surfacep[95] = new Triangle(v11, v14, v10, &(scene.material[5]));
		scene.surfacep[96] = new Triangle(v10, v14, v13, &(scene.material[0]));
		scene.surfacep[97] = new Triangle(v10, v13, v9, &(scene.material[0]));
		scene.surfacep[98] = new Triangle(v9, v13, v12, &(scene.material[5]));
		scene.surfacep[99] = new Triangle(v9, v12, v8, &(scene.material[5]));

		// Cube -z face
		scene.surfacep[100] = new Triangle(v27, v31, v30, &(scene.material[0]));
		scene.surfacep[101] = new Triangle(v27, v30, v26, &(scene.material[0]));
		scene.surfacep[102] = new Triangle(v26, v30, v29, &(scene.material[5]));
		scene.surfacep[103] = new Triangle(v26, v29, v25, &(scene.material[5]));
		scene.surfacep[104] = new Triangle(v25, v29, v28, &(scene.material[0]));
		scene.surfacep[105] = new Triangle(v25, v28, v24, &(scene.material[0]));

		scene.surfacep[106] = new Triangle(v31, v35, v34, &(scene.material[5]));
		scene.surfacep[107] = new Triangle(v31, v34, v30, &(scene.material[5]));
		scene.surfacep[108] = new Triangle(v30, v34, v33, &(scene.material[5]));
		scene.surfacep[109] = new Triangle(v30, v33, v29, &(scene.material[5]));
		scene.surfacep[110] = new Triangle(v29, v33, v32, &(scene.material[5]));
		scene.surfacep[111] = new Triangle(v29, v32, v28, &(scene.material[5]));

		scene.surfacep[112] = new Triangle(v35, v39, v38, &(scene.material[0]));
		scene.surfacep[113] = new Triangle(v35, v38, v34, &(scene.material[0]));
		scene.surfacep[114] = new Triangle(v34, v38, v37, &(scene.material[5]));
		scene.surfacep[115] = new Triangle(v34, v37, v33, &(scene.material[5]));
		scene.surfacep[116] = new Triangle(v33, v37, v36, &(scene.material[0]));
		scene.surfacep[117] = new Triangle(v33, v36, v32, &(scene.material[0]));
	}
	else {
		scene.numSurfaces = 24;
		scene.surfacep = new SurfacePtr[scene.numSurfaces];

		// Basic reflectors
		scene.surfacep[0] = new Plane(Vector3d(0.0, 1.0, 0.0), Vector3d(0.0, 0.0, 0.0), &(scene.material[4])); // Gray Bottom
		scene.surfacep[1] = new Plane(Vector3d(1.0, 0.0, 0.0), Vector3d(-90.0, 0.0, 0.0), &(scene.material[9])); // Mirror (Left)
		scene.surfacep[2] = new Plane(Vector3d(0.0, 0.0, 1.0), Vector3d(0.0, 0.0, -1200.0), &(scene.material[10])); // Diffuse plane (Right)
		scene.surfacep[3] = new Sphere(Vector3d(0.0, 25.0, 0.0), 25.0, &(scene.material[7])); // Base sphere.
		scene.surfacep[4] = new Sphere(Vector3d(70.0, -140.0, 0.0), 15.0, &(scene.material[7])); // Reflective sphere.
		scene.surfacep[5] = new Sphere(Vector3d(-40.0, 90.0, 70.0), 15.0, &(scene.material[8])); // Reflective sphere.

		// Pyramid
		scene.surfacep[6] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(35.0, 100.0, 0.0), Vector3d(0.0, 100.0, 35.0), &(scene.material[8]));
		scene.surfacep[7] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(0.0, 100.0, 35.0), Vector3d(-35.0, 100.0, 0.0), &(scene.material[8]));
		scene.surfacep[8] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(-35.0, 100.0, 0.0), Vector3d(0.0, 100.0, -35.0), &(scene.material[8]));
		scene.surfacep[9] = new Triangle(Vector3d(0.0, 50.0, 0.0), Vector3d(0.0, 100.0, -35.0), Vector3d(35.0, 100.0, 0.0), &(scene.material[8]));
		scene.surfacep[10] = new Triangle(Vector3d(-35.0, 100.0, 0.0), Vector3d(35.0, 100.0, 0.0), Vector3d(0.0, 100.0, -35.0), &(scene.material[8]));
		scene.surfacep[11] = new Triangle(Vector3d(-35.0, 100.0, 0.0), Vector3d(0.0, 100.0, 35.0), Vector3d(35.0, 100.0, 0.0), &(scene.material[8]));

		// Cube +y face (Yellow)
		scene.surfacep[12] = new Triangle(Vector3d(-30.0, 160.0, 30.0), Vector3d(30.0, 160.0, 30.0), Vector3d(30.0, 160.0, -30.0), &(scene.material[3]));
		scene.surfacep[13] = new Triangle(Vector3d(-30.0, 160.0, 30.0), Vector3d(30.0, 160.0, -30.0), Vector3d(-30.0, 160.0, -30.0), &(scene.material[3]));

		// Cube -y face (White)
		scene.surfacep[14] = new Triangle(Vector3d(-30.0, 100.0, 30.0), Vector3d(-30.0, 100.0, -30.0), Vector3d(30.0, 100.0, -30.0), &(scene.material[6]));
		scene.surfacep[15] = new Triangle(Vector3d(-30.0, 100.0, 30.0), Vector3d(30.0, 100.0, -30.0), Vector3d(30.0, 100.0, 30.0), &(scene.material[6]));

		// Cube +x face (Orange)
		scene.surfacep[16] = new Triangle(Vector3d(30.0, 160, 30.0), Vector3d(30.0, 100, 30.0), Vector3d(30.0, 100, -30.0), &(scene.material[5]));
		scene.surfacep[17] = new Triangle(Vector3d(30.0, 160, 30.0), Vector3d(30.0, 100, -30.0), Vector3d(30.0, 160, -30.0), &(scene.material[5]));

		// Cube -x face (Red)
		scene.surfacep[18] = new Triangle(Vector3d(-30.0, 100.0, -30.0), Vector3d(-30.0, 100.0, 30.0), Vector3d(-30.0, 160.0, 30.0), &(scene.material[0]));
		scene.surfacep[19] = new Triangle(Vector3d(-30.0, 100.0, -30.0), Vector3d(-30.0, 160.0, 30.0), Vector3d(-30.0, 160.0, -30.0), &(scene.material[0]));

		// Cube +z face (Green)
		scene.surfacep[20] = new Triangle(Vector3d(-30.0, 100.0, 30.0), Vector3d(30.0, 100.0, 30.0), Vector3d(30.0, 160.0, 30.0), &(scene.material[1]));
		scene.surfacep[21] = new Triangle(Vector3d(-30.0, 100.0, 30.0), Vector3d(30.0, 160.0, 30.0), Vector3d(-30.0, 160.0, 30.0), &(scene.material[1]));

		// Cube -z face (Blue)
		scene.surfacep[22] = new Triangle(Vector3d(30.0, 100.0, -30.0), Vector3d(-30.0, 100.0, -30.0), Vector3d(-30.0, 160.0, -30.0), &(scene.material[2]));
		scene.surfacep[23] = new Triangle(Vector3d(30.0, 100.0, -30.0), Vector3d(-30.0, 160.0, -30.0), Vector3d(30.0, 160.0, -30.0), &(scene.material[2]));
	}


	// Define camera.
	scene.camera = Camera(Vector3d(220.0, 210.0, 160.0), Vector3d(-35.0, 90.0, 25.0), Vector3d(0.0, 1.0, 0.0),
		(-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
		imageWidth, imageHeight);
}

/*		
		// Cross color approach
		// Cube +y face (Blue cross over yellow bg)
		scene.surfacep[10] = new Triangle(v0, v4, v48, &(scene.material[3]));
		scene.surfacep[11] = new Triangle(v0, v48, v44, &(scene.material[3]));
		scene.surfacep[12] = new Triangle(v44, v48, v51, &(scene.material[2]));
		scene.surfacep[13] = new Triangle(v44, v51, v40, &(scene.material[2]));
		scene.surfacep[14] = new Triangle(v40, v51, v32, &(scene.material[3]));
		scene.surfacep[15] = new Triangle(v40, v32, v36, &(scene.material[3]));

		scene.surfacep[16] = new Triangle(v4, v8, v49, &(scene.material[2]));
		scene.surfacep[17] = new Triangle(v4, v49, v48, &(scene.material[2]));
		scene.surfacep[18] = new Triangle(v48, v49, v50, &(scene.material[2]));
		scene.surfacep[19] = new Triangle(v48, v50, v51, &(scene.material[2]));
		scene.surfacep[20] = new Triangle(v51, v50, v28, &(scene.material[2]));
		scene.surfacep[21] = new Triangle(v51, v28, v32, &(scene.material[2]));

		scene.surfacep[22] = new Triangle(v8, v12, v16, &(scene.material[3]));
		scene.surfacep[23] = new Triangle(v8, v16, v49, &(scene.material[3]));
		scene.surfacep[24] = new Triangle(v49, v16, v20, &(scene.material[2]));
		scene.surfacep[25] = new Triangle(v49, v20, v50, &(scene.material[2]));
		scene.surfacep[26] = new Triangle(v50, v20, v24, &(scene.material[3]));
		scene.surfacep[27] = new Triangle(v50, v24, v32, &(scene.material[3]));

		// Cube -y face (Green cross over white bg)
		scene.surfacep[28] = new Triangle(v15, v11, v53, &(scene.material[6]));
		scene.surfacep[29] = new Triangle(v15, v53, v19, &(scene.material[6]));
		scene.surfacep[30] = new Triangle(v19, v53, v54, &(scene.material[1]));
		scene.surfacep[31] = new Triangle(v19, v54, v23, &(scene.material[1]));
		scene.surfacep[32] = new Triangle(v23, v54, v31, &(scene.material[6]));
		scene.surfacep[33] = new Triangle(v23, v31, v27, &(scene.material[6]));

		scene.surfacep[34] = new Triangle(v11, v7, v52, &(scene.material[1]));
		scene.surfacep[35] = new Triangle(v11, v52, v53, &(scene.material[1]));
		scene.surfacep[36] = new Triangle(v53, v52, v55, &(scene.material[1]));
		scene.surfacep[37] = new Triangle(v53, v55, v54, &(scene.material[1]));
		scene.surfacep[38] = new Triangle(v54, v55, v35, &(scene.material[1]));
		scene.surfacep[39] = new Triangle(v54, v35, v31, &(scene.material[1]));

		scene.surfacep[40] = new Triangle(v7, v3, v47, &(scene.material[6]));
		scene.surfacep[41] = new Triangle(v7, v47, v52, &(scene.material[6]));
		scene.surfacep[42] = new Triangle(v52, v47, v43, &(scene.material[1]));
		scene.surfacep[43] = new Triangle(v52, v43, v55, &(scene.material[1]));
		scene.surfacep[44] = new Triangle(v55, v43, v39, &(scene.material[6]));
		scene.surfacep[45] = new Triangle(v55, v39, v35, &(scene.material[6]));

		// Cube +x face (Yellow cross over blue bg)
		scene.surfacep[46] = new Triangle(v15, v19, v18, &(scene.material[2]));
		scene.surfacep[47] = new Triangle(v15, v18, v14, &(scene.material[2]));
		scene.surfacep[48] = new Triangle(v14, v18, v17, &(scene.material[3]));
		scene.surfacep[49] = new Triangle(v14, v17, v13, &(scene.material[3]));
		scene.surfacep[50] = new Triangle(v13, v17, v16, &(scene.material[2]));
		scene.surfacep[51] = new Triangle(v13, v16, v12, &(scene.material[2]));

		scene.surfacep[52] = new Triangle(v19, v23, v22, &(scene.material[3]));
		scene.surfacep[53] = new Triangle(v19, v22, v18, &(scene.material[3]));
		scene.surfacep[54] = new Triangle(v18, v22, v21, &(scene.material[3]));
		scene.surfacep[55] = new Triangle(v18, v21, v17, &(scene.material[3]));
		scene.surfacep[56] = new Triangle(v17, v21, v20, &(scene.material[3]));
		scene.surfacep[57] = new Triangle(v17, v20, v16, &(scene.material[3]));

		scene.surfacep[58] = new Triangle(v23, v27, v26, &(scene.material[2]));
		scene.surfacep[59] = new Triangle(v23, v26, v22, &(scene.material[2]));
		scene.surfacep[60] = new Triangle(v22, v26, v25, &(scene.material[3]));
		scene.surfacep[61] = new Triangle(v22, v25, v21, &(scene.material[3]));
		scene.surfacep[62] = new Triangle(v21, v25, v24, &(scene.material[2]));
		scene.surfacep[63] = new Triangle(v21, v24, v20, &(scene.material[2]));

		// Cube -x face (White cross over green bg)
		scene.surfacep[64] = new Triangle(v39, v43, v42, &(scene.material[1]));
		scene.surfacep[65] = new Triangle(v39, v42, v38, &(scene.material[1]));
		scene.surfacep[66] = new Triangle(v38, v42, v41, &(scene.material[6]));
		scene.surfacep[67] = new Triangle(v38, v41, v37, &(scene.material[6]));
		scene.surfacep[68] = new Triangle(v37, v41, v40, &(scene.material[1]));
		scene.surfacep[69] = new Triangle(v37, v40, v36, &(scene.material[1]));

		scene.surfacep[70] = new Triangle(v43, v47, v46, &(scene.material[6]));
		scene.surfacep[71] = new Triangle(v43, v46, v42, &(scene.material[6]));
		scene.surfacep[72] = new Triangle(v42, v46, v45, &(scene.material[6]));
		scene.surfacep[73] = new Triangle(v42, v45, v41, &(scene.material[6]));
		scene.surfacep[74] = new Triangle(v41, v45, v44, &(scene.material[6]));
		scene.surfacep[75] = new Triangle(v41, v44, v40, &(scene.material[6]));

		scene.surfacep[76] = new Triangle(v47, v3, v2, &(scene.material[1]));
		scene.surfacep[77] = new Triangle(v47, v2, v46, &(scene.material[1]));
		scene.surfacep[78] = new Triangle(v46, v2, v1, &(scene.material[6]));
		scene.surfacep[79] = new Triangle(v46, v1, v45, &(scene.material[6]));
		scene.surfacep[80] = new Triangle(v45, v1, v0, &(scene.material[1]));
		scene.surfacep[81] = new Triangle(v45, v0, v44, &(scene.material[1]));

		// Cube +z face (Red cross over orange bg)
		scene.surfacep[82] = new Triangle(v3, v7, v6, &(scene.material[5]));
		scene.surfacep[83] = new Triangle(v3, v6, v2, &(scene.material[5]));
		scene.surfacep[84] = new Triangle(v2, v6, v5, &(scene.material[0]));
		scene.surfacep[85] = new Triangle(v2, v5, v1, &(scene.material[0]));
		scene.surfacep[86] = new Triangle(v1, v5, v4, &(scene.material[5]));
		scene.surfacep[87] = new Triangle(v1, v4, v0, &(scene.material[5]));

		scene.surfacep[88] = new Triangle(v7, v11, v10, &(scene.material[0]));
		scene.surfacep[89] = new Triangle(v7, v10, v6, &(scene.material[0]));
		scene.surfacep[90] = new Triangle(v6, v10, v9, &(scene.material[0]));
		scene.surfacep[91] = new Triangle(v6, v9, v5, &(scene.material[0]));
		scene.surfacep[92] = new Triangle(v5, v9, v8, &(scene.material[0]));
		scene.surfacep[93] = new Triangle(v5, v8, v4, &(scene.material[0]));

		scene.surfacep[94] = new Triangle(v11, v15, v14, &(scene.material[5]));
		scene.surfacep[95] = new Triangle(v11, v14, v10, &(scene.material[5]));
		scene.surfacep[96] = new Triangle(v10, v14, v13, &(scene.material[0]));
		scene.surfacep[97] = new Triangle(v10, v13, v9, &(scene.material[0]));
		scene.surfacep[98] = new Triangle(v9, v13, v12, &(scene.material[5]));
		scene.surfacep[99] = new Triangle(v9, v12, v8, &(scene.material[5]));

		// Cube -z face (Orange cross over red bg)
		scene.surfacep[100] = new Triangle(v27, v31, v30, &(scene.material[0]));
		scene.surfacep[101] = new Triangle(v27, v30, v26, &(scene.material[0]));
		scene.surfacep[102] = new Triangle(v26, v30, v29, &(scene.material[5]));
		scene.surfacep[103] = new Triangle(v26, v29, v25, &(scene.material[5]));
		scene.surfacep[104] = new Triangle(v25, v29, v28, &(scene.material[0]));
		scene.surfacep[105] = new Triangle(v25, v28, v24, &(scene.material[0]));

		scene.surfacep[106] = new Triangle(v31, v35, v34, &(scene.material[5]));
		scene.surfacep[107] = new Triangle(v31, v34, v30, &(scene.material[5]));
		scene.surfacep[108] = new Triangle(v30, v34, v33, &(scene.material[5]));
		scene.surfacep[109] = new Triangle(v30, v33, v29, &(scene.material[5]));
		scene.surfacep[110] = new Triangle(v29, v33, v32, &(scene.material[5]));
		scene.surfacep[111] = new Triangle(v29, v32, v28, &(scene.material[5]));

		scene.surfacep[112] = new Triangle(v35, v39, v38, &(scene.material[0]));
		scene.surfacep[113] = new Triangle(v35, v38, v34, &(scene.material[0]));
		scene.surfacep[114] = new Triangle(v34, v38, v37, &(scene.material[5]));
		scene.surfacep[115] = new Triangle(v34, v37, v33, &(scene.material[5]));
		scene.surfacep[116] = new Triangle(v33, v37, v36, &(scene.material[0]));
		scene.surfacep[117] = new Triangle(v33, v36, v32, &(scene.material[0]));*/