
varying vec2 Texcoord0;
varying vec2 Texcoord3;
varying vec4 ShadowCoord;
uniform sampler2D sampler0;
uniform sampler2D sampler3;

void main(void)
{
	/*vec4 shadowCoordinateWdivide  = ShadowCoord / ShadowCoord.w ;
	shadowCoordinateWdivide.z += 0.0005;

	vec4 color = texture2D( sampler0, Texcoord0 );
	float depth = texture2D( sampler3, shadowCoordinateWdivide.st).z;
	float shadow = 1.0;
	if(depth < shadowCoordinateWdivide. z)
	{
		shadow =  0.5;
		gl_FragColor = color;
	}
	else
	{
		gl_FragColor = color;
		vec4(0.2, 0.2, 0.2, 1.0);
	}*/
	vec4 color = texture2D(sampler0, Texcoord0);
	vec4 sampSha = texture2D(sampler3, Texcoord3);
	gl_FragColor = sampSha;
}