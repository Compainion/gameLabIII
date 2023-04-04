#version 460 core

in vec3 normal;
in vec2 TexCoords;
in vec3 position;
in vec3 FragPos;

uniform vec4 color = vec4(0.945f, 0.768f, 0.058f, 1.0f);
uniform vec3 lightColor = vec3(1.0f , 1.0f ,1.0f);
uniform vec3 lightPos;


uniform sampler2D diffuse;
uniform float time;

out vec4 fragColor;

void main() {

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float attenuation = max(dot(norm, -lightDir), 0.0f);
    vec3 coolEffect = position * time;
    fragColor = vec4(coolEffect * attenuation, 1.0f);
}
