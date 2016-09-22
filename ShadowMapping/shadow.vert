
varying vec2 Texcoord;
varying vec4 ShadowCoord;
uniform mat4 ShadowVP;
uniform mat4 osg_ViewMatrixInverse;

void main( )
{
	mat4 modelMatrix =  osg_ViewMatrixInverse * gl_ModelViewMatrix;
	vec4 posWorld = modelMatrix * gl_Vertex;

	ShadowCoord = gl_TextureMatrix[4] * posWorld;
	Texcoord = gl_MultiTexCoord0.xy;

	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}