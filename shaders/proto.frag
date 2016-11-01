#version 330 core

/////////// Light uniforms structs
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/////////// Public
struct Material {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
}

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}

/////////// Material uniforms
//if(hasAmbientMap) {
sampler2D ambient;
//} else {
uniform vec3 ka;
//}

//if(hasDiffuseMap) {
sampler2D diffuse;
//} else {
uniform vec3 kd;
//}

//if(hasSpecularMap){
sampler2D specular;
//} else {
uniform vec3 ks;
//}
uniform vec3 shininess;

/////////// Light uniforms
uniform DirLight dirLights[//$nDirLight];
uniform PointLight pointLights[//$nPointLight];
uniform SpotLight spotLights[//$nSportLight];

/////////// Camera uniforms
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

vec3 calcLight(vec3 normal, vec3 viewDir, vec3 lightDir, Light light, Material material);

Light light;
Material material;

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	//if(hasAmbientMap) {
		material.ambient = vec3(texture(ambient, TexCoords));
	//} else {
		material.ambient = ka;
	//}
	//if(hasDiffuseMap) {
		material.diffuse = vec3(texture(diffuse, TexCoords));
	//} else {
		material.diffuse = kd;
	//}
	//if(hasSpecularMap) {
		material.specular = vec3(texture(specular, TexCoords));
	//} else {
		material.specular = ks;
	//}
	material.shininess = shininess;

	vec3 result = vec3(0);
	for(int i = 0; i < /*$nDirLight*/; i++) {
		DirLight tlight = dirLights[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(-tlight.direction);
		result += calcLight(norm, viewDir, lightDir, tlight, material);
	}
	for(int i = 0; i < /*$nPointLight*/; i++) {
		PointLight tlight = pointLight[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(tlight.position - fragPos);

		// Attenuation
		float distance    = length(tlight.position - fragPos);
		float attenuation = 1.0f / (tlight.constant + tlight.linear * distance +
								 tlight.quadratic * (distance * distance));

		result += attenuation * calcLight(norm, viewDir, lightDir, tlight, material);
	}
	for(int i = 0; i < /*$nSpotLight*/; i++) {
		SpotLight tlight = spotLight[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(tlight.position - fragPos);

		// Attenuation
		float distance    = length(tlight.position - fragPos);
		float attenuation = 1.0f / (tlight.constant + tlight.linear * distance +
								 tlight.quadratic * (distance * distance));
		// Spotlight intensity
		float theta = dot(lightDir, normalize(-tlight.direction));
		float epsilon = tlight.cutOff - tlight.outerCutOff;
		float intensity = clamp((theta - tlight.outerCutOff) / epsilon, 0.0, 1.0);

		result += attenuation * intensity * calcLight(norm, viewDir, lightDir, tlight, material);
	}

	color = vec4(result, 1.0);
}

//UserDef
vec3 calcLight(vec3 normal, vec3 viewDir, vec3 lightDir, Light light, Material material) {
	// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Combine results
    vec3 ambient  = light.ambient  * material.ka;
    vec3 diffuse  = light.diffuse  * diff * material.kd;
    vec3 specular = light.specular * spec * material.ks;

	return (ambient + diffuse + specular);
}