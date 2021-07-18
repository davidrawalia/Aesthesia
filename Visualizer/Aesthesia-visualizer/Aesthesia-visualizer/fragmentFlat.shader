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
	vec4 meshColour = texture(textureMap, outTexCoord);
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
	result_color = result_color * 0.85;
	result_color = clamp(result_color, 0.0, 1.0);
	result_color = vec4((floor(result_color.x*10.0))/10.0, (floor(result_color.y*10.0))/10.0, (floor(result_color.z*10.0))/10.0, 1.0);


	if (result_color.x == 1.0) {
		color = vec4(meshColour.r * 0.5, meshColour.g * 0.5, meshColour.b * 0.5, 1.0) + vec4(0.5, 0.5, 0.5, 1.0);
	}
	else if (result_color.x > 0.9) {
		color = vec4(meshColour.r * 0.75, meshColour.g * 0.75, meshColour.b * 0.75, 1.0) + vec4(0.25, 0.25, 0.25, 1.0);
	}
	else if (result_color.x > 0.4) {
		color = meshColour;
	}
	else if (result_color.x > 0.2) {
		color = vec4(meshColour.r * 0.75, meshColour.g * 0.75, meshColour.b * 0.75, 1.0);
	}
	else {
		color = vec4(meshColour.r * 0.5, meshColour.g * 0.5, meshColour.b * 0.5, 1.0);
	}


//	color = (meshColour + result_color)/2.0;


}
