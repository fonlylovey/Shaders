varying vec2 Texcoord;
varying vec4 vertColor;
varying vec4 ShadowCoord;
uniform sampler2D baseMap;
uniform sampler2DShadow shadowMap;
uniform vec4 shadowColor;
void main(void)
{
	vec4 color = texture2D(baseMap, Texcoord);
	if (color.x < 0.0001)
	{
		color = vertColor;
	}
	
	vec4 sCoord = ShadowCoord / ShadowCoord.w;
	sCoord.z += 0.0005;
	float disLight = texture2D(baseMap, sCoord.st).z;
	float shadow = 1.0;
	if (ShadowCoord.w > 0.0)
		shadow = disLight < sCoord.z ? 0.5 : 1.0;
	gl_FragColor = color * (0.5 + shadow2DProj(shadowMap, gl_TexCoord[0]) * 0.5);
}