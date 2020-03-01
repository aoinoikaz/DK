#include "game.h"
#include "iostream"

extern "C" void UpdateGameState(GameState* gameState, Input* input)
{
    int seconds = gameState->ticks++ / 60;
    int minutes = 0;
    minutes = seconds / 60;

    std::cout << "Development Session" << std::endl;
    std::cout << "Elapsed Time | " <<  minutes << " mins : " << (seconds > 60 ? (seconds % minutes) : seconds) << " secs" << std::endl;

    if(input->MouseButtonDown(MouseButton::Left))
    {
        std::cout << "Left mouse button down" << std::endl;
    }
    if(input->MouseButtonReleased(MouseButton::Left))
    {
        std::cout << "Left mouse button released" << std::endl;
    }
    if(input->MouseButtonDown(MouseButton::Right))
    {
        std::cout << "Right mouse button down" << std::endl;
    }
    if(input->MouseButtonReleased(MouseButton::Right))
    {
        std::cout << "Right mouse button released" << std::endl;
    }
    if(input->KeyDown(KeyCode::W)) 
    {
        std::cout << "W Down" << std::endl;
    }
    if(input->KeyDown(KeyCode::A)) 
    {
        std::cout << "A Down" << std::endl;
    }
    if(input->KeyDown(KeyCode::S))
    {
        std::cout << "S Down" << std::endl;
    }
    if(input->KeyDown(KeyCode::D)) 
    {
        std::cout << "D Down" << std::endl;
    }
    
    if(input->KeyPressed(KeyCode::Space))
    {
        if(gameState->CurrentBackground == gameState->background1)
        {
            gameState->CurrentBackground = gameState->background2;
        }
        else
        {
            gameState->CurrentBackground = gameState->background1;
        }
    }
}
