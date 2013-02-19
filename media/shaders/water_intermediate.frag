/**
This shader is based on GPU Gems, Chapter2: Rendering Water Caustics
http://http.developer.nvidia.com/GPUGems/gpugems_ch02.html


It was ported over from CG to GLSL, with some fixes
Author: Patrick Joos, Eduardo Hahn Paredes
*/


#version 150 compatibility

#define VTXSIZE 0.01f   // Amplitude
#define WAVESIZE 5.0f  // Frequency

#define FACTOR 2.0f
#define SPEED 2.0f
#define OCTAVES 2

// 'time' contains seconds since the program was linked.
uniform float time;
uniform sampler2D LightMap;
uniform sampler2D GroundMap;

vec3 intercept;
in vec4 vert;

out vec4 colour;

vec2 gradwave(float x, float y, float timer)
{
  float dZx = 0.0f;
  float dZy = 0.0f;
  float octaves = OCTAVES;
  float factor = FACTOR;
  float d = sqrt(x * x + y * y);

  do {
    dZx += d * sin(timer * SPEED + (1/factor) * x * y * WAVESIZE) *
             y * WAVESIZE - factor *
             cos(timer * SPEED + (1/factor) * x * y * WAVESIZE) * x/d;
    dZy += d * sin(timer * SPEED + (1/factor) * x * y * WAVESIZE) *
             x * WAVESIZE - factor *
             cos(timer * SPEED + (1/factor) * x * y * WAVESIZE) * y/d;
			 
    factor = factor/2;
    octaves--;
  } while (octaves > 0);

  return vec2(2 * VTXSIZE * dZx, 2 * VTXSIZE * dZy);
}

vec3 line_plane_intercept(vec3 lineP, vec3 lineN, vec3 planeN, float  planeD)
{  
	float distance = (planeD - lineP.z) / lineN.z;
  	return lineP + lineN * distance;
}

void main( void )
{

	vec2 dxdy = gradwave(vert.x, vert.y, time);
	vec4 vertex = vert;
	
	vertex.w = clamp(vertex.w, 0.0, 1.0);
	
	vec3 saturated = vec3(dxdy.x, dxdy.y, vertex.w);

	vec3 intercept = line_plane_intercept(vertex.xyz, saturated, vec3(0, 0, 1), -0.8);

  	colour = texture(LightMap, intercept.xy * 0.8);
	//colour.rgb = texture2D(intercept.xy * 0.8, LightMap);
  	colour += texture(GroundMap, vertex.xy);
	//colour.rgb = texture2D(vertex.xy, GroundMap);
  	colour.a = 1;
}
