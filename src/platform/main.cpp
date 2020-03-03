#include "dlfcn.h"
#include "iostream"
#include "unistd.h"
#include "sys/stat.h"

#include "map"
#include "vector"

#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "../shared/game_state.h"
#include "../shared/math_helper.h"
#include "../shared/input.h"
#include "../shared/game_object.h"
#include "../shared/scene.h"
#include "../shared/animation.h"

// Real-time Code Recompilation

#define DLL_PATH "lib/game.so"
#define DLL_SYMBOL "UpdateGameState"

typedef void _UpdateGameState(GameState* gameState, Scene* scene, Input input);

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

// Graphics

std::map<const char*, GPU_Image*> textures;
std::map<std::string, GPU_Image*> texts;
std::map<std::string, TTF_Font*> fonts;

// Get texture based on file path. 
// If the texture exists in cached memory, return a pointer to the texture.
// Otherwise, load the image from memory, cache it, and return a pointer to the texture.
GPU_Image* LoadAndGetTexture(const char* path)
{
    if(!textures[path])
    {
        GPU_Image* texture = NULL;
        SDL_Surface* surface = IMG_Load(path);
        if(surface == NULL)
        {
            std::cout << "Image load error | Path: " << path << " | Error: " << IMG_GetError() << std::endl;
            return NULL;
        }

        texture = GPU_CopyImageFromSurface(surface);
        if(texture == NULL)
        {
            std::cout << "Create texture error: " << SDL_GetError() << std::endl;
            return NULL;
        }

        textures[path] = texture;

        SDL_FreeSurface(surface);
    }
    return textures[path];
}

GPU_Image* GetText(std::string text, std::string fontPath, int fontSize, SDL_Colour colour)
{
    std::string fontKey = fontPath + (char)fontSize;
    if(fonts[fontKey] == nullptr)
    {
        fonts[fontKey] = TTF_OpenFont(fontPath.c_str(), fontSize);
        if(fonts[fontKey] == nullptr)
        {
            std::cout << "Font Loading Error | Path: " << fontPath << " | Error: " << TTF_GetError() << std::endl;
            return NULL;
        }
    }

    TTF_Font* font = fonts[fontKey];
    std::string textKey = text + fontPath + char(fontSize) + (char)colour.r + (char)colour.b + (char)colour.g;
    
    // If this isn't the same text 
    if(texts[textKey] == nullptr)
    {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), colour);
        if(!surface)
        {
            std::cout << "Text creation error: " << TTF_GetError() << std::endl;
            return NULL;
        }

        texts[textKey] = GPU_CopyImageFromSurface(surface);
        if(texts[textKey] == nullptr)
        {
            std::cout << "Text creation error: " << SDL_GetError() << std::endl;
            return NULL;
        }
        SDL_FreeSurface(surface);
    }
    return texts[textKey];
}

// Input handling

int numKeys = 0;
Uint8* previousKeyboardState;
const Uint8* keyboardState;
Uint32 previousMouseState;
Uint32 mouseState;
int mousePosX;
int mousePosY;
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
    return Vector2((float)mousePosX, (float)mousePosY);
}

// Main loop setup

SDL_Event gameEvents;
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
const int FRAME_RATE = 60;

bool running = true;

int main(int argc, char* argv[])
{
    GameCode gameCode = {};

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

    GameState gameState = {};
    gameState.ticks = 0;
    gameState.Background_Path = "assets/map/game_background_2.png";
    gameState.Player_Idle_Path = "assets/character/player_idle.png";
    gameState.Player_Run_Path = "assets/character/player_run.png";

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

    // Load the texture into memory; in this case
    // we aren't using the returned value as its being
    // loaded into our texture map
    LoadAndGetTexture(gameState.Background_Path);
    LoadAndGetTexture(gameState.Player_Idle_Path);
    LoadAndGetTexture(gameState.Player_Run_Path);

    GPU_Rect sourceRect, destinationRect;
    Scene scene;

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

        unsigned int elapsedTicks = SDL_GetTicks() - startTicks;
        float deltaTime = elapsedTicks * 0.001f;
        
        if(deltaTime >= (1.0f / FRAME_RATE))
        {   
            if(gameCode.Update)
            {
                gameCode.Update(&gameState, &scene, input);
            }

            GPU_Clear(target);

            mouseState = SDL_GetMouseState(&mousePosX, &mousePosY);

            // Iterate through all the objects in the scene
            for(auto obj : scene.GameObjects)
            {
                // Will have to blit data to the screen accordingly to the specific game object type
                switch(obj->Type)
                {
                    case OBJECT_TYPE::basic:
                    {
                        GPU_BlitRect(textures[obj->GraphicFilePath], NULL, target, NULL);
                    }break;
                    case OBJECT_TYPE::animated:
                    {
                        sourceRect.w = obj->CurrentAnimation.ClipW;
                        sourceRect.h = obj->CurrentAnimation.ClipH;
                        //std::cout << "1" << std::endl;
                        destinationRect.w = obj->CurrentAnimation.ClipW;
                        destinationRect.h = obj->CurrentAnimation.ClipH;
                        destinationRect.x = obj->Position.x;
                        destinationRect.y = obj->Position.y;
                        //std::cout << "2" << std::endl;
                        
                        if(!obj->CurrentAnimation.AnimationProcessed)
                        {
                            //std::cout << "3" << std::endl;
                            obj->CurrentAnimation.Timer += deltaTime;
                            if(obj->CurrentAnimation.Timer >= obj->CurrentAnimation.AnimationSpeed)
                            {
                                //std::cout << "4" << std::endl;
                                if(obj->CurrentAnimation.WrapMode == Animation::loop)
                                {
                                    //std::cout << "5" << std::endl;
                                    obj->CurrentAnimation.Timer -= obj->CurrentAnimation.AnimationSpeed;
                                }
                                else
                                {
                                    //std::cout << "6" << std::endl;
                                    obj->CurrentAnimation.AnimationProcessed = true;
                                    obj->CurrentAnimation.Timer = obj->CurrentAnimation.AnimationSpeed - obj->CurrentAnimation.TimePerFrame;
                                }
                            }
                            //std::cout << "7" << std::endl;
                            int index = obj->CurrentAnimation.ClipX + ((int)(obj->CurrentAnimation.Timer / obj->CurrentAnimation.TimePerFrame) - 1) * obj->CurrentAnimation.ClipW;
                            sourceRect.x = index % (obj->CurrentAnimation.ClipX * (obj->CurrentAnimation.Frames / obj->CurrentAnimation.Rows));
                            sourceRect.y = obj->CurrentAnimation.ClipY * (index / (obj->CurrentAnimation.ClipX * (obj->CurrentAnimation.Frames / obj->CurrentAnimation.Rows)));
                            //std::cout << "8" << std::endl;
                        }
                        //std::cout << "9" << std::endl;
                        //std::cout << "Flipped" << obj->CurrentAnimation.Flipped << std::endl;
                        GPU_BlitRectX(textures[obj->GraphicFilePath], &sourceRect, target, &destinationRect, 0, 0, 0, obj->CurrentAnimation.Flipped ? GPU_FLIP_HORIZONTAL : GPU_FLIP_NONE);
                        //std::cout << "10" << std::endl;
                    }break;
                }
            }

            GPU_Flip(target);
            
            ResetInputState();
        }
    }
    return 0;
}