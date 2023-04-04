#version 460 core

in vec3 normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 lightColor = vec3(1.0f , 1.0f ,1.0f);
uniform sampler2D baseColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {

    //diffuse
    vec3 normalizedNomal = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float difference = max(dot(normalizedNomal, lightDir), 0.0);
    vec3 diff = lightColor * difference * vec3(texture(baseColor, TexCoords));

    //specular highlights
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalizedNomal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float attenuation = max(dot(normalizedNomal, -lightDir), 0.0f);

    vec3 color = vec3(texture(baseColor, TexCoords));
    fragColor = vec4(diff + specular, 1.0f);
}

