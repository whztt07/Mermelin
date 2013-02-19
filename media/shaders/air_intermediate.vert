// Intermediate "air" fragment shader, 2012
// Patrick Joos, Eduardo Hahn Paredes
#version 150 compatibility

uniform mat4 world;
uniform vec3 camera;
			   
void main()
{	
	vec4 worldSpace = world * gl_Vertex;
	vec3 worldSpaceNormal = normalize(mat3(world) * gl_Normal );
	vec3 eyeSpace = normalize(worldSpace.xyz - camera.xyz );
	
    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1].xyz = reflect(eyeSpace, worldSpaceNormal);

    gl_Position = ftransform();
}
