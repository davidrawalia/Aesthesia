#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light;
uniform float ambLight;
uniform vec3 color;
uniform float textureBool;
uniform mat4 lightSpaceMatrix;

out vec3 fragPosition;
out vec3 outNormal;
out vec3 outColor;
out vec3 outLight;
out float outAmbLight;
out vec2 outTexCoord;
out float textureBoolOut;
out vec4 FragPosLightSpace;

void main() {
	gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 
												   1.0);
	outTexCoord = vec2(texCoord.x, 1 - texCoord.y);

	outNormal = vec3(model * vec4(normal, 0.0));
	fragPosition = vec3(model * vec4(position, 1.0));
	outColor = color;
	outAmbLight = ambLight;
	outLight = light;
	textureBoolOut = textureBool;
	FragPosLightSpace = lightSpaceMatrix * vec4(fragPosition, 1.0);
}