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

static Vector3d mirrorReflect(const Vector3d &L, const Vector3d &N)
{
	return (2.0 * dot(N, L)) * N - L;
}


//////////////////////////////////////////////////////////////////////////////
// Compute the outgoing transmitted/refraction vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output transmitted vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
//////////////////////////////////////////////////////////////////////////////
static Vector3d calcTransmittedRay(const Vector3d &L, const Vector3d &N, const float u)
{
	float cosTheta = (float)dot(L, N); // CosTheta1 - incident angle
	float sinTheta = u * (float)sqrt(fmax(1 - cosTheta*cosTheta, 0.0f)); // sinTheta2 = (u1/u2)*sinTheta1 
	float k = (1 - (float)pow(u, 2) * (1 - (float)pow(cosTheta, 2)));

	// Check so we don't get imaginary result
	// Also so we don't have total internal reflection => no refraction ray
	if (k > 0.0f && sinTheta <= 1) // If sinTheta2 = 1 then theta2 = 90 degrees => >1 == total reflection
		//return -u * L + (u*dot(N, L) - sqrt(1 - pow(u, 2) * (1 - pow(dot(N, L), 2))))*N;
		return -u*L + (u*cosTheta - sqrt(k))*N;
	else
		return Vector3d(0.0f, 0.0f, 0.0f);
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ]
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computePhongLighting(const Vector3d &L, const Vector3d &N, const Vector3d &V,
	const Material &mat, const PointLightSource &ptLight)
{
	Vector3d NN = (dot(L, N) >= 0.0) ? N : -N;

	Vector3d R = mirrorReflect(L, NN);
	float NL = (float)dot(NN, L);
	float RVn = pow((float)dot(R, V), (float)mat.n);

	return ptLight.I_source * (mat.k_d * NL + mat.k_r * RVn);
}



//////////////////////////////////////////////////////////////////////////////
// Compute I_source * [ k_t * (T.V)^m].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
//////////////////////////////////////////////////////////////////////////////

static Color computeRefraction(const Vector3d &L, const Vector3d &N, const Vector3d &V, const Material &mat, const PointLightSource &ptLight)
{
	// Checkif we are inside or outside of material
	Vector3d NN = ((float)dot(L, N) >= 0.0) ? N : -N;
	float u = ((float)dot(L, N) >= 0.0) ? 1.0f/mat.u : mat.u/1.0f;

	Vector3d T = calcTransmittedRay(L, N, u);

	float TVm = pow((float)dot(T, V), (float)mat.m);

	return ptLight.I_source * (mat.k_t * TVm);
}




//////////////////////////////////////////////////////////////////////////////
// Traces a ray into the scene.
// depth: specfies number of levels of reflections left (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
//////////////////////////////////////////////////////////////////////////////
Color Raytrace::TraceRay(const Ray &ray, const Scene &scene, int depth, bool hasShadow)
{

	Ray uRay(ray);
	uRay.makeUnitDirection();  // Normalize ray direction.


	// Find whether and where the ray hits some surface. 
	// Take the nearest hit point.

	bool hasHitSomething = false;
	double nearest_t = DEFAULT_TMAX;
	SurfaceHitRecord nearestHitRec;

	for (int i = 0; i < scene.numSurfaces; i++)
	{
		SurfaceHitRecord tempHitRec;
		bool hasHit = scene.surfacep[i]->hit(uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec);

		if (hasHit && tempHitRec.t < nearest_t)
		{
			hasHitSomething = true;
			nearest_t = tempHitRec.t;
			nearestHitRec = tempHitRec;
		}
	}

	if (!hasHitSomething) return scene.backgroundColor;

	nearestHitRec.normal.makeUnitVector();
	Vector3d N = nearestHitRec.normal;	// Unit vector.
	Vector3d V = -uRay.direction();		// Unit vector.

	Color result(0.0f, 0.0f, 0.0f);	// The result will be accumulated here.


	// Add to result the phong lighting contributed by each point light source.
	// Compute shadow if hasShadow is true.

	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	// Loop through point light sources and add resulting color to total color
	for (int i = 0; i < scene.numPtLights; i++)
	{
		// Assume we're not in shadow
		float k_shadow = 1.0f;

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
					k_shadow = 0.2f;
					break;
				}
			}
		}

		// Compute local color
		Color refl = computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
		Color refr = computeRefraction(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
		result += k_shadow * (refl + refr);

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

	Color zero(0.0f, 0.0f, 0.0f);
	
	// If material has refractive component
	if (depth > 0 && nearestHitRec.mat_ptr->k_tg != zero) // Recursively send out new transmitted rays
	{
		// Switch shadows so we don't check for shadows when we are inside of object
		//bool shadow = (hasShadow) ? false : true;

		// Check if we are inside an object
		// (Clean air/vacuum has u ~= 1.0)
		float u = (dot(V, N) >= 0.0) ? 1.0f / nearestHitRec.mat_ptr->u : nearestHitRec.mat_ptr->u / 1.0f;
		Vector3d NN = (dot(V, N) >= 0.0) ? N : -N;

		// Calculate new refraction ray
		Vector3d transmitDir = calcTransmittedRay(V, NN, u);
		transmitDir.makeUnitVector();
		Ray newTransRay = Ray(nearestHitRec.p, transmitDir);

		return result + nearestHitRec.mat_ptr->k_tg * TraceRay(newTransRay, scene, depth - 1, hasShadow);
	}

	// If material has reflective component
	if (depth > 0 && nearestHitRec.mat_ptr->k_rg != zero) // Recursively send out new reflection rays
	{
		// Need to check if we are inside an object after added refraction
		Vector3d NN = ((float)dot(V, N) >= 0.0) ? N : -N;

		// Calculate new reflection ray
		Vector3d reflDir = mirrorReflect(V, NN);
		reflDir.makeUnitVector();
		Ray newReflRay = Ray(nearestHitRec.p, reflDir);

		return result + nearestHitRec.mat_ptr->k_rg * TraceRay(newReflRay, scene, depth - 1, hasShadow);
	}
	else // Diffuse material or out of depth, return calculated result
	{
		return result;
	}

}