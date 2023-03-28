
#pragma once
#include "Entity.h"
#include "../Game.h"


namespace gl3 {

    class Planet : public Entity{
    public:
        explicit Planet(glm::mat4 modelMatrices[], int amount);
        void draw(Game *game) override;
    };


} // gl3
