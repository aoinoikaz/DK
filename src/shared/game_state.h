#include "math_helper.h"
#include "game_object.h"

struct GameState
{
    int ticks;
    const char* BackgroundPath;
    GameObject* background;
};
