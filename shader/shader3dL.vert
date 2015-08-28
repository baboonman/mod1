#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 in_Position;
in vec3 in_VertexN;

// Output data ; will be interpolated for each fragment.
out vec3 MaterialDiffuseColor;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightPosition_worldspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
//uniform vec3 LightPosition_worldspace;

float snoise(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){

	LightPosition_worldspace = vec3(4, 4, 4);

	gl_Position =  MVP * vec4(in_Position,1);
	float n = snoise(vec2(in_Position.x, in_Position.y));
//	MaterialDiffuseColor = vec3((( 1.0f ) / 2 + n) / 2 , ((1.0f + 1) / 2 + n) / 2 , (( 0.9 + 1) / 2 + n) / 2 );
	MaterialDiffuseColor = vec3(((in_Position.x ) / 2 + n) / 2 , ((in_Position.y + 1) / 2 + n) / 2 , ((in_Position.z + 1) / 2 + n) / 2 );

	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(in_Position,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(in_Position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(in_VertexN,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
}

