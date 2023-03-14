
#pragma once
#include "Entity.h"
#include "../Game.h"


namespace gl3 {

    class Planet : public Entity{
    public:
        explicit Planet(glm::vec3 position = glm::vec3(0, 0, 0), float size = 1, glm::vec4 color = glm::vec4(1, 1, 1, 1));
        void draw(Game *game) override;
    };


} // gl3
