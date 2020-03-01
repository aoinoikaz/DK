#ifndef INPUT_H
#define INPUT_H

#include "math_helper.h"

enum KeyCode
{
    W = 26,
    A = 4,
    S = 22,
    D = 7,
    Enter = 40,
    Space = 44,
    Escape = 41
};

enum MouseButton
{
    Left,
    Right
};

struct Input
{
    bool (*KeyDown) (KeyCode);
    bool (*KeyPressed) (KeyCode);
    bool (*KeyReleased) (KeyCode);
    bool (*MouseButtonDown) (MouseButton);
    bool (*MouseButtonPressed) (MouseButton);
    bool (*MouseButtonReleased) (MouseButton);
    Vector2 (*GetMouseCoordinates) (void);
};

#endif