#include "game.h"
#include "iostream"

extern "C" void UpdateGameState(GameState* gameState, Input input)
{
    //NOTE (KYLE): If the game isn't initialized, load textures
    if(!gameState->initialized) 
    {
        gameState->renderTextures.push_back("./assets/map/game_background_2.png");
        gameState->initialized = true;
    }

    if(gameState->ticks++ % 60 == 0)
    {
        int totalSeconds = gameState->ticks / 60;
        int totalMinutes = totalSeconds >= 60 ? totalSeconds / 60 : 0;
        int currentSeconds = totalSeconds % 60;
        int mod = totalMinutes >= 60 ? totalMinutes % 60 : 0;
        std::cout << 1000 % 60 << std::endl;
        std::cout << "Session time | " << totalMinutes/60 << ":" << (totalMinutes >= 60 ? mod : totalMinutes) << ":" << currentSeconds << std::endl;
    }  

    if(input.KeyDown(KeyCode::W)) 
    {
        std::cout << "Pressing W" << std::endl;
    }

    if(input.KeyDown(KeyCode::A)) 
    {
        std::cout << "Pressing A" << std::endl;
    }

    if(input.KeyDown(KeyCode::S)) 
    {
        std::cout << "Pressing S" << std::endl;
    }

    if(input.KeyDown(KeyCode::D)) 
    {
        std::cout << "Pressing D" << std::endl;
    }

    if(input.MouseButtonDown(MouseButton::Left))
    {
        std::cout << "Left mouse button down" << std::endl;
    }
    if(input.MouseButtonReleased(MouseButton::Left))
    {
        std::cout << "Left mouse button released" << std::endl;
    }

    //NOTE (KYLE): Animation sequence here. should be based on game ticks rather than time

    //animSpeed = 60 because we want it to switch once per second
    //frameCount = 10 because there are 10 frames in the sprite atlas
    //gameState->renderTexture[i] = textureArray[(ticks / animSpeed) % frameCount] <-- quick maths
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
