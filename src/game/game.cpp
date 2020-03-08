#include "game.h"
#include "iostream"
#include "fstream"


struct Bone {
    int id;
    std::string image;
    float width;
    float height;
};

struct Frame {
    int id;
    float posX;
    float posY;
};

struct Animation {
    std::vector<Bone> bones;
    std::vector<Frame> frames;
    int frameSpeed;
};
#include "cstring"
Animation LoadAnimation(std::string filePath)
{
    Animation anim;

    std::ifstream animationFile;
    animationFile.open(filePath);

    if(!animationFile)
    {
        std::cerr << "Failed to load animation from file: " << filePath << std::endl;
        exit(1);
    }

    std::string file, line;
    while (animationFile >> line)
    {
        file += line;
    }
    animationFile.close();

    std::cout << "Animation File:" << file << std::endl;

    std::string searchTerm = "frame_speed:";

    std::vector<std::string> sections;
    std::string frameSpeedSection = file.substr(0, file.find("bones:"));
    std::string boneSection = file.substr(frameSpeedSection.length(), file.find("frames:") - frameSpeedSection.length());
    std::string frameSection = file.substr(frameSpeedSection.length() + boneSection.length());

    std::cout << "Frame Speed: " << frameSpeedSection << std::endl<< std::endl;
    std::cout << "Bones: " << boneSection << std::endl<< std::endl;
    std::cout << "Frames: " << frameSection << std::endl<< std::endl;

    std::string frameSpeed = frameSpeedSection.substr(frameSpeedSection.find(":") + 1);

    std::string internals = boneSection.substr(boneSection.find("[") + 1, boneSection.find("]") - boneSection.find("[") - 1);
    std::cout << "Bones Internals: " << internals << std::endl;
    std::cout << "Internals length: " << internals.length() << std::endl;

    int currentIndex = 0;
    while(currentIndex < internals.length())
    {
        std::string properties = internals.substr(currentIndex + 1, internals.find(")"));
        std::cout << "Properties: " << properties << std::endl;


        // std::string indexStr = properties.substr(properties.find("id:\"") + 4, properties.find("\",") - 4);
        // std::cout << "Index:" << indexStr << std::endl;
    
        // size_t imageIndex = properties.find("image:\"") + 7;
        // size_t widthIndex = properties.find("width:") + 6;

        // std::cout <<  imageIndex << std::endl;
        // std::cout <<  widthIndex << std::endl;

        // std::string imageStr = properties.substr(imageIndex, widthIndex - imageIndex);
        // std::cout << "Image:" << imageStr << std::endl;

        currentIndex += properties.length() + 1;
    }

    anim.frameSpeed = std::stoi(frameSpeed);
    

    anim.frameSpeed = 0;
    int boneIndex = 0;
    // std::string boneStr = ;


    return anim;
}


extern "C" void UpdateGameState(GameState* gameState, Input input)
{
    //NOTE (KYLE): If the game isn't initialized, load textures
    if(!gameState->initialized) 
    {
        gameState->renderTextures.push_back(Texture(
            "./assets/map/game_background_2.png",
            Rect(0, 0, 3840, 2160), 
            Rect(0, 0, 900, 700)
        ));
        gameState->renderTextures.push_back(Texture(
            "./assets/character/player_idle.png",
            Rect(0, 0, 256, 256),
            Rect(0, 0, 256, 256)
        ));

        Animation anim = LoadAnimation("./assets/animations/player.sa");

        gameState->initialized = true;
    }

    if(input.KeyDown(KeyCode::Space))
    {
        gameState->renderTextures.clear();
        gameState->initialized = false;
    }

    if(gameState->ticks++ % 60 == 0)
    {
        int totalSeconds = gameState->ticks / 60;
        int totalMinutes = totalSeconds >= 60 ? totalSeconds / 60 : 0;
        int currentSeconds = totalSeconds % 60;
        int mod = totalMinutes >= 60 ? totalMinutes % 60 : 0;
        //std::cout << "Session time | " << totalMinutes/60 << ":" << (totalMinutes >= 60 ? mod : totalMinutes) << ":" << currentSeconds << std::endl;
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
