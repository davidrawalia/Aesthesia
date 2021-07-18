#version 330 core
uniform vec3 cameraPosition;
uniform sampler2D textureMap;
uniform sampler2D shadowMap;

in vec3 outColor;
in vec3 fragPosition;
in vec3 outNormal;
in vec3 outLight;
in float outAmbLight;
in vec2 outTexCoord;
in float textureBoolOut;
in vec4 FragPosLightSpace;

out vec4 color;

void main()
{
	vec4 meshColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textureBoolOut > 0) {
		meshColour = texture(textureMap, outTexCoord);
	}
	else {
		meshColour = vec4(outColor, 1.0f);
	}
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
	float specularStrength = 0.5;
	float reflectionCoefficient = 0.9f;
	vec3 amb_contribution = outAmbLight * lightColor;

	vec3 norm = normalize(outNormal);
	vec3 light_direction = normalize(outLight - fragPosition);
	float degree = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse_contribution = degree * lightColor;

	vec3 viewDir = normalize(cameraPosition - fragPosition);
	vec3 reflectDir = reflect(-light_direction, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular_contribution = specularStrength * spec * specularColor;

	vec4 result_color = vec4(amb_contribution + (diffuse_contribution + specular_contribution), 1.0);


	color = result_color * meshColour, 1.0;

}
