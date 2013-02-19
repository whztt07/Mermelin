// Intermediate "Air" fragment shader, 2012
// Patrick Joos, Eduardo Hahn Paredes
#version 150 compatibility

uniform samplerCube cubeMap;
uniform sampler2D colorMap;
uniform float time;

void main (void)
{
    float reflectivity = max(abs(sin(time)) / 1.5, 0.5);
	vec3 baseColor = texture2D(colorMap, gl_TexCoord[0].xy).rgb;
	vec3 cubeColor = textureCube(cubeMap, gl_TexCoord[1].xyz).rgb;
	gl_FragColor = vec4(mix(baseColor, cubeColor, reflectivity), 0.6);
}