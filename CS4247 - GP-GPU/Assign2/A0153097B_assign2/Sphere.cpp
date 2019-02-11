//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : E0025203@u.nus.edu
// COMMENTS TO GRADER:
// - I included an additional library, I hope that is okay!
//
// ============================================================
//
// FILE: Sphere.cpp


#include <cmath>
#include "Sphere.h"
#include <algorithm>

using namespace std;


bool Sphere::hit( const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec ) const 
{
	//***********************************************
    //***********		MODIFIED		*************
    //***********************************************

	// Transform ray to sphere's local space
	Vector3d rayOrigin = r.origin() - center;

	double a = dot(r.direction(), r.direction()); // = 1
	double b = 2 * dot(r.direction(), rayOrigin);
	double c = dot(rayOrigin, rayOrigin) - (radius*radius);
	double d = (b*b) - (4 * a*c);
	
	// If discriminant is negative we only have imaginary solutions => not relevant
	if (d < 0)
		return false;

	// Calculate t+ and t-
	double t_p = (-b + sqrt(d)) / (2*a);
	double t_m = (-b - sqrt(d)) / (2*a);
	
	// Chose closest positive t
	double t;
	if (t_p >= tmin && t_m >= tmin) // Both over threshold, choose minimum
	{
		t = min(t_p, t_m);
	}
	else if(t_p >= tmin) // Implies t- is under threshold
	{
		t = t_p;
	}
	else if (t_m >= tmin) // Implies t+ is under threshold
	{
		t = t_m;
	}
	else // Both below threshold
	{
		return false;
	}

	// Set values if we are inside upper threshold
	if (t <= tmax)
	{
		rec.t = t;
		rec.p = r.pointAtParam(t);
		Vector3d localP = rec.p - center; // Need to transform normals as well
		rec.normal = localP.unitVector(); // P(t)/|P(t)| = unitVector(p)
		rec.mat_ptr = matp;
		return true;
	}
	return false;

}




bool Sphere::shadowHit( const Ray &r, double tmin, double tmax ) const 
{
	//***********************************************
	//***********		MODIFIED		*************
	//***********************************************

	// Transform ray to sphere's local space
	Vector3d rayOrigin = r.origin() - center;

	double a = 1; // Simplified
	double b = 2 * dot(r.direction(), rayOrigin);
	double c = dot(rayOrigin, rayOrigin) - (radius*radius);
	double d = (b*b) - (4 * a*c);

	// If discriminant is negative we only have imaginary solutions => not relevant
	if (d < 0)
		return false;

	// Calculate t+ and t-
	double t_p = (-b + sqrt(d)) / (2 * a);
	double t_m = (-b - sqrt(d)) / (2 * a);

	// Chose closest positive t
	double t;
	if (t_p >= tmin && t_m >= tmin) // Both over threshold, choose minimum
	{
		t = min(t_p, t_m);
	}
	else if (t_p >= tmin) // Implies t- is under threshold
	{
		t = t_p;
	}
	else if (t_m >= tmin) // Implies t+ is under threshold
	{
		t = t_m;
	}
	else // Both below threshold
	{
		return false;
	}

	// Return true if we are inside upper threshold 
	return (t <= tmax);

}

