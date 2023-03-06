#include "Game.h"
#include <iostream>

#define GET_STRING(x) #x
#define GET_DIR(x) GET_STRING(x)

int main() {
    try {
        gl3::Game spaceBattle("Hallo Mama");
        spaceBattle.run();
    }
    catch(const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }
}
