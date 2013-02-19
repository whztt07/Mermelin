//Source: http://irrlicht.sourceforge.net/forum//viewtopic.php?t=21057
#version 150 compatibility

out vec2 Texcoord;   
out vec4 Pos;   

void main( void )   
{   
    gl_Position = ftransform();   
    Pos = gl_Vertex;   
    Texcoord = gl_MultiTexCoord0.xy;   
} 