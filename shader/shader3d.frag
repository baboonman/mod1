#version 150
in vec4 out_color;
out vec4 ex_FragColor;

float snoise(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main()
{
	float n = snoise(vec2(out_color.x, out_color.y));
	ex_FragColor = vec4(out_color);
}
