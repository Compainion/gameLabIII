#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 mvp;
uniform mat4 modelToWorldNormal;
out vec3 normal;
out vec2 TexCoords;

void main() {
    gl_Position = mvp * vec4(aPosition, 1.0f);
    normal = vec3(modelToWorldNormal * vec4(aNormal, 0.0f));
    TexCoords = aTexCoords;
}