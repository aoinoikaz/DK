#include "game.h"
#include "iostream"

extern "C" void UpdateAndRender(GameState* gameState)
{
    std::cout << "Development session time: " << gameState->Level++ << std::endl;
}
