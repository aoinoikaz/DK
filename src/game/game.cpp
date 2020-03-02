#include "game.h"
#include "iostream"

extern "C" void UpdateGameState(GameState* gameState, Scene* scene, Input input)
{
    int seconds = gameState->ticks++ / 60;
    int minutes = seconds / 60;

    std::cout << "Session time | " <<  minutes << " mins : " << (seconds > 60 ? (seconds % minutes) : seconds) << " secs" << std::endl;
    std::cout << "Scene object count: " << scene->GameObjects.size() << std::endl;

    if(input.KeyPressed(KeyCode::Space))
    {
        if(scene->GameObjects.size() < 1)
        {
            scene->GameObjects.push_back(new GameObject("Background_Level_2", gameState->BackgroundPath));
            std::cout << &gameState->BackgroundPath << std::endl;
        }
        else
        {
            RemoveObjectsFromScene(scene);
        }
    }

    if(input.MouseButtonDown(MouseButton::Left))
    {
        std::cout << "Left mouse button down" << std::endl;
    }
    if(input.MouseButtonReleased(MouseButton::Left))
    {
        std::cout << "Left mouse button released" << std::endl;
    }
    if(input.MouseButtonDown(MouseButton::Right))
    {
        std::cout << "Right mouse button down" << std::endl;
    }
    if(input.MouseButtonReleased(MouseButton::Right))
    {
        std::cout << "Right mouse button released" << std::endl;
    }
    if(input.KeyDown(KeyCode::W)) 
    {
        std::cout << "W Down" << std::endl;
    }
    if(input.KeyDown(KeyCode::A)) 
    {
        std::cout << "A Down" << std::endl;
    }
    if(input.KeyDown(KeyCode::S))
    {
        std::cout << "S Down" << std::endl;
    }
    if(input.KeyDown(KeyCode::D)) 
    {
        std::cout << "D Down" << std::endl;
    }
}
