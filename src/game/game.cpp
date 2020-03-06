#include "game.h"
#include "iostream"

extern "C" void UpdateGameState(GameState* gameState, Scene* scene, Input input)
{
    int totalSeconds = gameState->ticks++ / 60;
    int totalMinutes = totalSeconds >= 60 ? totalSeconds / 60 : 0;
    int currentSeconds = totalSeconds % 60;
    int mod = totalMinutes >= 60 ? totalMinutes % 60 : 0;

    std::cout << "Session time | " << totalMinutes/60 << ":" << (totalMinutes >= 60 ? mod : totalMinutes) << ":" << currentSeconds << std::endl;

    if(input.MouseButtonDown(MouseButton::Left))
    {
        Vector2 mouseClick = input.GetMouseCoordinates();
        std::cout << "Left mouse button down : " << mouseClick.x << ", " << mouseClick.y << std::endl;
    }
    if(input.MouseButtonReleased(MouseButton::Left))
    {
        std::cout << "Left mouse button released" << std::endl;
    }
}



// // Test
// if(input.KeyPressed(KeyCode::Space))
// {
//     std::vector<Animation>anims;

//     anims.push_back(Animation("Idle", 18, 3, 1.0f, 256, 256, 256, 256,Animation::loop));
//     anims.push_back(Animation("Run", 12, 2, 1.0f, 256, 256, 256, 256, Animation::loop));
//     AddObjectToScene(scene, new GameObject("Background", gameState->Background_Path));
//     AddObjectToScene(scene, new GameObject("Player", 
//                                             gameState->Player_Idle_Path,
//                                             anims, Vector2(150, 150), VEC2_ONE, 0));


// }
// if(input.KeyReleased(KeyCode::Enter))
// {
//     RemoveObjectsFromScene(scene);
// }
// // End Test


// if(input.KeyDown(KeyCode::W))
// {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_UP*2);
// }
// else if (input.KeyReleased(KeyCode::W))
// {
//     SwapAnimation(FindObjectByName(scene, "Player"), gameState->Player_Idle_Path, "Idle");
// }


// if(input.KeyDown(KeyCode::A))
// {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     //obj->CurrentAnimation.Flipped = true;
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_LEFT*2);
// }
// else if (input.KeyReleased(KeyCode::A))
// {
//     GameObject* go = FindObjectByName(scene, "Player");
//     std::cout << "ObjectByName(Player): " << &go << std::endl;
//     //SwapAnimation(FindObjectByName(scene, "Player"), gameState->Player_Idle_Path, "Idle");
// }

// if(input.KeyDown(KeyCode::S))
// {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_DOWN*2);
// }
// else if (input.KeyReleased(KeyCode::S))
// { {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_UP*2);
// }
// else if (input.KeyReleased(KeyCode::W))
// {
//     SwapAnimation(FindObjectByName(scene, "Player"), gameState->Player_Idle_Path, "Idle");
// }


// if(input.KeyDown(KeyCode::A))
// {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     //obj->CurrentAnimation.Flipped = true;
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_LEFT*2);
// }
// else if (input.KeyReleased(KeyCode::A))
// {
//     GameObject* go = FindObjectByName(scene, "Player");
//     std::cout << "ObjectByName(Player): " << &go << std::endl;
//     //SwapAnimation(FindObjectByName(scene, "Player"), gameState->Player_Idle_Path, "Idle");
// }


// if(input.KeyDown(KeyCode::D))
// {
//     GameObject* obj = FindObjectByName(scene, "Player");
//     obj->CurrentAnimation.Flipped = false;
//     SwapAnimation(obj, gameState->Player_Run_Path, "Run");
//     Translate(obj, VEC2_RIGHT*2);
// }
// else if (input.KeyReleased(KeyCode::D))
// {
//     SwapAnimation(FindObjectByName(scene, "Player"), gameState->Player_Idle_Path, "Idle");
// }
