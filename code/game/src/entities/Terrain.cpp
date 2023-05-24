//
// Created by Fabian on 16/05/2023.
//

#include "Terrain.h"

gl3::Terrain::Terrain(float scale) : Entity(Shader("shaders/shaded/vertexShader.vert", "shaders/shaded/fragmentShader.frag"),
                                 Mesh("gltf/terrain.glb")){
    Entity::scale = {scale, scale, scale};

}
