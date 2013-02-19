// Basic "Earth" vertex shader, 2012
// Eduardo Hahn Paredes
#version 110

void main()
{	
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position    = ftransform();
}
