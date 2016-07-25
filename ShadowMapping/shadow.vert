
varying vec2 Texcoord0;
varying vec2 Texcoord3;
varying vec4 ShadowCoord;

void main( void )
{
	gl_Position = ftransform();
	Texcoord0 = gl_MultiTexCoord0.xy;
	Texcoord3 = gl_MultiTexCoord3.xy;
	//ShadowCoord = gl_TextureMatrix[3] * gl_Vertex;
}