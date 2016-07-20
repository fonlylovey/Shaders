
varying vec2 Texcoord;
varying vec3 LightDir;
varying vec3 SightLineDir;
varying vec3 Normal;

void main( void )
{
	vec3 vLightDir = normalize(LightDir);
	vec3 vNormal = normalize(Normal);
  
	gl_FragColor = texture2D( sampler0, Texcoord );
       
}