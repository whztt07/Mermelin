// Basic "Rotation" vertex shader, 2012
// Patrick Joos, Eduardo Hahn Paredes
#version 110

uniform float time;
uniform int axis;
uniform int speed;

varying vec3 normal;
varying vec3 position;
varying vec4 texCoord;

vec4 rotate( float angle, vec4 v)
{	
	float sa = sin( angle );
	float ca = cos( angle );

	if (axis == 0) {
		return vec4( sa*v.z + ca*v.x, v.y, ca*v.z - sa*v.x, v.w );
	}
	if (axis == 1) {
		return vec4( ca*v.x - sa*v.y, sa*v.x + ca*v.y, v.z, v.w );
	}
	if (axis == 2) {
		return vec4( v.x, ca*v.y - sa*v.z, sa*v.y + ca*v.z, v.w );
	}
}

void main( void )
{
	gl_FrontColor = gl_Color;
	texCoord = gl_MultiTexCoord0;
	
	// rotate vertex and normal
	vec4 vertex = rotate(time * speed, gl_Vertex);
	vec3 N = vec3(rotate(time * speed, vec4(gl_Normal,1.0)));

	// now apply the application provided transformations
	normal = gl_NormalMatrix * N;
	position = vec3( gl_ModelViewMatrix * vertex );

	// rotate the model
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}

