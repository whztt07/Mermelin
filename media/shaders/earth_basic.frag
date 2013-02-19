// Basic "Earth" fragment shader, 2012
// Eduardo Hahn Paredes
#version 110

uniform sampler2D dirt_color;

void main()
{
	gl_FragColor = texture2D(dirt_color, gl_TexCoord[0].st);
}
