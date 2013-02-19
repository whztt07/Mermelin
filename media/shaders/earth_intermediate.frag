// Intermediate "Earth" fragment shader, 2012
// Patrick Joos, Eduardo Hahn Paredes
#version 150 compatibility

uniform sampler2D dirt_color;
uniform sampler2D dirt_normal;

in vec2 texCoord;
in vec3 viewDirection;
in vec3 lightDirection;

void main( void )
{
    float shininess = 2.0;

    vec3 lightDir = normalize(lightDirection);
    vec3 normal = normalize((texture2D(dirt_normal, texCoord).xyz * 2.0) - 1.0 );
    float dotLight = dot(normal, lightDir); 
   
    vec3 reflection = normalize((normal * dotLight) - lightDir); 
    vec3 viewDir = normalize(viewDirection);
    float dotView = max(0.0, dot(reflection, viewDir));
   
    vec4 color = texture2D(dirt_color, texCoord);
   
    vec4 ambient = gl_LightSource[0].ambient;
    vec4 diffuse = gl_LightSource[0].diffuse * dotLight; 
    vec4 specular = gl_LightSource[0].specular * (pow(dotView, shininess));
  
    gl_FragColor =  color * clamp(ambient + diffuse + specular, 0.0, 1.0) ;
}