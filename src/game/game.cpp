#include "game.h"
//#include "iostream"
#include "fstream"
#include "sstream"

struct Bone {
    std::string id;
    std::string image;
    Rect rect;
};

struct Frame {
    std::vector<Bone> boneStates;
};

struct Animation {
    std::vector<Bone> bones;
    std::vector<Frame> frames;
    int frameSpeed;
};


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

    std::cout << "Loaded Animation File:" << file << std::endl;

    std::string frameSpeedSection = file.substr(0, file.find("bones:"));
    std::string boneSection = file.substr(frameSpeedSection.length(), file.find("frames:") - frameSpeedSection.length());
    std::string frameSection = file.substr(frameSpeedSection.length() + boneSection.length());

    boneSection = boneSection.substr(boneSection.find("[") + 1, boneSection.find("]") - boneSection.find("[") - 1);
    frameSection = frameSection.substr(frameSection.find("[") + 1, frameSection.find("]") - frameSection.find("[") - 1);

    std::string frameSpeed = frameSpeedSection.substr(frameSpeedSection.find(":") + 1);
    anim.frameSpeed = std::stoi(frameSpeed);
    
    int currentIndex = 0;
    while(currentIndex < boneSection.length() - 1)
    {        
        std::string properties = boneSection.substr(currentIndex + 1, boneSection.find(")", currentIndex + 1) - currentIndex - 1);

        std::stringstream ss(properties);
        std::vector<std::string> segments;

        while(ss.good())
        {   
            std::string buffer;
            getline(ss, buffer, ',');

            if(buffer.find("\"") != -1)
            {
                buffer = buffer.substr(buffer.find("\"") + 1, buffer.length() - buffer.find("\"") - 2);
            }
            else
            {
                buffer = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":") - 1);
            }

            segments.push_back(buffer);
        }

        Bone bone;
        bone.id = segments[0];
        bone.image = segments[1];
        bone.rect.w = std::stoi(segments[2]);
        bone.rect.h = std::stoi(segments[3]);

        anim.bones.push_back(bone);

        currentIndex += properties.length() + 2;
    }

    currentIndex = 0;
    while(currentIndex < frameSection.length() - 1)
    {        
        Frame frame;

        for(std::vector<Bone>::iterator it = anim.bones.begin();
            it != anim.bones.end(); ++it)
        {
            Bone newBoneState;
            newBoneState.id = it->id;
            newBoneState.image = it->image;
            newBoneState.rect = it->rect;
            frame.boneStates.push_back(newBoneState);
        }

        std::string properties;
        
        if(frameSection.find("))(", currentIndex + 1) != -1)
        {
            properties = frameSection.substr(currentIndex + 1, frameSection.find("))(", currentIndex + 1) - currentIndex);
        } 
        else
        {
            properties = frameSection.substr(currentIndex + 1, frameSection.find("))]", currentIndex + 1) - currentIndex);
        }

        std::stringstream ss(properties);

        while(ss.good())
        {
            std::string buffer;
            getline(ss, buffer, ')');
            std::stringstream ss2(buffer);
            std::vector<std::string> boneSegments;

            if(buffer != "") 
            {
                while(ss2.good() && buffer != "")
                {
                    getline(ss2, buffer, ',');
                    if(buffer.find("\"") != -1)
                    {
                        buffer = buffer.substr(buffer.find("\"") + 1, buffer.length() - buffer.find("\"") - 2);
                    }
                    else
                    {
                        buffer = buffer.substr(buffer.find(":") + 1, buffer.length() - buffer.find(":") - 1);
                    }
                    std::cout << "Buffer: " << buffer << std::endl;
                    boneSegments.push_back(buffer);
                }

                for(int i = 0; i < frame.boneStates.size(); i++)
                {
                    std::cout << "Bone Segment 0: " << boneSegments[0] << std::endl;
                    std::cout << "Bone Segment 1: " << std::stoi(boneSegments[1]) << std::endl;
                    std::cout << "Bone Segment 2: " << std::stoi(boneSegments[2]) << std::endl;
                    if(frame.boneStates[i].id == boneSegments[0])
                    {
                        frame.boneStates[i].rect.x = std::stoi(boneSegments[1]);
                        frame.boneStates[i].rect.y = std::stoi(boneSegments[2]);
                    }
                }
 
                anim.frames.push_back(frame);
            }
        }

        currentIndex += properties.length() + 2;
    }

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
        
        for(int i = 0; i < anim.bones.size(); i++)
        {
            std::cout << "Bone " << i + 1 << "-> Id:" << anim.bones[i].id << " Image: " << anim.bones[i].image << std::endl;
        }
        for(int i = 0; i < anim.frames.size(); i++)
        {
            std::cout << "Frame " << i + 1 << "-> Id:" << anim.bones[i].id << " Image: " << anim.bones[i].image << std::endl;
            for(int j = 0; j < anim.frames[i].boneStates.size(); j++)
            {
                std::cout << "BoneState " << j + 1 << "-> Id:" << anim.frames[i].boneStates[j].id << " PosX: " << anim.frames[i].boneStates[j].rect.x << " PosY: " << anim.frames[i].boneStates[j].rect.y <<std::endl;
            }
        }

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
