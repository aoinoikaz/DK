#include "game.h"
#include "iostream"

extern "C" void UpdateGameState(GameState* gameState, Input* input)
{
    std::cout << ++gameState->ticks / 60 << std::endl;

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
<<<<<<< HEAD

    if(input->MouseButtonDown(MouseButton::Left)) 
    {
        std::cout << "Left mouse down" << std::endl;
    }

    if(input->MouseButtonReleased(MouseButton::Left)) 
    {
        std::cout << "Left mouse Released" << std::endl;
    }
=======
>>>>>>> develop
}
