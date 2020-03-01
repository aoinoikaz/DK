#include "dlfcn.h"
#include "iostream"
#include "unistd.h"
#include "sys/stat.h"

#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "../shared/game_state.h"
#include "../shared/math_helper.h"
#include "../shared/input.h"

#define DLL_PATH "lib/game.so"
#define DLL_SYMBOL "UpdateGameState"

typedef void _UpdateGameState(GameState* gameState, Input* input);

struct GameCode
{
    void* gameDLL;
    _UpdateGameState* Update;
    timespec lastWriteTime;
};

bool operator >(const timespec& lhs, const timespec& rhs)
{
	if (lhs.tv_sec == rhs.tv_sec) 
		return lhs.tv_nsec > rhs.tv_nsec;
	else
		return lhs.tv_sec > rhs.tv_sec;
}

void TryReloadGameCode(GameCode* gameCode, const char* gameDLLPath)
{
    struct stat fileInfo;

    if(stat(gameDLLPath, &fileInfo) == -1)
    {
        std::cout << "Unable to get file properties: " << errno << std::endl;
    }

    timespec currentWriteTime = fileInfo.st_mtim;
    if(currentWriteTime > gameCode->lastWriteTime || !gameCode->gameDLL)
    {
        if(gameCode->gameDLL)
        {
            dlclose(gameCode->gameDLL);
            gameCode->Update = NULL;
        }

        gameCode->gameDLL = dlopen(gameDLLPath, RTLD_NOW);
        if(gameCode->gameDLL)
        {
            gameCode->Update = (_UpdateGameState*) dlsym(gameCode->gameDLL, DLL_SYMBOL);
            gameCode->lastWriteTime = currentWriteTime;
        }
    }
}

// Input handling
int numKeys = 0;
Uint8* previousKeyboardState;
const Uint8* keyboardState;
Uint32 previousMouseState;
Uint32 mouseState;
Vector2 mousePosition;
unsigned int startTicks;

void ResetInputState()
{
    startTicks = SDL_GetTicks();
    previousMouseState = mouseState;
    memcpy(previousKeyboardState, keyboardState, numKeys);
}

bool KeyDown(KeyCode keyCode)
{
    return keyboardState[keyCode];
}

bool KeyPressed(KeyCode keyCode)
{
    return !previousKeyboardState[keyCode] && keyboardState[keyCode];
}

bool KeyReleased(KeyCode keyCode)
{
    return previousKeyboardState[keyCode] && !keyboardState[keyCode];
}

Uint32 GetButtonMask(MouseButton mouseButton)
{
    Uint32 mask = 0;
    switch(mouseButton)
    {
        case Left:
            mask = SDL_BUTTON_LMASK;
            break;
        case Right:
            mask = SDL_BUTTON_RMASK;
            break;
    }
    return mask;
}

bool MouseButtonDown(MouseButton mouseButton)
{
    Uint32 mask = GetButtonMask(mouseButton);
    return (mouseState & mask);
}

bool MouseButtonPressed(MouseButton mouseButton)
{
    Uint32 mask = GetButtonMask(mouseButton);
    return !(previousMouseState & mask) && (mouseState & mask);
}

bool MouseButtonReleased(MouseButton mouseButton)
{
    Uint32 mask = GetButtonMask(mouseButton);
    return (previousMouseState & mask) && !(mouseState & mask);
}

Vector2 GetMousePosition(void)
{
    return mousePosition;
}

// Main loop setup
SDL_Event gameEvents;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FRAME_RATE = 60;

bool running = true;

int main(int argc, char* argv[])
{
    GameCode gameCode = {};

    GameState gameState = {};
    gameState.ticks = 0;

    GPU_Target* target;
    if((target = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS)) == NULL) 
    {
        std::cout << "SDL GPU Init Error" << SDL_GetError() << std::endl;
    }

    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int flags = IMG_INIT_PNG;

    if(!(IMG_Init(flags) & flags))
    {
        std::cout << "IMG Init error: " << IMG_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1)
    {
        std::cout << "TTF Init error: " << TTF_GetError() << std::endl;
        return false;
    }

    Input input = {};
    input.KeyDown = &KeyDown;
    input.KeyPressed = &KeyPressed;
    input.KeyReleased = &KeyReleased;
    input.MouseButtonDown = &MouseButtonDown;
    input.MouseButtonPressed = &MouseButtonPressed;
    input.MouseButtonReleased = &MouseButtonReleased;

    keyboardState = SDL_GetKeyboardState(&numKeys);
    previousKeyboardState = new Uint8[numKeys];
    startTicks = SDL_GetTicks();

    while(running)
    { 
        TryReloadGameCode(&gameCode, DLL_PATH);

        while(SDL_PollEvent(&gameEvents) != 0)
        {
            if(gameEvents.type == SDL_QUIT)
            {
                running = false;
            }
        }

        mouseState = SDL_GetMouseState(&mousePosition.x, 
            &mousePosition.y
        );

        unsigned int elapsedTicks = SDL_GetTicks() - startTicks;
        float deltaTime = elapsedTicks * 0.001f;
        
        if(deltaTime >= (1.0f / FRAME_RATE))
        {   
            // Input
            mouseState = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
            
            // Update/render the physics and changes from input
            if(gameCode.Update)
            {
                gameCode.Update(&gameState, &input);
            }

            // Reset any state
            ResetInputState();
        } 
    }

    delete[] previousKeyboardState; // Doesnt this get released when execution ends anyways?

    return 0;
}