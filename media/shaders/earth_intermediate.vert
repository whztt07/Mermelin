// Intermediate "Earth" vertex shader, 2012
// Patrick Joos, Eduardo Hahn Paredes
#version 150 compatibility

uniform vec3 light;
uniform vec3 camera;
  
// vertex attributes
in vec3 tangent;
in vec3 binormal;
in vec4 vertex;
in vec3 normal;

out vec2 texCoord;
out vec3 viewDirection;
out vec3 lightDirection;
              
void main(void)
{
    gl_Position = ftransform();
    texCoord = gl_MultiTexCoord0.st;
   
    vec3 n = gl_NormalMatrix * normal;
    vec3 t = gl_NormalMatrix * tangent;
    vec3 b = gl_NormalMatrix * binormal;

    vec3 object = vec3(vertex);
    mat3 rotation = mat3(t, b, n); 
   
    lightDirection = rotation * (light - object); 
    viewDirection = rotation * (camera - object);
}