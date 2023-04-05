
#ifndef GAMELABIII_LIGHT_H
#define GAMELABIII_LIGHT_H


#include "Entity.h"

namespace gl3 {

    class Light : public Entity{
    public:
        explicit Light();
        void update(Game *game, float deltaTime) override;
    };

}


#endif
