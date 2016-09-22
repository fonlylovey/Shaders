
varying vec2 Texcoord;

varying vec4 ShadowCoord;

uniform sampler2D baseMap;
uniform sampler2D shadowMap;

uniform vec4 shadowColor;

void main()
{
	vec4 color = texture2D(baseMap, Texcoord);

	vec4 sCoord = ShadowCoord / ShadowCoord.w;
	sCoord.z -= 0.0005;
	float disLight = texture2D(shadowMap, sCoord.st).z;

	float shadow = 1.0;
	if (ShadowCoord.w > 0.0)
	{
		shadow = disLight < sCoord.z ? 0.5 : 1.0;
	}

	gl_FragColor = shadow * color;
}