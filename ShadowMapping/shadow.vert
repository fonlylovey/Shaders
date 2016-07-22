varying vec2 Texcoord;
  
void main( void )
{
   gl_Position = ftransform();
   Texcoord = gl_MultiTexCoord0.xy;
    
   vec4 viewPos = gl_ModelViewMatrix * gl_Vertex;
   gl_ModelViewProjectionMatrix * gl_Vertex; 
}