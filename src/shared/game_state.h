#include "math_helper.h"
#include "vector"

struct GameState
{
    int ticks;
    bool initialized;

    std::vector<const char*> renderTextures;
    std::vector<Vector2> renderTexturePositions;
};