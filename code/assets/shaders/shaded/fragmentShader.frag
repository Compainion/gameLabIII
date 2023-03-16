#version 460 core

in vec3 normal;
in vec2 TexCoords;

uniform vec4 color = vec4(0.945f, 0.768f, 0.058f, 1.0f);
uniform vec3 lightColor = vec3(1.0f , 1.0f ,1.0f);
uniform vec4 lightDirection = vec4(-0.5f, -0.5f, -0.5f, 1.0f);
uniform sampler2D diffuse;

out vec4 fragColor;

void main() {


    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(vec3(lightDirection));
    float difference = max(dot(normal, lightDir), 0.0);
    float attenuation = max(dot(norm, -lightDir), 0.0f);
    vec3 diff = lightColor * difference * vec3(texture(diffuse, TexCoords));
    vec3 result = vec3(texture(diffuse, TexCoords));
    fragColor = vec4(result * attenuation, 1.0f);
}