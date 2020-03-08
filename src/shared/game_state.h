#include "math_helper.h"
#include "texture.h"
#include "vector"

struct GameState
{
    int ticks;
    bool initialized;

    std::vector<Texture> renderTextures;
};