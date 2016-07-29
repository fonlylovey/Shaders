uniform sampler2D baseMap;

varying vec2 Texcoord;

void main(void)
{
	vec4 baseColor = texture2D(baseMap, Texcoord);
	gl_FragColor = vec4(baseColor.xyz, 0.8); // 
}