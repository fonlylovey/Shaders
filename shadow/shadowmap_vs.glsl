//#version 330 core

uniform mat4 shadowMatrix;
uniform mat4 osg_ViewMatrixInverse;

varying vec4 shadowCoord;
varying vec2 uv;
void main()
{
	mat4 modelMatrix =  osg_ViewMatrixInverse*gl_ModelViewMatrix;
	vec4 posInWorld = modelMatrix * gl_Vertex;

	shadowCoord = gl_TextureMatrix[4] * posInWorld;
	//shadowCoord = shadowMatrix * posInWorld;

	uv = gl_MultiTexCoord0.xy;

	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}