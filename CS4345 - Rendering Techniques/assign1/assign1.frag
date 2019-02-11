//============================================================
// STUDENT NAME: Adam Alsegard
// MATRIC NO.  : A0153097B
// NUS EMAIL   : e0025203@u.nus.edu
// COMMENTS TO GRADER:
// 
//
//============================================================
//
// FILE: assign1.frag


//============================================================================
// Eye-space position and vectors for setting up a tangent space at the fragment.
//============================================================================

varying vec3 ecPosition;    // Fragment's 3D position in eye space.
varying vec3 ecNormal;      // Fragment's normal vector in eye space.
varying vec3 ecTangent;     // Frgament's tangent vector in eye space.


//============================================================================
// TileDensity specifies the number of tiles to span across each dimension when the
// texture coordinates gl_TexCoord[0].s and gl_TexCoord[0].t range from 0.0 to 1.0.
//============================================================================

uniform float TileDensity;  // (0.0, inf)


//============================================================================
// TubeRadius is the radius of the semi-circular mirror tubes that run along 
// the boundary of each tile. The radius is relative to the tile size, which 
// is considered to be 1.0 x 1.0.
//============================================================================

uniform float TubeRadius;  // (0.0, 0.5]


//============================================================================
// StickerWidth is the width of the square sticker. The entire square sticker 
// must appear at the center of each tile. The width is relative to the 
// tile size, which is considered to be 1.0 x 1.0.
//============================================================================

uniform float StickerWidth;  // (0.0, 1.0]


//============================================================================
// EnvMap references the environment cubemap for reflection mapping.
//============================================================================

uniform samplerCube EnvMap;


//============================================================================
// DiffuseTex1 references the wood texture map whose color is used to 
// modulate the ambient and diffuse lighting components on the non-mirror and
// non-sticker regions.
//============================================================================

uniform sampler2D DiffuseTex1;


//============================================================================
// DiffuseTex2 references the sticker texture map whose color is used to 
// modulate the ambient and diffuse lighting components on the sticker regions.
//============================================================================

uniform sampler2D DiffuseTex2;




void main()
{
	// Some useful eye-space vectors.
    vec3 ecNNormal = normalize( ecNormal );
    vec3 ecViewVec = -normalize( ecPosition );

    vec2 c = TileDensity * gl_TexCoord[0].st;
    vec2 p = fract( c ) - vec2( 0.5 );
	/*------------------------------------------*/

	float TubeStartPos = 0.5-TubeRadius;

	bool notBumpRegion = (abs(p.x) < TubeStartPos && abs(p.y) < TubeStartPos)? true : false;
	
	bool backfacing = (dot(ecViewVec, ecNNormal) < 0.0 )? true : false;

    if ( backfacing || notBumpRegion)
    {
        //======================================================================
        // In here, fragment is backfacing or in the non-bump region.
        //======================================================================

        // For the lighting computation, use the half-vector approach 
        // to compute the specular component.

		//Blinn-Phong Shading (Half-vector instead of reflection)
		vec3 ecLightPos = vec3( gl_LightSource[0].position ) / gl_LightSource[0].position.w;
		vec3 ecLightVec = normalize( ecLightPos - ecPosition );
		vec3 halfVector = normalize( ecLightVec + ecViewVec );

		if (backfacing) ecNNormal = -ecNNormal; //Because we're on the inside in that case
        float dotNL = max( 0.0, dot( ecNNormal, ecLightVec ) );
		float dotNH = max( 0.0, dot( ecNNormal, halfVector ) );

		vec4 woodTex = texture2D(DiffuseTex1, gl_TexCoord[0].st);

		float pf = ( dotNH == 0.0 )? 0.0 : pow( dotNH, gl_FrontMaterial.shininess );


		//Sticker texture - look up from coordinates inside the tiles
		bool stickerPlace = (abs(p.x) < StickerWidth/2 && abs(p.y) < StickerWidth/2)? true : false;
		vec2 stickerST = (p / StickerWidth) + vec2(0.5);
		vec4 stickerTex = texture2D(DiffuseTex2, stickerST);

		// Render stickers
		if(stickerPlace && !backfacing)
		{
			gl_FragColor = gl_FrontLightModelProduct.sceneColor * stickerTex +
						gl_LightSource[0].ambient * gl_FrontMaterial.ambient * stickerTex  +
						gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * stickerTex * dotNL +
						gl_LightSource[0].specular * gl_FrontMaterial.specular * pf;
		}

		else //Wooden
		{
			gl_FragColor = gl_FrontLightModelProduct.sceneColor * woodTex +
						gl_LightSource[0].ambient * gl_FrontMaterial.ambient * woodTex  +
						gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * woodTex * dotNL +
						gl_LightSource[0].specular * gl_FrontMaterial.specular * pf;
		}
	
	}
    else
    {
        //======================================================================
        // In here, fragment is front-facing and in the mirror-like bump region.
        //======================================================================

        vec3 N = ecNNormal;
        vec3 B = normalize( cross( N, ecTangent ) );
        vec3 T = cross( B, N ); //Q: Why not use T = normalize(ecTangent)? 

        vec3 tanPerturbedNormal;  // The perturbed normal vector in tangent space of fragment.
        vec3 ecPerturbedNormal;   // The perturbed normal vector in eye space.
        vec3 ecReflectVec;        // The mirror reflection vector in eye space.
		/*-------------------------------------------*/

		//Procedural Bump mapping

		// Matrix is defined column-wise (i.e. it looks like the transpose)
		mat3 ec2tan = mat3( T.x, B.x, N.x,
							T.y, B.y, N.y,
							T.z, B.z, N.z);


		vec3 tanViewDir = normalize(ec2tan * ecPosition);
		tanPerturbedNormal = normalize(ec2tan * ecNNormal); //Not sure what I need this for though...

		// Circle equation: x^2 + y^2 = r^2 
		float squareRadius = pow(TubeRadius, 2);
		vec2 XY = vec2(0.5) - abs(p);

		if (abs(p.x) > abs(p.y))  // We're on the sides (Left-Right)
		{ 
			//We need to switch sides for position in order to have correct sign
			float xPos = (p.x > 0) ? p.x - 0.5 : p.x + 0.5; 
			float squareX = pow(XY.x, 2); 
			ecPerturbedNormal = normalize(vec3(xPos, 0, sqrt(squareRadius - squareX)));
		}
		else // Top-Down
		{
			float yPos = (p.y > 0) ? p.y - 0.5 : p.y + 0.5;
			float squareY = pow(XY.y, 2); 
			ecPerturbedNormal = normalize(vec3(0, yPos, sqrt(squareRadius - squareY)));
		}

        // Mirror reflection (use incoming vector towards object (i.e. tanViewDir instead of tanViewVec))
		ecReflectVec = reflect( tanViewDir,  ecPerturbedNormal);
		gl_FragColor = texture(EnvMap, ecReflectVec);

    }

}
