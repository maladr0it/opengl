#version 330 core

struct Material {
  float shininess;
};

struct DirectionalLight {
  vec3 dir;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;

uniform sampler2D specular1;
uniform sampler2D specular2;

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight sunlight;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 fragColor;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 viewDir = normalize(fragPos - viewPos);
  vec3 result = vec3(0.0);

  result += calcDirectionalLight(sunlight, normal, viewDir);

  fragColor = vec4(result, 1.0);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(light.dir);

  // diffuse
  float diffuseStrength = max(dot(-lightDir, normal), 0.0);

  // specular
  vec3 reflectDir = reflect(lightDir, normal);
  float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);

  // result
  vec3 ambient = light.ambient * vec3(texture(diffuse1, fragTexCoords));
  vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(diffuse1, fragTexCoords));
  vec3 specular = light.specular * specularStrength * vec3(texture(specular1, fragTexCoords));
  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(fragPos - light.pos);
    
    // diffuse
    float diffuseStrength = max(dot(-lightDir, normal), 0.0);
    
    // specular
    vec3 reflectDir = reflect(lightDir, normal);
    float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // result
    vec3 ambient = light.ambient * vec3(texture(diffuse1, fragTexCoords));
    vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(diffuse1, fragTexCoords));
    vec3 specular = light.specular * specularStrength * vec3(texture(specular1, fragTexCoords));
    return (ambient + diffuse + specular) * attenuation;
} 
