
#version 330

in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 vertexNormal_worldspace;
in vec3 eyeDirection_cameraspace;
in vec4 fragmentColor;
in vec2 UV;

out vec4 color;

uniform mat4 MV;
uniform sampler2D myTexture;

void main()
{
	vec3 lightPos = vec3(300, 1000, 0);

	vec3 lightColor = vec3(1, 1, 1);
	float lightPower = 2.0f;

	// Diffuse color
	vec3 normalDir_worldspace = normalize(vertexNormal_worldspace);
	vec3 lightDir = normalize(position_worldspace - lightPos);

	float diffuse = max(0.3, dot(normalDir_worldspace, lightDir));

	vec3 normalDir = normalize(normal_cameraspace);
	vec3 eyeDir = normalize(eyeDirection_cameraspace);
	vec3 reflectDir = reflect(-lightDir, normalDir);

	float specAttenuation = clamp(dot(eyeDir, reflectDir), 0, 1);

	//vec3 specular = vec3(0.3, 0.3, 0.3) * pow(specAttenuation, 5) *  lightPower;

	//color = fragmentColor * diffuse * lightPower;
	// + vec4(specular, 1.0);

	color = clamp(vec4(texture(myTexture, UV).rgb, 1), 0, 1);

	if(color.r == 1.0 && color.g == 1.0 && color.b == 1.0)
	{
		color.a = 0.0;
	}

	else if(color.r == 0.0 && color.g == 0.0 && color.b == 0.0)
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	
}
