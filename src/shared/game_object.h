#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "animation.h"
#include "math_helper.h"
#include "iostream"
#include "vector"
#include "string.h"

enum OBJECT_TYPE { basic = 0, dynamic = 1, animated = 2};
enum SPACE { local = 0, world = 1 };

struct GameObject
{
    GameObject() = default;

    GameObject(const char* objName, const char* graphicFilePath)
    {
        Type = OBJECT_TYPE::basic;
        GraphicFilePath = graphicFilePath;
        Name = objName;
        Parent = NULL;
    }

    GameObject(const char* objName, const char* graphicFilePath, std::vector<Animation> anims, Vector2 pos, Vector2 scale , float rot)
    {
        Type = OBJECT_TYPE::animated;
        CurrentAnimation = anims.front();
        Animations = anims;

        GraphicFilePath = graphicFilePath;
        Name = objName;
       
        Position = pos;
        Rotation = rot;
        Scale = scale;

        Parent = NULL;
    }

    const char* Name;
    const char* GraphicFilePath;
    OBJECT_TYPE Type;
    GameObject* Parent;
    Animation CurrentAnimation;
    std::vector<Animation>Animations;

    float Rotation;
    Vector2 Position;
    Vector2 Scale;
};

Animation GetAnimationByName(GameObject* obj, const char* animationToFind)
{
    for(std::vector<Animation>::iterator it = obj->Animations.begin();
        it != obj->Animations.end(); ++it)
    {
        if(strcmp((it)->Name, animationToFind) == 0)
        {
            return *it;
        }
    }
}

void SwapAnimation(GameObject* obj, const char* path, const char* animName)
{
    std::cout << "Object: " << &obj;
    std::cout << " Path: " << path;
    std::cout << " AnimName: " << animName << std::endl;

    obj->CurrentAnimation = GetAnimationByName(obj, animName);
    obj->GraphicFilePath = path;
}

float GetRotation(GameObject gameObject, SPACE space)
{
    if(space == local || gameObject.Parent == NULL)
    {
        return gameObject.Rotation;
    }

    return GetRotation(*gameObject.Parent, world) + gameObject.Rotation;
}

void SetRotation(GameObject* gameObject, float rotation)
{
    gameObject->Rotation = rotation;

    if(rotation > 360.0f)
    {
        gameObject->Rotation -= 360.0f;
    }

    if(rotation < 0.0f)
    {
        gameObject->Rotation += 360.0f;
    }
}

Vector2 GetScale(GameObject gameObject, SPACE space)
{
    if(space == local || gameObject.Parent == NULL)
    {
        return gameObject.Scale;
    }

    Vector2 parentScale = GetScale(*gameObject.Parent, world);
    parentScale.x *= gameObject.Scale.x;
    parentScale.y *= gameObject.Scale.y;

    return gameObject.Scale;
}

void Scale(GameObject* gameObject, Vector2 scale)
{
    gameObject->Scale = scale;
}

Vector2 GetPos(GameObject gameObject, SPACE space)
{
    if(space == local || gameObject.Parent == NULL)
    {
        return gameObject.Position;
    }

    Vector2 parentScale = GetScale(*gameObject.Parent, world);
    Vector2 objectPos = gameObject.Position;
    
    Vector2 rotPos = RotateVector(Vector2(
        objectPos.x * parentScale.x,
        objectPos.y * parentScale.y
    ), GetRotation(*gameObject.Parent, local));

    return GetPos(*gameObject.Parent, world) + rotPos;
}

void SetPos(GameObject* gameObject, Vector2 pos)
{
    gameObject->Position = pos;
}

void Translate(GameObject* gameObject, Vector2 vec)
{
    gameObject->Position += vec;
}

void Rotate(GameObject* gameObject, float amount)
{
    SetRotation(gameObject, gameObject->Rotation + amount);
}

/*
TODO: This function is currently not working

void SetParent(GameObject* gameObject, GameObject parent)
{
    if(gameObject->Parent == NULL)
    {
        gameObject->Position = GetPos(*gameObject, world);
        gameObject->Rotation = GetRotation(*gameObject, world);
        gameObject->Scale = GetScale(*gameObject, world);
    }
    else 
    {
        SetParent(gameObject, NULL);
    }
}
*/

#endif