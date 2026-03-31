#ifndef LEAF_H
#define LEAF_H

#include "raylib.h"

class Leaf
{
public:
    Leaf(Vector2 pos, Texture2D* leafTexture, Texture2D* leafTexture2); 
    void tick(float deltaTime,float map);

private:
    Vector2 position;
    Texture2D texture;
    Texture2D texture1st;
    Texture2D texture2nd;
    Vector2 velocity;
    float frames{0.f};
    float maxFrames{4.f};
    float width;
    float height;
    float updateTime{1.f / 12.f}; 
    float runningTime{0.f};
    float angle{0.f};
    float rotationSpeed;
    
    float swayTime;
    float swaySpeed;
    float swayAmount;
    
    float scale; 
};

#endif