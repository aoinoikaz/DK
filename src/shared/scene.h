// #ifndef SCENE_H
// #define SCENE_H

// #include "game_object.h"
// #include "vector"
// #include "iostream"
// #include "algorithm"
// #include "string.h"

// struct Scene
// {
//     std::vector<GameObject*> GameObjects;
// };

// GameObject* FindObjectByName(Scene* scene, std::string objToFind)
// {
//     for(std::vector<GameObject*>::iterator it = scene->GameObjects.begin();
//         it != scene->GameObjects.end(); ++it)
//     {
//         if(strcmp((*it)->Name.c_str(), objToFind.c_str()) == 0)
//         {
//             return (*it);
//         }
//     }
// }

// void AddObjectToScene(Scene* scene, GameObject* obj)
// {
//     std::cout << "Adding obj " << &obj << " to scene" << std::endl;
//     scene->GameObjects.push_back(obj);
// }

// void RemoveObjectsFromScene(Scene* scene)
// {
//     for(std::vector<GameObject*>::iterator it = scene->GameObjects.begin();
//         it != scene->GameObjects.end(); ++it)
//     {
//         delete *it;
//     } 
//     scene->GameObjects.clear();
// }

// bool RemoveObjectFromScene(Scene* scene, GameObject* obj)
// {
//     bool removed = false;

//     std::vector<GameObject*>::iterator it;
//     it = find(scene->GameObjects.begin(), scene->GameObjects.end(), obj);

//     if(it != scene->GameObjects.end())
//     {
//         delete *it;
//         scene->GameObjects.clear();
//         removed = true;
//     }
//     return removed;
// }

// #endif