#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords; // Added to pass texture coordinates from vertex shader

out vec4 FragColor;

struct Material {
    sampler2D diffuse;   // Texture for diffuse lighting
    sampler2D specular;  // Texture for specular lighting
    float shininess;     // Shininess factor for specular highlights
};

struct PointLight {
    vec3 position;
    float k_c;           // Constant attenuation
    float k_l;           // Linear attenuation
    float k_q;           // Quadratic attenuation
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DiectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cos_theta;     // Spotlight cutoff
    float k_c;           // Constant attenuation
    float k_l;           // Linear attenuation
    float k_q;           // Quadratic attenuation
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DiectionalLight diectionalLight;
uniform SpotLight spotlight;
uniform bool dlighton;
uniform bool spotlighton;

// Function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcDirectionalLight(Material material, DiectionalLight light, vec3 N, vec3 V);
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V);

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    
    // Add lighting contributions
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(material, pointLights[i], N, FragPos, V);
    
    if (dlighton)
        result += CalcDirectionalLight(material, diectionalLight, N, V);

    if (spotlighton)
        result += CalcSpotLight(material, spotlight, N, FragPos, V);

    // Final color output
    FragColor = vec4(result, 1.0);
    //FragColor = texture(material.diffuse, TexCoords);

}

vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);

    // Sample texture values
    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecular = vec3(texture(material.specular, TexCoords));

    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));

    vec3 ambient = texDiffuse * light.ambient * attenuation;
    vec3 diffuse = texDiffuse * max(dot(N, L), 0.0) * light.diffuse * attenuation;
    vec3 specular = texSpecular * pow(max(dot(V, R), 0.0), material.shininess) * light.specular * attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcDirectionalLight(Material material, DiectionalLight light, vec3 N, vec3 V)
{
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);

    // Sample texture values
    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecular = vec3(texture(material.specular, TexCoords));

    vec3 ambient = texDiffuse * light.ambient;
    vec3 diffuse = texDiffuse * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = texSpecular * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);

    // Sample texture values
    vec3 texDiffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 texSpecular = vec3(texture(material.specular, TexCoords));

    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));

    float cos_alpha = dot(L, normalize(-light.direction));
    float intensity = cos_alpha > light.cos_theta ? cos_alpha : 0.0;

    vec3 ambient = texDiffuse * light.ambient;
    vec3 diffuse = texDiffuse * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = texSpecular * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}