//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : e0025203@u.nus.edu
// COMMENTS TO GRADER:
// -
//
//============================================================
//
// FILE: assign2.frag

// The GL_EXT_gpu_shader4 extension extends GLSL 1.10 with 
// 32-bit integer (int) representation, integer bitwise operators, 
// and the modulus operator (%).

#extension GL_EXT_gpu_shader4 : require
#extension GL_ARB_texture_rectangle : require

uniform sampler2DRect InputTex;  // The input texture.

uniform int TexWidth;   // Always an even number.
uniform int TexHeight;
uniform int PassCount;  // For the very first pass, PassCount == 0.


void main()
{
    float P1 = texture2DRect( InputTex, gl_FragCoord.xy ).a; 
    float P2;

	int col = int( gl_FragCoord.x );
	int row = int( gl_FragCoord.y );

    if ( PassCount % 2 == 0 )  // PassCount is Even.
    {
		
		// Get the second value

		if (col%2 == 0) // Compare with the one in front
		{
			vec2 lookupVec = vec2(gl_FragCoord.x + 1.0f, gl_FragCoord.y );
			P2 = texture2DRect( InputTex, lookupVec ).a;

			// Compare and switch if necessary
			if (P2 < P1)
				gl_FragColor = vec4(P2);
			else
				gl_FragColor = vec4(P1);
		}
		else // Compare with the one before
		{
			vec2 lookupVec = vec2(gl_FragCoord.x - 1.0f, gl_FragCoord.y );
			P2 = texture2DRect( InputTex, lookupVec ).a;

			// Compare and switch if necessary
			if (P2 > P1)
				gl_FragColor = vec4(P2);
			else
				gl_FragColor = vec4(P1);
		}	
    }

    else  // PassCount is Odd.
    {

        int index1D = row * TexWidth + col;

		// Skip the first and last value
		if(index1D == 0 || index1D == TexWidth*TexHeight-1)
			gl_FragColor = vec4(P1);
		
		else if (col%2 == 0) // Compare with the one before
		{
			int compareID = index1D-1;
			vec2 lookupVec = vec2(float(compareID % TexWidth) + 0.5f, float(compareID / TexWidth) + 0.5f);
			P2 = texture2DRect( InputTex, lookupVec).a;

			if (P2 > P1)
				gl_FragColor = vec4(P2);
			else
				gl_FragColor = vec4(P1);
		}
		else // Compare with the one in front
		{
			int compareID = index1D+1;
			vec2 lookupVec = vec2(float(compareID % TexWidth) + 0.5f, float(compareID / TexWidth) + 0.5f);
			P2 = texture2DRect( InputTex, lookupVec).a;

			if (P2 < P1)
				gl_FragColor = vec4(P2);
			else
				gl_FragColor = vec4(P1);
		}
    }
}