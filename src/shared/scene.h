#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"
#include "vector"
#include "iostream"
#include "algorithm"

struct Scene
{
    std::vector<GameObject*> GameObjects;
    std::vector<GameObject*> Enemies;
    std::vector<GameObject*> NPCs;
    std::vector<GameObject*> UIElements;
};

void RemoveObjectsFromScene(Scene* scene)
{
    for(std::vector<GameObject*>::iterator it = scene->GameObjects.begin();
        it != scene->GameObjects.end(); ++it)
    {
        delete *it;
    } 
    scene->GameObjects.clear();
}

void AddObjectToScene(Scene* scene, GameObject* obj)
{
    scene->GameObjects.push_back(obj);
}

void RemoveObjectFromScene(Scene* scene, GameObject* obj)
{
    std::vector<GameObject*>::iterator it;
    it = find(scene->GameObjects.begin(), scene->GameObjects.end(), obj);

    if(it != scene->GameObjects.end())
    {
        std::cout << "Elemeny found in vector: " << *it << std::endl;
        delete *it;
        scene->GameObjects.clear();
    }
    else
    {
        std::cout << "Error: unable to remove object from scene!" << std::endl;
    }
}

#endif