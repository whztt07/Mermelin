// Basic "Air" vertex shader, 2013
// Eduardo Hahn Paredes
#version 110

uniform float time;

mat4 rotationMatrix(float angle){
    return mat4(cos(angle),     -sin(angle),    0.0,    0.0,
                sin(angle ),    cos(angle ),    0.0,    0.0,
                0.0,            0.0,            1.0,    0.0,
                0.0,            0.0,            0.0,    1.0);
}

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord0 * rotationMatrix(time);
	gl_TexCoord[2] = gl_MultiTexCoord0 * rotationMatrix(time * 2.0);
}
