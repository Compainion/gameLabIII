#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec3 normal;
out vec2 TexCoords;
out vec3 position;

uniform mat4 mvp;
uniform mat4 modelToWorldNormal;

vec3 currentPos;

void main() {
    currentPos = vec3(instanceMatrix * vec4(aPosition, 1.0f));
    gl_Position = mvp * vec4(currentPos, 1.0f);
    normal = vec3(modelToWorldNormal * vec4(aNormal, 0.0f));
    position = aPosition;
    TexCoords = aTexCoords;
}
