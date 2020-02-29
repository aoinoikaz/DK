#include "dlfcn.h"
#include "iostream"
#include "unistd.h"
#include "sys/stat.h"

#include "../shared/game_state.h"

typedef void _UpdateAndRender(GameState* gameState);

struct GameCode
{
    void* gameDLL;
    _UpdateAndRender* UpdateAndRender;
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
        std::cout << "Unable to get file properties." << std::endl;
        exit(EXIT_FAILURE);
    }

    timespec currentWriteTime = fileInfo.st_mtim;
    if(!gameCode->gameDLL || currentWriteTime > gameCode->lastWriteTime)
    {
        std::cout << "Reloading" << std::endl;

        if(gameCode->gameDLL)
        {
            dlclose(gameCode->gameDLL);
        }

        gameCode->gameDLL = dlopen(gameDLLPath, RTLD_NOW);
        if(gameCode->gameDLL)
        {
            gameCode->UpdateAndRender = (_UpdateAndRender*)dlsym(gameCode->gameDLL, "UpdateAndRender");
            gameCode->lastWriteTime = currentWriteTime;
        }
    }
}

#define LIB_PATH "../../lib/game.so"

int main(int argc, char* argv[])
{
    GameState gameState = {};
    gameState.Level = 1;

    GameCode gameCode = {};

    while(true)
    {
        TryReloadGameCode(&gameCode, LIB_PATH);

        if(gameCode.UpdateAndRender)
        {
            gameCode.UpdateAndRender(&gameState);
        }

        sleep(0.1f);
    }
    return 0;
}