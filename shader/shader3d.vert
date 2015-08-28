#version 150

uniform mat4 project;
uniform mat4 modelview;
in vec3 in_Position;
out vec4 out_color;
float snoise(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main()
{
    gl_Position = project * modelview * vec4(in_Position, 1.0);
	float n = snoise(vec2(in_Position.x, in_Position.y));
	out_color = vec4(((in_Position.x ) / 2 + n) / 2 , ((in_Position.y + 1) / 2 + n) / 2 , ((in_Position.z + 1) / 2 + n) / 2 , 1.0);
//	out_color = vec4((( 1.0f ) / 2 + n) / 2 , ((1.0f + 1) / 2 + n) / 2 , (( 0.9 + 1) / 2 + n) / 2 , 1.0);
}
