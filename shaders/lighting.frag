//Light uniforms structs
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

//Public Struct
struct Material {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//Material uniforms
#ifdef AMBIENT_MAP
uniform sampler2D ambient;
#else
uniform vec3 ka;
#endif

#ifdef DIFFUSE_MAP
uniform sampler2D diffuse;
#else
uniform vec3 kd;
#endif

#ifdef SPECULAR_MAP
uniform sampler2D specular;
#else
uniform vec3 ks;
#endif

#ifdef NORMAL_MAP
uniform sampler2D normalMap;
#endif

uniform float shininess;

//Light uniforms
#ifndef NO_DIRLIGHT
uniform DirLight dirLights[N_DIRLIGHT];
#endif
#ifndef NO_POINTLIGHT
uniform PointLight pointLights[N_POINTLIGHT];
#endif
#ifndef NO_SPOTLIGHT
uniform SpotLight spotLights[N_SPOTLIGHT];
#endif

//Camera uniform
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

out vec4 color;

vec3 calcLight(vec3 normal, vec3 viewDir, vec3 lightDir, Light light, Material material);

void main()
{
#ifdef NORMAL_MAP
	vec3 norm = texture(normalMap, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);
	norm = normalize(TBN * norm);
#else
	vec3 norm = normalize(Normal);
#endif
	//norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos);

	Light light;
	Material material;

#ifdef AMBIENT_MAP
	material.ka = vec3(texture(ambient, TexCoords));
#else
	material.ka = ka;
#endif

#ifdef DIFFUSE_MAP
	material.kd = vec3(texture(diffuse, TexCoords));
#else
	material.kd = kd;
#endif

#ifdef SPECULAR_MAP
	material.ks = vec3(texture(specular, TexCoords));
#else
	material.ks = ks;
#endif
	
	material.shininess = shininess;

	vec3 result = vec3(0);
#ifndef NO_DIRLIGHT
	for(int i = 0; i < N_DIRLIGHT; i++) {
		DirLight tlight = dirLights[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(-tlight.direction);
		result += calcLight(norm, viewDir, lightDir, light, material);
	}
#endif
#ifndef NO_POINTLIGHT
	for(int i = 0; i < N_POINTLIGHT; i++) {
		PointLight tlight = pointLights[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(tlight.position - FragPos);

		// Attenuation
		float distance    = length(tlight.position - FragPos);
		float attenuation = 1.0f / (tlight.constant + tlight.linear * distance +
								 tlight.quadratic * (distance * distance));

		result += attenuation * calcLight(norm, viewDir, lightDir, light, material);
	}
#endif
#ifndef NO_SPOTLIGHT
	for(int i = 0; i < N_SPOTLIGHT; i++) {
		SpotLight tlight = spotLights[i];
		light.ambient = tlight.ambient;
		light.diffuse = tlight.diffuse;
		light.specular = tlight.specular;

		vec3 lightDir = normalize(tlight.position - FragPos);

		// Attenuation
		float distance    = length(tlight.position - FragPos);
		float attenuation = 1.0f / (tlight.constant + tlight.linear * distance +
								 tlight.quadratic * (distance * distance));
		// Spotlight intensity
		float theta = dot(lightDir, normalize(-tlight.direction));
		float epsilon = tlight.cutOff - tlight.outerCutOff;
		float intensity = clamp((theta - tlight.outerCutOff) / epsilon, 0.0, 1.0);

		result += attenuation * intensity * calcLight(norm, viewDir, lightDir, light, material);
	}
#endif

	color = vec4(result, 1.0);
}