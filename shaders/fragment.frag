#version 150
out vec4 color;
in vec3 color_frag;
void main()
{
    color = vec4(color_frag, 1.0);
}
