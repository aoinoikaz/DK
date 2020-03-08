#ifndef TEXTURE_H
#define TEXTURE_H

#include "math_helper.h"
#include "iostream"

struct Texture
{
    Texture() = default;
    Texture(std::string path, Rect src, Rect dest, float rot = 0.0f, bool flip = false)
    {
        filePath = path;
        source = src;
        destination = dest;
        rotation = rot;
        flipped = flip;
    }

    std::string filePath;
    Rect source;
    Rect destination;
    float rotation;
    bool flipped;
};

#endif