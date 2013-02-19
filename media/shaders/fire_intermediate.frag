#version 150 compatibility

uniform sampler2D Texture1;   
uniform sampler2D Texture2;   
uniform float time;
uniform float modifier;
   
in vec2 Texcoord;   
in vec4 Pos;   

void main( void )   
{   
	vec4 noise = texture2D(Texture1, Texcoord);   
	vec2 T1 = Texcoord + vec2(1.5, -1.5) * time * 2 * modifier;   
	vec2 T2 = Texcoord + vec2(-0.5, 2.0) * time * modifier; 
	
	T1.x += (noise.x) * 2.0;   
	T1.y += (noise.y) * 2.0;   
	T2.x -= (noise.y) * 0.2;   
	T2.y += (noise.z) * 0.2;   
	
	float p = texture2D(Texture1, T1 * 2.0).a;     
	vec4 color = texture2D(Texture2, T2 * 2.0);    
	vec4 temp = color * (vec4(p,p,p,p) * 2.0) + (color * color - 0.1);   
	
	if(temp.r > 1.0)
	{
		temp.bg+=clamp(temp.r-2.0,0.0,100.0);
	}   
	if(temp.g > 1.0)
	{
		temp.rb+=temp.g-1.0;
	}   
	if(temp.b > 1.0)
	{
		temp.rg+=temp.b-1.0;
	}   
    
	gl_FragColor = temp;   
} 