uniform vec4 ltDiffus;
uniform vec4 ltAmbient;
uniform vec4 ltSpecular;
uniform float ltIntensity;
uniform sampler2D sampler0;

varying vec2 Texcoord;
varying vec3 LightDir;
varying vec3 SightLineDir;
varying vec3 Normal;

void main( void )
{
	vec3 vLightDir = normalize(LightDir);
	vec3 vNormal = normalize(Normal);

	float fNDotL           = dot( vNormal, vLightDir ); 

	vec3  fvReflection     = normalize( ( 2.0 * fNDotL* vNormal) - vLightDir );

	vec3 vSightLineDir = normalize(SightLineDir);
	float fRDotV           = max( 0.0, dot( fvReflection, vSightLineDir ) );

	vec4  fvBaseColor      = texture2D( sampler0, Texcoord );
	vec4  fvTotalAmbient   = ltAmbient * fvBaseColor; 
	vec4  fvTotalDiffuse   = ltDiffus * fNDotL * fvBaseColor; 
	vec4  fvTotalSpecular  = ltSpecular * ( pow( fRDotV, 20. ) );
  
	gl_FragColor = (fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular);
       
}