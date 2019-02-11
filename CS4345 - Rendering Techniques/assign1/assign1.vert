//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : e0025203@u.nus.edu
// COMMENTS TO GRADER:
// -
//
//============================================================
// FILE: assign1.vert


varying vec3 ecPosition; // Vertex's position in eye space.
varying vec3 ecNormal;   // Vertex's normal vector in eye space.
varying vec3 ecTangent;  // Vertex's tangent vector in eye space.

attribute vec3 Tangent;  // Input vertex's tangent vector in model space.


void main( void )
{

	// Transform Tangent, Normal and Position to eye space
	ecTangent = normalize(gl_NormalMatrix * Tangent);

	ecNormal = normalize(gl_NormalMatrix * gl_Normal);

	vec4 ecPosition4 = gl_ModelViewMatrix * gl_Vertex;
	ecPosition = vec3( ecPosition4 ) / ecPosition4.w;

	// Get tex coordinates and set vertex position in clip/screen space (pos = ftransform = MVP*vertex)
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();

}
