// Basic "Fire" fragment shader, 2013
// Eduardo Hahn Paredes
#version 110

uniform vec4 startColor;
uniform vec4 endColor;
uniform float time;

varying float noise;
varying vec4 color;

void main()
{
	float lerpValue = abs(sin(time) + noise);
	gl_FragColor = mix(startColor, endColor, lerpValue);
}
