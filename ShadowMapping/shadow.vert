
varying vec2 Texcoord;
varying vec4 ShadowCoord;
varying vec4 vertColor;
uniform mat4 ShadowVP;
void main( void )
{
	gl_Position = ftransform();
	vertColor = gl_Color;
	Texcoord = gl_MultiTexCoord0.xy;
	ShadowCoord = ShadowVP * gl_Vertex;
}