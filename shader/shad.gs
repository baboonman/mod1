#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 fragVert[];
out vec3 normal[];
out vec3 fragColor[];

out vec3 fragVertT;
out vec3 normalT;
out vec3 fragColorT;

void main() {
	fragVertT = fragVert[0];
	normalT = normal[0];
	fragColorT = fragColor[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	fragVertT = fragVert[1];
	normalT = normal[1];
	fragColorT = fragColor[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	fragVertT = fragVert[2];
	normalT = normal[2];
	fragColorT = fragColor[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
