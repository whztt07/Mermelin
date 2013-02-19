// Basic "Water" vertex shader, 2012
// Eduardo Hahn Paredes
#version 110

uniform float time;

varying vec2 waveSpeed;
varying vec3 vTexCoord;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;

	gl_TexCoord[0].x += time * waveSpeed.x;
	gl_TexCoord[0].y += time * waveSpeed.y;
	gl_TexCoord[1].x -= time * waveSpeed.x;
	gl_TexCoord[1].y -= time * waveSpeed.y;

	gl_Position = ftransform();
}
