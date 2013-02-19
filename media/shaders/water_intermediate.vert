/**
This shader is based on GPU Gems, Chapter2: Rendering Water Caustics
http://http.developer.nvidia.com/GPUGems/gpugems_ch02.html


It was ported over from CG to GLSL, with some fixes
Author: Patrick Joos, Eduardo Hahn Paredes
*/

#version 150 compatibility

out vec4 vert;

void main( void )
{
	// Generate a normal (line direction) from the gradient
   	// of the wave function and intercept with the water plane.
   	// We use screen-space z to attenuate the effect to avoid aliasing.
	vert = ftransform();
	gl_Position = vert;	
}