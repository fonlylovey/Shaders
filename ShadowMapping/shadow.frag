
uniform sampler2D sampler0;
uniform sampler2DShadow sampler8;

void main(void)
{
	vec4 color = gl_Color * texture2D( sampler0, gl_TexCoord[0].xy );
	gl_FragColor = color * (0.1 + shadow2DProj( sampler8,gl_TexCoord[3] ) * 0.1);
}