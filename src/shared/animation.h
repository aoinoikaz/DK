#ifndef ANIMATION_H
#define ANIMATION_H

struct Animation
{
    enum WRAP_MODE { once = 0, loop = 1 };
    WRAP_MODE WrapMode;

    int ClipX, ClipY, ClipW, ClipH;
    int Frames;
    int Rows;
    float AnimationSpeed;
    float TimePerFrame;

    bool Flipped;
    const char* Name;

    Animation() = default;
    Animation(const char* name, int frames, int rows, float animSpeed, int clipx, int clipy, int clipw, int cliph, WRAP_MODE wrapMode)
    {
        Name = name;
        Frames = frames;
        Rows = rows;
        AnimationSpeed = animSpeed;
        TimePerFrame = AnimationSpeed / Frames;
        ClipX = clipx;
        ClipY = clipy;
        ClipW = clipw;
        ClipH = cliph;
        WrapMode = wrapMode;
        Flipped = false;
    }
};

#endif