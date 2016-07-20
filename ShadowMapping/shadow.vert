uniform vec3 ltPos;
uniform vec3 eyePos;

uniform mat4 lightViewMatrix;
uniform mat4 lightProjectmatrix;

varying vec2 Texcoord;
varying vec3 LightDir;
varying vec3 SightLineDir;
varying vec3 Normal;
   
void main( void )
{
   gl_Position = ftransform();
   Texcoord = gl_MultiTexCoord0.xy;
    
   vec4 viewPos = gl_ModelViewMatrix * gl_Vertex;
   
   LightDir = ltPos - viewPos.xyz;
   SightLineDir  = eyePos - viewPos.xyz;
   Normal = gl_NormalMatrix * gl_Normal;
   
}