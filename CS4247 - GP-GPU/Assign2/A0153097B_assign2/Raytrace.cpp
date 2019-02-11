//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : E0025203@u.nus.edu
// COMMENTS TO GRADER:
// - 
//
// ============================================================
//
// FILE: Raytrace.cpp


#include <cmath>
#include <cfloat>
#include "Vector3d.h"
#include "Color.h"
#include "Ray.h"
#include "Material.h"
#include "Surface.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"
#include "Scene.h"
#include "Raytrace.h"

using namespace std;


// This is for avoiding the "epsilon problem" or the shadow acne problem.
#define DEFAULT_TMIN	10e-6		

// Use this for tmax for non-shadow ray intersection test.
#define DEFAULT_TMAX	DBL_MAX



//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////

static Vector3d mirrorReflect( const Vector3d &L, const Vector3d &N )
{
	return ( 2.0 * dot( N, L ) ) * N - L;
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting( const Vector3d &L, const Vector3d &N, const Vector3d &V,
								   const Material &mat, const PointLightSource &ptLight )
{
	Vector3d NN = ( dot( L, N ) >= 0.0 )?  N : -N;

	Vector3d R = mirrorReflect( L, NN );
	float NL = (float) dot( NN, L );
	float RVn = pow( (float) dot( R, V ), (float) mat.n );

	return ptLight.I_source * ( mat.k_d * NL  +  mat.k_r * RVn );
}





//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// reflectLevels: specfies number of levels of reflections (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////

Color Raytrace::TraceRay( const Ray &ray, const Scene &scene, 
					      int reflectLevels, bool hasShadow )
{
	Ray uRay( ray );
	uRay.makeUnitDirection();  // Normalize ray direction.


// Find whether and where the ray hits some surface. 
// Take the nearest hit point.

	bool hasHitSomething = false;
	double nearest_t = DEFAULT_TMAX;
	SurfaceHitRecord nearestHitRec;

	for ( int i = 0; i < scene.numSurfaces; i++ )
	{
		SurfaceHitRecord tempHitRec;
		bool hasHit = scene.surfacep[i]->hit( uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec );

		if ( hasHit && tempHitRec.t < nearest_t )
		{
			hasHitSomething = true;
			nearest_t = tempHitRec.t;
			nearestHitRec = tempHitRec;
		}
	}

	if ( !hasHitSomething ) return scene.backgroundColor;

	nearestHitRec.normal.makeUnitVector();
	Vector3d N = nearestHitRec.normal;	// Unit vector.
	Vector3d V = -uRay.direction();		// Unit vector.

	Color result( 0.0f, 0.0f, 0.0f );	// The result will be accumulated here.


// Add to result the phong lighting contributed by each point light source.
// Compute shadow if hasShadow is true.

	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	// Loop through point light sources and add resulting color to total color
	for (int i = 0; i < scene.numPtLights; i++)
	{
		// Assume we're not in shadow
		double k_shadow = 1;

		// Find the vector pointing towards the light source
		Vector3d L = (scene.ptLight[i].position - nearestHitRec.p);
		double newMax = L.length(); // Needed for not including occluders behind the light source
		L.makeUnitVector(); // Needed for phong lighting

		if (hasShadow)
		{
			// Create shadow ray
			Ray shadowRay(nearestHitRec.p, L);

			for (int i = 0; i < scene.numSurfaces; i++)
			{
				// Break loop if we find an occluder
				if (scene.surfacep[i]->shadowHit(shadowRay, DEFAULT_TMIN, newMax))
				{
					k_shadow = 0;
					break;
				}		
			}
		}
		
		result += k_shadow * computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
	}



// Add to result the global ambient lighting.

	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	result += nearestHitRec.mat_ptr->k_a * scene.amLight.I_a;


// Add to result the reflection of the scene.

	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	// Base case: no reflection color input
	if (reflectLevels == 0)
	{
		return result;
	}
	else // Recursively send out new reflection rays (else-statement not really needed)
	{
		Vector3d reflRay = mirrorReflect(V, N);
		Ray newRay = Ray(nearestHitRec.p, reflRay);
		return result + nearestHitRec.mat_ptr->k_rg * TraceRay(newRay, scene, reflectLevels - 1, hasShadow);
	}
	
}
