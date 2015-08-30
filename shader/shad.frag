#version 330 core
#define M_PI 3.1415926535897932384626433832795

in vec3 fragVert;
in vec3 normal;
in vec3	fragColor;

out vec4 finalColor;

void main()
{
	vec3 light_position = vec3(4.0, 4.0, 4.0);
	vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 lightRay = light_position - fragVert;

    float brightness = dot(normal, lightRay) / (length(lightRay) * length(normal));
    brightness = clamp(brightness, 0, 1);

    if (brightness < 0.5f)
        brightness = 0.5f;
    finalColor = vec4(brightness * light_color * fragColor, 1.0);
}
