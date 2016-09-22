//#version 330 core

uniform sampler2D depTex;
uniform sampler2D sceneTex;

varying vec4 shadowCoord;
varying vec2 uv;

void main()
{

	vec4 shadowCoordWdivide = shadowCoord/shadowCoord.w;

	shadowCoordWdivide.z -= 0.0005;

	float depth = texture2D(depTex, shadowCoordWdivide.st).z;

	float shadow = 1.0;
	if(shadowCoord.w > 0)
	{
		shadow = depth < shadowCoordWdivide.z ? 0.5 : 1.0;
	}

	gl_FragColor = shadow*texture2D(sceneTex, uv);

		
		
}
