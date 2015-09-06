#version 330 core
#define M_PI 3.1415926535897932384626433832795

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float AN;
uniform float BN;


in vec3 in_Position;
in vec3 in_VertexN;

in vec3 instancePosition;

out vec3 fragVert;
out vec3 normal;
out vec3 fragColor;

float wnoise(vec3 pos, vec3 n, float a, float b, float c) {
	vec3 light_position = vec3(4.0, 4.0, 4.0);
    vec3 lightRay = light_position - pos;

    float alpha = dot(n, lightRay) / (length(lightRay) * length(n));
    alpha = clamp(alpha, 0, 1);

	return a * sin( (alpha * 2 * M_PI + c) * b );
}

vec3 waterRing(vec3 pos, vec3 n, float x) {
	mat4	tr = mat4( vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(n * (x / 50.0), 1.0) );
	vec3	v = vec3( tr * vec4(pos, 1.0) );
	return v;
}

vec3	instanceMove(vec3 pos, vec3 trans)
{
	vec3	v = pos + trans;
	return v;
}

void main() {
	float a = 2.0;
	float coln = gl_InstanceID;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    normal = normalize(normalMatrix * in_VertexN);

	fragVert  = vec3(M * vec4(in_Position, 1.0));

	float n = wnoise(fragVert, normal, a, BN, AN);

//	fragVert = waterRing(fragVert, normal, n);
//	vec3 trans = vec3(coln - 16, 0.0, 0.0);
	fragVert = instanceMove(fragVert, instancePosition);

//	float coln = ( (n + a) / 2 ) / a;
//	fragColor = vec3( (0.027 + coln) / 2 , (0.651 + coln) / 2 , (0.859 + coln) / 2 );
//float cr = ( sin( (coln / 32) * 2 * M_PI ) + 1 ) / 2;
//float cg = ( cos( (coln / 32) * 2 * M_PI ) + 1 ) / 2;
//	fragColor = vec3(cr, 0.5, cg);

	float cr = float(((gl_InstanceID % 1600) % 40)) / 40.0 + 0.2;
	float cb = float(((gl_InstanceID % 1600) / 40)) / 40.0 + 0.2;
	float cg = float((gl_InstanceID / 1600)) / 40.0 + 0.2;
	fragColor = vec3(cr, cg, cb);
	gl_Position = P * V * vec4(fragVert, 1.0);
}
